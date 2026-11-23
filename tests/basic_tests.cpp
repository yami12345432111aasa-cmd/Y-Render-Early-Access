#include "yrender/memory_cache.hpp"
#include "yrender/resource_predictor.hpp"
#include "yrender/diagnostics.hpp"
#include <cassert>
#include <cstddef>
#include <vector>

int main() {
    using namespace yrender;
    MemoryCache cache(16);
    std::vector<std::byte> data{std::byte{1}, std::byte{2}, std::byte{3}};
    assert(cache.put("sample", data));
    std::vector<std::byte> out;
    assert(cache.get("sample", out));
    assert(out.size() == 3);
    assert(cache.used_bytes() == 3);

    ResourcePredictor predictor;
    predictor.observe("texture_a");
    predictor.observe("texture_a");
    predictor.observe("texture_b");
    assert(predictor.predict("texture_a"));
    assert(!predictor.predict("missing"));

    PerformanceSample sample{0, 0, -1, -1, "test"};
    assert(!BottleneckDetector::evaluate(sample).empty());
    return 0;
}
