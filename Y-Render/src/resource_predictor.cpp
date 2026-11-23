#include "yrender/resource_predictor.hpp"
namespace yrender {
void ResourcePredictor::observe(const std::string& r) { ++counts_[r]; ++total_; }
bool ResourcePredictor::predict(const std::string& r) const {
    const auto it = counts_.find(r);
    // Conservative frequency heuristic; this is not predictive access to game internals.
    return it != counts_.end() && total_ >= 3 && it->second * 4 >= total_;
}
std::size_t ResourcePredictor::observations() const { return total_; }
}
