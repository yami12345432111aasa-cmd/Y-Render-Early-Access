#pragma once
#include "yrender/types.hpp"
#include <string>
namespace yrender {
class BottleneckDetector {
public:
    static std::string evaluate(const PerformanceSample& sample);
};
}
