#include "yrender/async_loader.hpp"
#include <fstream>
#include <iterator>
#include <stdexcept>
namespace yrender {
std::future<std::vector<std::byte>> AsyncLoader::load_file_async(std::string path) const {
    return std::async(std::launch::async, [p = std::move(path)] {
        std::ifstream file(p, std::ios::binary);
        if (!file) throw std::runtime_error("Cannot open file: " + p);
        std::vector<char> chars((std::istreambuf_iterator<char>(file)), {});
        std::vector<std::byte> bytes(chars.size());
        for (std::size_t i = 0; i < chars.size(); ++i)
            bytes[i] = static_cast<std::byte>(static_cast<unsigned char>(chars[i]));
        return bytes;
    });
}
}
