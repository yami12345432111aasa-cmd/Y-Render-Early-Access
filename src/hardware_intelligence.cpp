#include "yrender/hardware_intelligence.hpp"
#include <iostream>
#include <thread>
#if defined(_WIN32)
#define NOMINMAX
#include <windows.h>
#elif defined(__linux__)
#include <sys/sysinfo.h>
#include <unistd.h>
#endif

namespace yrender {
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
#elif defined(__linux__)
    info.operating_system = "Linux";
    info.architecture = sizeof(void*) == 8 ? "x64" : "x86";
    struct sysinfo mem{};
    if (sysinfo(&mem) == 0) {
        info.total_ram_bytes = static_cast<std::uint64_t>(mem.totalram) * mem.mem_unit;
        info.available_ram_bytes = static_cast<std::uint64_t>(mem.freeram + mem.bufferram) * mem.mem_unit;
    }
#elif defined(__APPLE__)
    info.operating_system = "macOS";
    info.architecture = sizeof(void*) == 8 ? "64-bit" : "32-bit";
#else
    info.operating_system = "Unknown";
    info.architecture = sizeof(void*) == 8 ? "64-bit" : "32-bit";
#endif
    return info;
}
void HardwareIntelligence::print(const HardwareInfo& i) {
    const auto mib = [](std::uint64_t n) { return static_cast<double>(n) / (1024.0 * 1024.0); };
    std::cout << "OS: " << i.operating_system << " (" << i.architecture << ")\\n"
              << "Logical threads: " << i.logical_threads << "\\n"
              << "RAM total: " << mib(i.total_ram_bytes) << " MiB\\n"
              << "RAM available: " << mib(i.available_ram_bytes) << " MiB\\n"
              << "GPU: " << i.gpu_note << "\\n";
}
}
