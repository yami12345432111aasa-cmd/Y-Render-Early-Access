#pragma once
#include <filesystem>
#include <string>
namespace yrender {
struct FileAnalysis { bool exists{false}; std::uintmax_t bytes{0}; std::string extension; std::string note; };
class MediaAnalyzer {
public:
    static FileAnalysis analyze(const std::filesystem::path& path);
};
}
