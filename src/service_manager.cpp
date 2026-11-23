#include "yrender/service_manager.hpp"
namespace yrender {
std::vector<std::string> ServiceManager::safety_policy() {
    return {
        "Service inspection is not implemented in this portable starter.",
        "No services are stopped, disabled, or modified.",
        "Never disable security, networking, audio, storage, or operating-system critical services.",
        "Any future service controls must require explicit consent and provide rollback."
    };
}
}
