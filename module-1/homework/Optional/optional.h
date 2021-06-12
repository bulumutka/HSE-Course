#include <cstdlib>
#include <type_traits>

#pragma once

namespace task {

struct NullOpt {
    enum class Construct { Token };
    explicit constexpr NullOpt(Construct) {
    }
};

constexpr NullOpt kNullOpt{NullOpt::Construct::Token};

struct InPlace {
    explicit InPlace() = default;
};

constexpr InPlace kInPlace{};

template <typename T>
class Optional;

template <typename T, bool>
class BaseOptional {
public:
    constexpr BaseOptional() noexcept {
    }

    template <typename U = T>
    constexpr explicit BaseOptional(U&& value) : value_(std::forward<U>(value)), hasValue_(true) {
    }

    constexpr explicit BaseOptional(NullOpt) noexcept {
    }

    template <typename... Args>
    constexpr explicit BaseOptional(InPlace, Args&&... args)
        : value_(std::forward<Args>(args)...), hasValue_(true) {
    }

    virtual ~BaseOptional() {
    }

protected:
    template <typename U = T>
    void Construct(U&& value) {
        value = std::forward<U>(value);
        hasValue_ = true;
    }

    void Reset() {
        hasValue_ = false;
    }

    T value_;
    bool hasValue_ = false;
};

template <typename T>
class BaseOptional<T, false> {
public:
    constexpr BaseOptional() noexcept {
    }

    template <typename U = T>
    constexpr explicit BaseOptional(U&& value) : value_(std::forward<U>(value)), hasValue_(true) {
    }

    constexpr explicit BaseOptional(NullOpt) noexcept {
    }

    template <typename... Args>
    constexpr explicit BaseOptional(InPlace, Args&&... args)
        : value_(std::forward<Args>(args)...), hasValue_(true) {
    }

    virtual ~BaseOptional() {
        if (hasValue_) {
            value_.~T();
        }
    }

protected:
    template <typename U = T>
    void Construct(U&& value) {
        if (hasValue_) {
            value_.~T();
        }
        value = std::forward<U>(value);
        hasValue_ = true;
    }

    void Reset() {
        if (hasValue_) {
            value_.~T();
        }
        hasValue_ = false;
    }

    T value_;
    bool hasValue_ = false;
};

template <typename T>
class Optional : public BaseOptional<T, std::is_trivially_destructible<T>::value> {
private:
    using base = BaseOptional<T, std::is_trivially_destructible<T>::value>;

public:
    using value_type = T;

    constexpr Optional() noexcept {
    }

    template <typename U = value_type>
    constexpr explicit Optional(U&& value) : base(std::forward<U>(value)) {
    }

    constexpr explicit Optional(NullOpt) noexcept {
    }

    template <typename... Args>
    constexpr explicit Optional(InPlace, Args&&... args)
        : base(kInPlace, std::forward<Args>(args)...) {
    }

    Optional& operator=(NullOpt) noexcept {
        base::Reset();
        return *this;
    }

    template <typename U = T>
    Optional& operator=(U&& value) {
        base::Construct(std::forward<U>(value));
        return *this;
    }

    void Reset() noexcept {
        base::Reset();
    }

    template <typename U>
    constexpr T ValueOr(U&& default_value) const& {
        return base::hasValue_ ? base::value_ : default_value;
    }

    template <typename U>
    constexpr T ValueOr(U&& default_value) && {
        return base::hasValue_ ? base::value_ : default_value;
    }

    constexpr bool HasValue() const noexcept {
        return base::hasValue_;
    }

    constexpr explicit operator bool() const noexcept {
        return HasValue();
    }

    constexpr std::add_pointer_t<const value_type> operator->() const {
        return base::value_;
    }

    constexpr std::add_pointer_t<value_type> operator->() {
        return &(base::value_);
    }

    constexpr const value_type& operator*() const& {
        return base::value_;
    }

    constexpr value_type& operator*() & {
        return base::value_;
    }

    constexpr const value_type&& operator*() const&& {
        return std::move(base::value_);
    }

    constexpr value_type&& operator*() && {
        return std::move(base::value_);
    }
};

}  // namespace task
