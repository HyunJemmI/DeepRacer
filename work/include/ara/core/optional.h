// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef ARA_CORE_OPTIONAL_H_
#define ARA_CORE_OPTIONAL_H_

#include <functional>
#include <initializer_list>
#include <memory>
#include <type_traits>
#include <utility>

#include "ara/core/utility.h"

namespace ara
{
namespace core
{

// optional for object types
template <class T>
class Optional;

// Disengaged state indicator
struct nullopt_t
{
    struct placeholder
    {};

    explicit constexpr nullopt_t(placeholder)
    {}
};

constexpr nullopt_t nullopt{nullopt_t::placeholder()};

namespace  // unnamed
{
template <typename T>
struct is_optional : std::false_type
{};

template <typename T>
struct is_optional<Optional<T>> : std::true_type
{};

template <typename U, typename T>
using EnableIfConvertible = typename std::enable_if<std::is_convertible<U, T>::value>::type;
template <typename T>
using EnableIfCopyConstructible = typename std::enable_if<std::is_copy_constructible<T>::value>::type;
template <typename T>
using EnableIfMoveConstructible = typename std::enable_if<std::is_move_constructible<T>::value>::type;
template <typename T>
using EnableIfNotOptional = typename std::enable_if<!is_optional<typename std::decay<T>::type>::value>::type;
template <typename T>
using EnableIfLValueReference = typename std::enable_if<std::is_lvalue_reference<T>::value>::type;
template <typename T>
using EnableIfNotLValueReference = typename std::enable_if<!std::is_lvalue_reference<T>::value>::type;
template <typename T, typename... Args>
using EnableIfConstructible = typename std::enable_if<std::is_constructible<T, Args...>::value>::type;
}  // unnamed namespace

/// @brief A container that contains key-value pairs with unique keys
/// @uptrace{SWS_CORE_01033}
/// @uptrace{SWS_CORE_01030}
/// @uptrace{SWS_CORE_01031}
template <class T>
class Optional
{
  public:
    typedef T value_type;

    /// @brief Default constructor
    constexpr Optional() noexcept : mInit(false), mVal(nullptr)
    {}

    constexpr Optional(nullopt_t) noexcept : mInit(false), mVal(nullptr)
    {}

    // Delete the predefined copy-constructor (for volatile, since we don't need it).
    // Doing so prevents another copy constructor from being implicitly declared.
    // This will allow us to SFINAE-in the copy-constructor when needed.
    Optional(const volatile Optional&) = delete;

    template <typename T_ = T, typename = EnableIfCopyConstructible<T_>>
    Optional(const Optional<T>& other) : mInit(other.mInit), mVal(other.mInit ? new T(*other.mVal) : nullptr)
    {}

    // Delete the predefined move-constructor (for volatile, since we don't need it).
    // Doing so prevents another move constructor from being implicitly declared.
    // This will allow us to SFINAE-in the move-constructor when needed.
    Optional(volatile Optional&&) = delete;

    template <typename T_ = T, typename = EnableIfMoveConstructible<T_>>
    Optional(Optional<T>&& other) noexcept(std::is_nothrow_move_constructible<T>::value)
        : mInit(other.mInit), mVal(std::move(other.mVal))
    {}

    // Delete the predefined copy-constructor (for volatile, since we don't need it).
    // Doing so prevents another copy constructor from being implicitly declared.
    // This will allow us to SFINAE-in the copy-constructor when needed.
    Optional(const volatile T&) = delete;

    template <typename T_ = T, typename = EnableIfCopyConstructible<T_>>
    constexpr Optional(const T& v) : mInit(true), mVal(new T(v))
    {}

    // Delete the predefined move-constructor (for volatile, since we don't need it).
    // Doing so prevents another move constructor from being implicitly declared.
    // This will allow us to SFINAE-in the move-constructor when needed.
    Optional(volatile T&&) = delete;

    template <typename T_ = T, typename = EnableIfMoveConstructible<T_>>
    constexpr Optional(T&& v) : mInit(true), mVal(new T(std::move(v)))
    {}

    template <class... Args, typename T_ = T, typename = EnableIfConstructible<T_, Args&&...>>
    constexpr explicit Optional(in_place_t, Args&&... args) : mInit(true), mVal(new T(std::forward<Args>(args)...))
    {}

    template <class U, class... Args, typename T_ = T,
              typename = EnableIfConstructible<T_, std::initializer_list<U>, Args&&...>>
    constexpr explicit Optional(in_place_t, std::initializer_list<U> il, Args&&... args)
        : mInit(true), mVal(new T(il, std::forward<Args>(args)...))
    {}

    ~Optional() = default;

    Optional<T>& operator=(nullopt_t) noexcept
    {
        mInit = false;
        mVal.reset(nullptr);
        return *this;
    }

    Optional<T>& operator=(const Optional<T>& rhs)
    {
        mInit = rhs.mInit;
        mVal.reset(rhs.mInit ? new T(*rhs.mVal) : nullptr);
        return *this;
    }

    Optional<T>& operator=(Optional<T>&& rhs) noexcept(
        std::is_nothrow_move_assignable<T>::value&& std::is_nothrow_move_constructible<T>::value)
    {
        mInit = rhs.mInit;
        mVal.reset(rhs.mInit ? new T(std::move(*rhs.mVal)) : nullptr);
        return *this;
    }

    template <typename U = T,
              typename = typename std::enable_if<
                  !is_optional<typename std::decay<U>::type>::value && std::is_constructible<T, U>::value &&
                  std::is_assignable<T&, U>::value &&
                  (!std::is_same<typename std::decay<U>::type, T>::value || !std::is_scalar<T>::value)>::type>
    Optional<T>& operator=(U&& rhs)
    {
        if (mInit) {
            *mVal = std::forward<U>(rhs);
        } else {
            mInit = true;
            mVal.reset(new T(std::forward<U>(rhs)));
        }
        return *this;
    }

    template <class... Args, typename T_ = T, typename = EnableIfConstructible<T_, Args&&...>>
    void emplace(Args&&... args)
    {
        *this = nullopt;
        mInit = true;
        mVal.reset(new T(std::forward<Args>(args)...));
    }

    template <class U, class... Args, typename T_ = T,
              typename = EnableIfConstructible<T_, std::initializer_list<U>, Args&&...>>
    void emplace(std::initializer_list<U> il, Args&&... args)
    {
        *this = nullopt;
        mInit = true;
        mVal.reset(new T(il, std::forward<Args>(args)...));
    }

    void reset() noexcept
    {
        mInit = false;
        mVal.reset(nullptr);
    }

    template <typename T_ = T, typename = EnableIfMoveConstructible<T_>>
    void swap(Optional<T>& rhs) noexcept(
        std::is_nothrow_move_constructible<T>::value&& noexcept(std::swap(std::declval<T&>(), std::declval<T&>())))
    {
        std::swap(this->mVal, rhs.mVal);
        std::swap(mInit, rhs.mInit);
    }

    constexpr const T* operator->() const
    {
        return mVal.get();
    }

    T* operator->()
    {
        return mVal.get();
    }

    constexpr const T& operator*() const
    {
        return *mVal;
    }

    T& operator*()
    {
        return *mVal;
    }

    constexpr bool has_value() const noexcept
    {
        return mInit;
    }

    constexpr explicit operator bool() const noexcept
    {
        return mInit;
    }

    template <class U, typename T_ = T, typename = EnableIfCopyConstructible<T_>,
              typename = EnableIfConvertible<U&&, T_>>
    constexpr T value_or(U&& v) const&
    {
        return bool(*this) ? **this : static_cast<T>(std::forward<U>(v));
    }

    template <class U, typename T_ = T, typename = EnableIfMoveConstructible<T_>,
              typename = EnableIfConvertible<U&&, T_>>
    T value_or(U&& v) &&
    {
        return bool(*this) ? std::move(**this) : static_cast<T>(std::forward<U>(v));
    }

  private:
    bool mInit;
    std::unique_ptr<T> mVal;
};

/// Relational operators
template <class T>
constexpr bool operator==(const Optional<T>& x, const Optional<T>& y)
{
    return bool(x) != bool(y) ? false : (!bool(x) ? true : *x == *y);
}

template <class T>
constexpr bool operator!=(const Optional<T>& x, const Optional<T>& y)
{
    return !(x == y);
}

template <class T>
constexpr bool operator<(const Optional<T>& x, const Optional<T>& y)
{
    return !bool(y) ? false : (!bool(x) ? true : std::less<T>{}(*x, *y));
}

template <class T>
constexpr bool operator>(const Optional<T>& x, const Optional<T>& y)
{
    return !(x < y) && !(x == y);
}

template <class T>
constexpr bool operator<=(const Optional<T>& x, const Optional<T>& y)
{
    return (x < y) || (x == y);
}

template <class T>
constexpr bool operator>=(const Optional<T>& x, const Optional<T>& y)
{
    return !(x < y);
}

/// Comparison with nullopt
template <class T>
constexpr bool operator==(const Optional<T>& x, nullopt_t) noexcept
{
    return !bool(x);
}

template <class T>
constexpr bool operator==(nullopt_t, const Optional<T>& x) noexcept
{
    return !bool(x);
}

template <class T>
constexpr bool operator!=(const Optional<T>& x, nullopt_t) noexcept
{
    return !(x == nullopt);
}

template <class T>
constexpr bool operator!=(nullopt_t, const Optional<T>& x) noexcept
{
    return !(nullopt == x);
}

template <class T>
constexpr bool operator<(const Optional<T>&, nullopt_t) noexcept
{
    return false;
}

template <class T>
constexpr bool operator<(nullopt_t, const Optional<T>& x) noexcept
{
    return bool(x);
}

template <class T>
constexpr bool operator<=(const Optional<T>& x, nullopt_t) noexcept
{
    return (x < nullopt) || (x == nullopt);
}

template <class T>
constexpr bool operator<=(nullopt_t, const Optional<T>& x) noexcept
{
    return (nullopt < x) || (nullopt == x);
}

template <class T>
constexpr bool operator>(const Optional<T>& x, nullopt_t) noexcept
{
    return !(x < nullopt) && !(x == nullopt);
}

template <class T>
constexpr bool operator>(nullopt_t, const Optional<T>& x) noexcept
{
    return !(nullopt < x) && !(nullopt == x);
}

template <class T>
constexpr bool operator>=(const Optional<T>& x, nullopt_t) noexcept
{
    return !(x < nullopt);
}

template <class T>
constexpr bool operator>=(nullopt_t, const Optional<T>& x) noexcept
{
    return !(nullopt < x);
}

/// Comparison with T
template <class T>
constexpr bool operator==(const Optional<T>& x, const T& v)
{
    return bool(x) ? *x == v : false;
}

template <class T>
constexpr bool operator==(const T& v, const Optional<T>& x)
{
    return bool(x) ? v == *x : false;
}

template <class T>
constexpr bool operator!=(const Optional<T>& x, const T& v)
{
    return !(x == v);
}

template <class T>
constexpr bool operator!=(const T& v, const Optional<T>& x)
{
    return !(v == x);
}

template <class T>
constexpr bool operator<(const Optional<T>& x, const T& v)
{
    return bool(x) ? std::less<T>{}(*x, v) : true;
}

template <class T>
constexpr bool operator<(const T& v, const Optional<T>& x)
{
    return bool(x) ? std::less<T>{}(v, *x) : false;
}

template <class T>
constexpr bool operator<=(const Optional<T>& x, const T& v)
{
    return (x < v) || (x == v);
}

template <class T>
constexpr bool operator<=(const T& v, const Optional<T>& x)
{
    return (v < x) || (v == x);
}

template <class T>
constexpr bool operator>(const Optional<T>& x, const T& v)
{
    return !(x < v) && !(x == v);
}

template <class T>
constexpr bool operator>(const T& v, const Optional<T>& x)
{
    return !(v < x) && !(v == x);
}

template <class T>
constexpr bool operator>=(const Optional<T>& x, const T& v)
{
    return !(x < v);
}

template <class T>
constexpr bool operator>=(const T& v, const Optional<T>& x)
{
    return !(v < x);
}

/// Specialized algorithms
/// @uptrace{SWS_CORE_01096}
template <class T>
void swap(Optional<T>& x, Optional<T>& y) noexcept(noexcept(x.swap(y)))
{
    x.swap(y);
}

template <class T, typename = EnableIfNotLValueReference<T>>
constexpr Optional<typename std::decay<T>::type> make_optional(T&& v)
{
    return Optional<typename std::decay<T>::type>(std::forward<T>(v));
}

template <class T, typename = EnableIfLValueReference<T>>
constexpr Optional<typename std::decay<T>::type> make_optional(typename std::remove_reference<T>::type&& v)
{
    return Optional<typename std::decay<T>::type>(std::forward<typename std::remove_reference<T>::type>(v));
}

/// hash support
template <class T>
struct hash;

template <class T>
struct hash<Optional<T>>
{
    std::size_t operator()(const Optional<T>& o) const
    {
        if (bool(o))
            return std::hash<T>()(*o);
        return 0;
    }
};

}  // namespace core
}  // namespace ara

#endif  // ARA_CORE_OPTIONAL_H_
