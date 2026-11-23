#pragma once
#include <cstddef>
#include <string>
#include <unordered_map>
namespace yrender {
class ResourcePredictor {
public:
    void observe(const std::string& resource);
    bool predict(const std::string& resource) const;
    std::size_t observations() const;
private:
    std::unordered_map<std::string, std::size_t> counts_;
    std::size_t total_{0};
};
}
