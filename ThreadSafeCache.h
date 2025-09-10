#pragma once

#include <unordered_map>
#include <mutex>
#include <optional>
#include <functional>
#include <concepts>

// C++23 concepts for better type safety
template<typename K>
concept Hashable = requires(K k) { std::hash<K>{}(k); };

template<typename F, typename K, typename V>
concept LoaderFunction = std::invocable<F, K> && std::convertible_to<std::invoke_result_t<F, K>, V>;

template<Hashable Key, typename Value>
class ThreadSafeCache {
public:
    using Loader = std::function<Value(const Key&)>;

    // C++23 simplified constructor with perfect forwarding
    explicit ThreadSafeCache(auto&& loader = nullptr)
        requires LoaderFunction<std::decay_t<decltype(loader)>, Key, Value> || std::same_as<std::decay_t<decltype(loader)>, std::nullptr_t>
        : loader_(std::forward<decltype(loader)>(loader)) {}

    // Simplified get with C++23 auto and proper scoping
    auto get(const Key& key) -> std::optional<Value> {
        // Try to find in cache first
        {
            std::lock_guard lock{mutex_};
            if (auto it = cache_.find(key); it != cache_.end()) {
                return it->second;
            }
        }

        // Load if loader available
        if (!loader_) return std::nullopt;

        auto loaded = loader_(key);
        std::lock_guard lock{mutex_};
        return cache_.try_emplace(key, loaded).first->second;
    }

    // Simplified methods using C++23 features
    void put(const Key& key, auto&& value) 
        requires std::convertible_to<std::decay_t<decltype(value)>, Value> {
        std::lock_guard lock{mutex_};
        cache_[key] = std::forward<decltype(value)>(value);
    }

    bool contains(const Key& key) const {
        std::lock_guard lock{mutex_};
        return cache_.contains(key);  // C++20 feature, but simplified
    }

    bool erase(const Key& key) {
        std::lock_guard lock{mutex_};
        return cache_.erase(key) > 0;
    }

    void clear() {
        std::lock_guard lock{mutex_};
        cache_.clear();
    }

    auto size() const {
        std::lock_guard lock{mutex_};
        return cache_.size();
    }

private:
    mutable std::mutex mutex_;
    std::unordered_map<Key, Value> cache_;
    Loader loader_;
};
