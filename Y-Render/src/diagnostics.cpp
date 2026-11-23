#include "yrender/diagnostics.hpp"
namespace yrender {
std::string BottleneckDetector::evaluate(const PerformanceSample& s) {
    if (s.fps <= 0.0 || s.frame_time_ms <= 0.0) return "Insufficient data: capture FPS and frame time first.";
    if (s.cpu_percent >= 90.0 && s.cpu_percent > 0.0) return "Possible CPU limitation; validate per-core usage and game thread.";
    if (s.memory_mb > 0.0) return "No clear bottleneck from supplied sample; inspect GPU utilization, VRAM and frame-time spikes.";
    return "Insufficient telemetry. Add CPU, GPU and memory measurements for a stronger diagnosis.";
}
}
