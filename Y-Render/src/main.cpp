#include "yrender/hardware_intelligence.hpp"
#include "yrender/diagnostics.hpp"
#include "yrender/evolution_engine.hpp"
#include "yrender/media_analyzer.hpp"
#include "yrender/service_manager.hpp"
#include <iostream>

int main() {
    using namespace yrender;
    std::cout << "====================================\\n"
                 "             Y-RENDER 0.1.0          \\n"
                 "====================================\\n"
                 "Starter build | C++20 | Cross-platform\\n\\n";
    std::cout << "[Y-Hardware Intelligence]\\n";
    HardwareIntelligence::print(HardwareIntelligence::detect());

    std::cout << "\\n[ Y-Bottleneck Detector ]\\n";
    PerformanceSample sample{60.0, 16.67, -1.0, -1.0, "Baseline"};
    std::cout << BottleneckDetector::evaluate(sample) << "\\n";

    std::cout << "\\n[ Y-Evolution Engine ]\\n";
    EvolutionEngine evolution;
    evolution.add_sample(sample);
    std::cout << "Samples: " << evolution.history().size() << "\\n";
    std::cout << "Best profile: " << evolution.best_profile() << "\\n";

    std::cout << "\\n[ Y-Service Manager ]\\n";
    for (const auto& line : ServiceManager::safety_policy()) std::cout << "- " << line << "\\n";

    std::cout << "\\nY-Render core initialized. See README.md for implemented and planned features.\\n";
    return 0;
}
