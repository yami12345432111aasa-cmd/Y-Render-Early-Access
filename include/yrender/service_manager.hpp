#pragma once
#include <string>
#include <vector>
namespace yrender {
// Deliberately read-only. No Windows services are stopped or reconfigured.
class ServiceManager {
public:
    static std::vector<std::string> safety_policy();
};
}
