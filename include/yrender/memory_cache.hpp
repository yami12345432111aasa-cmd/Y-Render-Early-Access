#pragma once
#include <cstddef>
#include <list>
#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>

namespace yrender {
// Small thread-safe LRU byte cache. It stores data supplied by the caller;
// it does not intercept a game's memory or assets automatically.
class MemoryCache {
public:
    explicit MemoryCache(std::size_t capacity_bytes);
    bool put(const std::string& key, std::vector<std::byte> value);
    bool get(const std::string& key, std::vector<std::byte>& out);
    void clear();
    std::size_t used_bytes() const;
private:
    struct Entry { std::vector<std::byte> value; std::list<std::string>::iterator age; };
    void evict_until(std::size_t needed);
    std::size_t capacity_, used_{0};
    mutable std::mutex mutex_;
    std::list<std::string> lru_;
    std::unordered_map<std::string, Entry> entries_;
};
}
