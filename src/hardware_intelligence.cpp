#include "yrender/hardware_intelligence.hpp"
#include <iostream>
#include <thread>
#include <vector>
#include <string>

#if defined(_WIN32)
#define NOMINMAX
#include <windows.h>
#include <dxgi.h>
#pragma comment(lib, "dxgi.lib")
#elif defined(__linux__)
#include <sys/sysinfo.h>
#include <unistd.h>
#endif

namespace yrender {

// Helper para convertir WCHAR a std::string (Windows)
#ifdef _WIN32
static std::string wide_to_narrow(const wchar_t* wstr) {
    if (!wstr) return "";
    int size = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, nullptr, 0, nullptr, nullptr);
    if (size <= 0) return "";
    std::string result(size - 1, '\0');
    WideCharToMultiByte(CP_UTF8, 0, wstr, -1, &result[0], size, nullptr, nullptr);
    return result;
}
#endif

HardwareInfo HardwareIntelligence::detect() {
    HardwareInfo info;
    info.logical_threads = std::thread::hardware_concurrency();

#if defined(_WIN32)
    info.operating_system = "Windows";
    SYSTEM_INFO si{};
    GetNativeSystemInfo(&si);
    info.architecture = (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64) ? "x64" :
                        (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_INTEL) ? "x86" : "Unknown";

    MEMORYSTATUSEX mem{};
    mem.dwLength = sizeof(mem);
    if (GlobalMemoryStatusEx(&mem)) {
        info.total_ram_bytes = mem.ullTotalPhys;
        info.available_ram_bytes = mem.ullAvailPhys;
    }

    // === GPU DETECTION: Enumera TODAS las GPUs (integrada + dedicada) ===
    IDXGIFactory1* factory = nullptr;
    HRESULT hr = CreateDXGIFactory1(__uuidof(IDXGIFactory1), reinterpret_cast<void**>(&factory));
    
    if (SUCCEEDED(hr) && factory) {
        IDXGIAdapter1* adapter = nullptr;
        UINT index = 0;
        
        while (factory->EnumAdapters1(index, &adapter) != DXGI_ERROR_NOT_FOUND) {
            DXGI_ADAPTER_DESC1 desc;
            if (SUCCEEDED(adapter->GetDesc1(&desc))) {
                GpuInfo gpu;
                gpu.name = wide_to_narrow(desc.Description);
                gpu.dedicated_vram_mb = desc.DedicatedVideoMemory / (1024ULL * 1024ULL);
                gpu.shared_vram_mb   = desc.SharedSystemMemory   / (1024ULL * 1024ULL);
                
                // Detectar si es integrada o dedicada por flags y VRAM dedicada
                bool is_software = (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) != 0;
                bool has_dedicated = desc.DedicatedVideoMemory > (512ULL * 1024 * 1024); // >512MB
                
                if (is_software) {
                    gpu.type = "Software/Microsoft Basic";
                } else if (has_dedicated) {
                    gpu.type = "Dedicated";
                } else {
                    gpu.type = "Integrated";
                }
                
                info.gpus.push_back(gpu);
            }
            adapter->Release();
            ++index;
        }
        
        factory->Release();
        
        // Elegir la GPU principal: la dedicada con más VRAM
        const GpuInfo* primary = nullptr;
        for (const auto& g : info.gpus) {
            if (g.type == "Dedicated") {
                if (!primary || g.dedicated_vram_mb > primary->dedicated_vram_mb) {
                    primary = &g;
                }
            }
        }
        // Si no hay dedicada, usar la primera no-software
        if (!primary) {
            for (const auto& g : info.gpus) {
                if (g.type != "Software/Microsoft Basic") { primary = &g; break; }
            }
        }
        
        if (primary) {
            info.gpu_name = primary->name;
            info.gpu_vram_mb = primary->dedicated_vram_mb;
            info.gpu_type = primary->type;
            info.gpu_note = "Detected (" + std::to_string(info.gpus.size()) + " GPU(s) found)";
        } else {
            info.gpu_note = "No GPU adapters detected via DXGI";
        }
    } else {
        info.gpu_note = "DXGI initialization failed (hr=" + std::to_string(hr) + ")";
    }

#elif defined(__linux__)
    info.operating_system = "Linux";
    info.architecture = sizeof(void*) == 8 ? "x64" : "x86";
    struct sysinfo mem{};
    if (sysinfo(&mem) == 0) {
        info.total_ram_bytes = static_cast<std::uint64_t>(mem.totalram) * mem.mem_unit;
        info.available_ram_bytes = static_cast<std::uint64_t>(mem.freeram + mem.bufferram) * mem.mem_unit;
    }
    // TODO: detección de GPU vía /sys/class/drm/card*/device/ o NVML
    info.gpu_note = "GPU detection not implemented on Linux yet";

#elif defined(__APPLE__)
    info.operating_system = "macOS";
    info.architecture = sizeof(void*) == 8 ? "64-bit" : "32-bit";
    // TODO: IKit / Metal device enumeration
    info.gpu_note = "GPU detection not implemented on macOS yet";
#else
    info.operating_system = "Unknown";
    info.architecture = sizeof(void*) == 8 ? "64-bit" : "32-bit";
    info.gpu_note = "Unsupported platform";
#endif

    return info;
}

void HardwareIntelligence::print(const HardwareInfo& i) {
    const auto mib = [](std::uint64_t n) { return static_cast<double>(n) / (1024.0 * 1024.0); };
    
    std::cout << "OS: " << i.operating_system << " (" << i.architecture << ")\n"
              << "Logical threads: " << i.logical_threads << "\n"
              << "RAM total: " << mib(i.total_ram_bytes) << " MiB\n"
              << "RAM available: " << mib(i.available_ram_bytes) << " MiB\n";
    
    // Imprimir TODAS las GPUs detectadas
    if (!i.gpus.empty()) {
        std::cout << "GPUs detected: " << i.gpus.size() << "\n";
        for (size_t idx = 0; idx < i.gpus.size(); ++idx) {
            const auto& g = i.gpus[idx];
            std::cout << "  [" << idx << "] " << g.name 
                      << " | Tipo: " << g.type
                      << " | VRAM dedicada: " << g.dedicated_vram_mb << " MB"
                      << " | VRAM compartida: " << g.shared_vram_mb << " MB\n";
        }
    }
    
    // GPU principal seleccionada
    if (!i.gpu_name.empty()) {
        std::cout << "GPU principal: " << i.gpu_name 
                  << " (" << i.gpu_type << ", " << i.gpu_vram_mb << " MB)\n";
    }
    std::cout << "GPU: " << i.gpu_note << "\n";
}

} // namespace yrender
