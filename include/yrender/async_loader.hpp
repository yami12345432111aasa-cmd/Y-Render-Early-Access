#pragma once
#include <future>
#include <string>
#include <vector>
#include <cstddef>
namespace yrender {
class AsyncLoader {
public:
    std::future<std::vector<std::byte>> load_file_async(std::string path) const;
};
}
