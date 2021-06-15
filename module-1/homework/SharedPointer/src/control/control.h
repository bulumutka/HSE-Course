#pragma once

#include <atomic>
#include <cassert>

class SharedCount {
public:
    virtual ~SharedCount() = default;

    size_t Count() const {
        return value_.load();
    }

    bool IsZero() const {
        return Count() == 0;
    }

    void Add() {
        ++value_;
    }

    virtual void Remove() {
        assert(!IsZero());
        --value_;
    }

protected:
    std::atomic<size_t> value_{0};
};

class SharedWeakCount : public SharedCount {
public:
    bool IsZeroWeak() const {
        return IsZero() && valueWeak_.load() == 0;
    }

    void AddWeak() {
        valueWeak_++;
    }

    void RemoveWeak() {
        assert(valueWeak_.load() != 0);
        --valueWeak_;
    }

protected:
    std::atomic<size_t> valueWeak_{0};
};

template <typename T, typename Deleter>
class ControlBlock : public SharedWeakCount {
public:
    explicit ControlBlock(T* value) : value_(value) {
        Add();
    }

    virtual void Remove() override {
        SharedCount::Remove();
        if (IsZero()) {
            Deleter del;
            del(value_);
            value_ = nullptr;
        }
    }

private:
    T* value_ = nullptr;
};
