#pragma once

#include "../control/control.h"

struct DefaultDeleter {
    template <typename T>
    void operator()(T* ptr) {
        delete ptr;
    }
};

struct DefaultArrayDeleter {
    template <typename T>
    void operator()(T* ptr) {
        delete[] ptr;
    }
};

template <typename T>
struct DeleterType {
    using deleter = DefaultDeleter;
};

template <typename T>
struct DeleterType<T[]> {
    using deleter = DefaultArrayDeleter;
};

// SharedPtr
template <typename T>
class WeakPtr;

template <typename T>
class SharedPtr {
public:
    using element_type = T;
    using deleter_type = typename DeleterType<T>::deleter;

    constexpr SharedPtr() noexcept = default;

    ~SharedPtr() {
        Reset();
    }

    template <typename Y>
    explicit SharedPtr(Y* p) : value_{p}, control_{new ControlBlock<Y, deleter_type>(p)} {
    }

    template <typename Y, typename Deleter>
    SharedPtr(Y* p, Deleter /*deleter*/) noexcept
        : value_{p}, control_{new ControlBlock<Y, Deleter>(p)} {
    }

    SharedPtr(const SharedPtr& other) noexcept : value_{other.value_}, control_{other.control_} {
        control_->Add();
    }

    SharedPtr(SharedPtr&& other) noexcept : value_{other.value_}, control_{other.control_} {
        other.value_ = nullptr;
        other.control_ = nullptr;
    }

    SharedPtr& operator=(const SharedPtr& r) noexcept {
        Reset();
        value_ = r.value_;
        control_ = r.control_;
        control_->Add();
        return *this;
    }

    template <typename Y>
    SharedPtr& operator=(const SharedPtr<Y>& r) noexcept {
        Reset();
        value_ = r.value_;
        control_ = r.control_;
        control_->Add();
        return *this;
    }

    SharedPtr& operator=(SharedPtr&& r) noexcept {
        Reset();
        value_ = r.value_;
        control_ = r.control_;
        r.value_ = nullptr;
        r.control_ = nullptr;
        return *this;
    }

    template <typename Y>
    SharedPtr& operator=(SharedPtr<Y>&& r) noexcept {
        Reset();
        value_ = r.value_;
        control_ = r.control_;
        r.value_ = nullptr;
        r.control_ = nullptr;
        return *this;
    }

    // Modifiers
    void Reset() noexcept {
        if (control_ == nullptr) {
            return;
        }

        control_->Remove();
        if (control_->IsZeroWeak()) {
            delete control_;
        }

        value_ = nullptr;
        control_ = nullptr;
    }

    template <typename Y>
    void Reset(Y* p) noexcept {
        Reset();
        value_ = p;
        control_ = new ControlBlock<Y, deleter_type>(p);
    }

    template <typename Y, typename Deleter>
    void Reset(Y* p, Deleter /* deleter */) noexcept {
        Reset();
        value_ = p;
        control_ = new ControlBlock<Y, Deleter>(p);
    }

    void Swap(SharedPtr& other) noexcept {
        std::swap(value_, other.value_);
        std::swap(control_, other.control_);
    }

    // Observers
    T* Get() const noexcept {
        return value_;
    }

    int64_t UseCount() const noexcept {
        if (control_ == nullptr) {
            return 0;
        }
        return control_->Count();
    }

    T& operator*() const noexcept {
        return *value_;
    }

    T* operator->() const noexcept {
        return value_;
    }

    element_type& operator[](std::ptrdiff_t idx) const {
        return value_[idx];
    }

    explicit operator bool() const noexcept {
        return value_;
    }

    template <typename U>
    friend class WeakPtr;

private:
    T* value_{nullptr};
    SharedWeakCount* control_{nullptr};
};

template <typename T, typename... Args>
SharedPtr<T> MakeShared(Args&&... args) {
    return SharedPtr<T>(new T(std::forward<Args>(args)...));
}

// WeakPtr
template <typename T>
class WeakPtr {

public:
    using element_type = T;
    using deleter_type = typename DeleterType<T>::deleter;

    // Special-member functions
    constexpr WeakPtr() noexcept = default;

    template <typename Y>
    explicit WeakPtr(const SharedPtr<Y>& other) : value_{other.value_}, control_{other.control_} {
        control_->AddWeak();
    }

    WeakPtr(const WeakPtr& other) noexcept : value_{other.value_}, control_{other.conrol_} {
        control_->AddWeak();
    }

    WeakPtr(WeakPtr&& other) noexcept : value_{other.value_}, control_{other.control_} {
        other.value_ = nullptr;
        other.control_ = nullptr;
    }

    template <typename Y>
    WeakPtr& operator=(const SharedPtr<Y>& other) {
        Reset();
        value_ = other.value_;
        control_ = other.control_;
        control_->AddWeak();
        return *this;
    }

    WeakPtr& operator=(const WeakPtr& other) noexcept {
        Reset();
        value_ = other.value_;
        control_ = other.control_;
        control_->AddWeak();
        return *this;
    }

    WeakPtr& operator=(WeakPtr&& other) noexcept {
        Reset();
        value_ = other.value_;
        control_ = other.control_;
        other.value_ = nullptr;
        other.control_ = nullptr;
        return *this;
    }

    ~WeakPtr() {
        Reset();
    }

    // Modifiers
    void Reset() noexcept {
        if (control_ == nullptr) {
            return;
        }

        control_->RemoveWeak();
        if (control_->IsZeroWeak()) {
            delete control_;
        }

        value_ = nullptr;
        control_ = nullptr;
    }

    void Swap(WeakPtr<T>& other) noexcept {
        std::swap(value_, other.value_);
        std::swap(control_, other.control_);
    }

    // Observers
    bool Expired() const noexcept {
        return control_ == nullptr || control_->IsZero();
    }

    SharedPtr<T> Lock() const noexcept {
        SharedPtr<T> shared;
        if (Expired()) {
            return shared;
        }
        shared.value_ = value_;
        shared.control_ = control_;
        shared.control_->Add();
        return shared;
    }

    template <typename U>
    friend class SharedPtr;

public:
    T* value_{nullptr};
    SharedWeakCount* control_{nullptr};
};
