#pragma once
#include <cstdint>
#include <string>
#include <vector>

namespace yrender {
struct HardwareInfo {
    std::string operating_system;
    std::string architecture;
    unsigned int logical_threads{0};
    std::uint64_t total_ram_bytes{0};
    std::uint64_t available_ram_bytes{0};
    std::string cpu_name{"Unknown (portable detection)"};
    std::string gpu_note{"GPU details require a platform-specific graphics API adapter"};
};

struct ResourceEvent {
    std::string resource;
    bool predicted{false};
};

struct PerformanceSample {
    double fps{0.0};
    double frame_time_ms{0.0};
    double cpu_percent{-1.0};
    double memory_mb{-1.0};
    std::string profile;
};
}
