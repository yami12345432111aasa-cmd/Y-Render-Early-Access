#include "yrender/memory_cache.hpp"
namespace yrender {
MemoryCache::MemoryCache(std::size_t cap) : capacity_(cap) {}
void MemoryCache::evict_until(std::size_t needed) {
    while (!lru_.empty() && used_ + needed > capacity_) {
        const auto key = lru_.back();
        auto it = entries_.find(key);
        if (it != entries_.end()) { used_ -= it->second.value.size(); entries_.erase(it); }
        lru_.pop_back();
    }
}
bool MemoryCache::put(const std::string& key, std::vector<std::byte> value) {
    std::lock_guard lock(mutex_);
    if (value.size() > capacity_) return false;
    auto old = entries_.find(key);
    if (old != entries_.end()) {
        used_ -= old->second.value.size();
        lru_.erase(old->second.age);
        entries_.erase(old);
    }
    evict_until(value.size());
    lru_.push_front(key);
    used_ += value.size();
    entries_.emplace(key, Entry{std::move(value), lru_.begin()});
    return true;
}
bool MemoryCache::get(const std::string& key, std::vector<std::byte>& out) {
    std::lock_guard lock(mutex_);
    auto it = entries_.find(key);
    if (it == entries_.end()) return false;
    lru_.erase(it->second.age);
    lru_.push_front(key);
    it->second.age = lru_.begin();
    out = it->second.value;
    return true;
}
void MemoryCache::clear() { std::lock_guard lock(mutex_); entries_.clear(); lru_.clear(); used_ = 0; }
std::size_t MemoryCache::used_bytes() const { std::lock_guard lock(mutex_); return used_; }
}
