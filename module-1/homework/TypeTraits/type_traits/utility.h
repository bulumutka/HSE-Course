#pragma once

#include <type_traits>
#include <utility>

template <typename T>
struct RemoveConst {
    using type = T;
};

template <typename T>
struct RemoveConst<T const> {
    using type = T;
};

template <typename T>
struct RemoveVolatile {
    using type = T;
};

template <typename T>
struct RemoveVolatile<volatile T> {
    using type = T;
};

template <typename T>
struct RemoveRef {
    using type = T;
};

template <typename T>
struct RemoveRef<T&> {
    using type = T;
};

template <typename T>
struct RemoveRef<T&&> {
    using type = T;
};

template <typename T>
using remove_const = typename RemoveConst<T>::type;

template <typename T>
using remove_volatile = typename RemoveVolatile<T>::type;

template <typename T>
using remove_reference = typename RemoveRef<T>::type;

template <typename T>
struct Uncvref {
    using type = remove_const<remove_volatile<remove_reference<T>>>;
};

template <typename T>
using uncvref_t = typename Uncvref<T>::type;

template <typename T>
struct AddConst {
    using type = const T;
};

template <typename T>
using add_const_t = typename AddConst<T>::type;

template <typename T>
struct AddLvalueReference {
    using type = T&;
};

template <typename T>
struct AddRvalueReference {
    using type = T&&;
};

template <typename T>
using add_lvalue_reference_t = typename AddLvalueReference<T>::type;

template <typename T>
using add_rvalue_reference_t = typename AddRvalueReference<T>::type;

template <typename T>
T&& Declval(int);

template <typename T>
T Declval(int64_t);

template <typename T>
decltype(Declval<T>(0)) Declval() noexcept;

template <typename T>
void ImplicitCast(T) noexcept {
}
