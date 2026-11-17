//
// Created by Sennh on 17-11-2026.
//
#pragma once
#include <mutex>
#include <optional>
#include <utility>

template <typename T>
class Mailbox {
public:
    void set(T value) {
        std::lock_guard<std::mutex> lock(mutex_);
        value_ = std::move(value);
        hasValue_ = true;
    }
    std::optional<T> latest() const {
        std::lock_guard<std::mutex> lock(mutex_);
        if (!hasValue_) return std::nullopt;
        return value_;
    }
private:
    mutable std::mutex mutex_;
    T value_{};
    bool hasValue_ = false;
};