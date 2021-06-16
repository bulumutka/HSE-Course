#pragma once

#include <type_traits>
#include <utility>

#include "utility.h"

template <typename T, typename... Args>
struct LibCppIsConstructible;

template <typename Derived, typename Base>
struct IsInvalidBaseToDerivedCast {
    using derived = uncvref_t<Derived>;
    using base = uncvref_t<Base>;

    static constexpr bool kValue = !std::is_same_v<base, derived> &&
                                   std::is_base_of_v<base, derived> &&
                                   !LibCppIsConstructible<derived, base>::type::value;
};

template <typename To, typename From>
struct IsInvalidLvalueToRvalueCast : std::false_type {};

template <typename RefTo, typename RefFrom>
struct IsInvalidLvalueToRvalueCast<RefTo&&, RefFrom&> {
    using to = uncvref_t<RefTo>;
    using from = uncvref_t<RefFrom>;
    static constexpr bool kValue = std::is_same_v<from, to> && std::is_base_of_v<to, from>;
};

struct IsConstructibleHelper {
    template <typename T, typename... Args, typename = decltype(T(Declval<Args>()...))>
    static std::true_type CheckConstructible(int);

    template <typename, typename>
    static std::false_type CheckConstructible(...);

    template <typename To, typename From, typename = decltype(ImplicitCast<To>(Declval<From>()))>
    static std::true_type CheckCastable(int);

    template <typename To, typename From, typename = decltype(static_cast<To>(Declval<From>()))>
    static std::integral_constant<bool, !IsInvalidBaseToDerivedCast<To, From>::kValue &&
                                            !IsInvalidLvalueToRvalueCast<To, From>::kValue>
        CheckCastable(int64_t);

    template <typename, typename>
    static std::false_type CheckCastable(...);
};

template <typename T, typename... Args>
struct LibCppIsConstructible {
    using type = decltype(IsConstructibleHelper::CheckConstructible<T, Args...>(0));
};

template <typename T, typename V>
struct LibCppIsConstructible<T, V> {
    using type = decltype(IsConstructibleHelper::CheckConstructible<T, V>(0));
};

template <typename T, typename V>
struct LibCppIsConstructible<T&, V> {
    using type = decltype(IsConstructibleHelper::CheckCastable<T&, V>(0));
};

template <typename T, typename V>
struct LibCppIsConstructible<T&&, V> {
    using type = decltype(IsConstructibleHelper::CheckCastable<T&&, V>(0));
};

template <typename T, typename... Args>
struct IsConstructible : public LibCppIsConstructible<T, Args...>::type {};

template <typename T>
struct IsCopyConstructible : public LibCppIsConstructible<T, const T&>::type {};
