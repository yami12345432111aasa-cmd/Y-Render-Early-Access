#include "yrender/media_analyzer.hpp"
#include <algorithm>
namespace yrender {
FileAnalysis MediaAnalyzer::analyze(const std::filesystem::path& p) {
    FileAnalysis a;
    std::error_code ec;
    a.exists = std::filesystem::is_regular_file(p, ec);
    if (!a.exists || ec) { a.note = "File not found or not a regular file"; return a; }
    a.bytes = std::filesystem::file_size(p, ec);
    a.extension = p.extension().string();
    std::transform(a.extension.begin(), a.extension.end(), a.extension.begin(),
        [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
    a.note = "Metadata only: no conversion or quality-changing compression is performed.";
    return a;
}
}
