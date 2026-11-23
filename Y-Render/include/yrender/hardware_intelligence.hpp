#pragma once
#include "yrender/types.hpp"
namespace yrender {
class HardwareIntelligence {
public:
    static HardwareInfo detect();
    static void print(const HardwareInfo& info);
};
}
