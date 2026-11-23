#include "yrender/evolution_engine.hpp"
#include <algorithm>
namespace yrender {
void EvolutionEngine::add_sample(PerformanceSample s) { samples_.push_back(std::move(s)); }
const std::vector<PerformanceSample>& EvolutionEngine::history() const { return samples_; }
std::string EvolutionEngine::best_profile() const {
    if (samples_.empty()) return "No validated samples yet";
    const auto it = std::max_element(samples_.begin(), samples_.end(),
        [](const auto& a, const auto& b) { return a.fps < b.fps; });
    return it->profile + " (highest recorded FPS; compare equivalent test scenes)";
}
}
