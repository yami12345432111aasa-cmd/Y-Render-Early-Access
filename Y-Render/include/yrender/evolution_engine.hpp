#pragma once
#include "yrender/types.hpp"
#include <vector>
#include <string>
namespace yrender {
class EvolutionEngine {
public:
    void add_sample(PerformanceSample sample);
    const std::vector<PerformanceSample>& history() const;
    std::string best_profile() const;
private:
    std::vector<PerformanceSample> samples_;
};
}
