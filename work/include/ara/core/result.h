// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef ARA_CORE_RESULT_H_
#define ARA_CORE_RESULT_H_

#include <iostream>
#include <memory>
#include <system_error>
#include <type_traits>
#include <utility>

#include "ara/core/error_code.h"
#include "ara/core/optional.h"
#include "ara/core/variant.h"

namespace ara
{
namespace core
{

/// @brief This class is a type that contains either a value or an error
/// @uptrace{SWS_CORE_00701}
template <typename T, typename E = ErrorCode>
class Result final
{
  private:
    ara::core::Variant<T, E> mData;

    template <typename U, typename... Args>
    struct has_as_first_checker;

    template <typename U>
    struct has_as_first_checker<U> : std::false_type
    {};

    template <typename U, typename... Args>
    struct has_as_first_checker<U, U, Args...> : std::true_type
    {};

    template <typename U, typename A, typename... Args>
    struct has_as_first_checker<U, A, Args...> : std::false_type
    {};

    template <typename U, typename... Args>
    struct has_as_first : public has_as_first_checker<typename std::remove_reference<U>::type,
                                                      typename std::remove_reference<Args>::type...>
    {};

  public:
    /// @brief Type alias for the type T of values.
    /// @uptrace{SWS_CORE_00711}
    using value_type = T;
    /// @brief Type alias for the type E of errors.
    /// @uptrace{SWS_CORE_00712}
    using error_type = E;

    /// @brief Build a new Result from the specified value(given as lvalue).
    /// @uptrace{SWS_CORE_00731}
    static Result FromValue(const T& t)
    {
        return Result(t);
    }

    /// @brief Build a new Result from the specified value (given as rvalue).
    /// @uptrace{SWS_CORE_00732}
    static Result FromValue(T&& t)
    {
        return Result(std::move(t));
    }

    /// @brief Build a new Result from a value that is constructed in-place from the given arguments.
    /// @uptrace{SWS_CORE_00733}
    template <typename... Args,
              typename = typename std::enable_if<true                                           //
                                                 && std::is_constructible<T, Args&&...>::value  //
                                                 && !has_as_first<T, Args...>::value            //
                                                 && !has_as_first<Result, Args...>::value       //
                                                 >::type                                        //
              >
    static Result FromValue(Args&&... args)
    {
        return Result(T{std::forward<Args>(args)...});
    }

    /// @brief Build a new Result from the specified error(given as lvalue).
    /// @uptrace{SWS_CORE_00734}
    static Result FromError(const E& e)
    {
        return Result(e);
    }

    /// @brief Build a new Result from the specified error(given as rvalue).
    /// @uptrace{SWS_CORE_00735}
    static Result FromError(E&& e)
    {
        return Result(std::move(e));
    }

    /// @brief Build a new Result from an error that is constructed in-place from the given arguments.
    /// @uptrace{SWS_CORE_00736}
    template <typename... Args,
              typename = typename std::enable_if<true                                           //
                                                 && std::is_constructible<E, Args&&...>::value  //
                                                 && !has_as_first<E, Args...>::value            //
                                                 && !has_as_first<Result, Args...>::value       //
                                                 >::type                                        //
              >
    static Result FromError(Args&&... args)
    {
        return Result(E{std::forward<Args>(args)...});
    }

    /// @brief Construct a new Result from the specified value(given as lvalue).
    /// @uptrace{SWS_CORE_00721}
    Result(const T& t) : mData{t}
    {}

    /// @brief Construct a new Result from the specified value(given as rvalue).
    /// @uptrace{SWS_CORE_00722}
    Result(T&& t) : mData{std::move(t)}
    {}

    /// @brief Construct a new Result from the specified error (given as lvalue).
    /// @uptrace{SWS_CORE_00723}
    explicit Result(const E& e) : mData{e}
    {}

    /// @brief Construct a new Result from the specified error (given as rvalue).
    /// @uptrace{SWS_CORE_00724}
    explicit Result(E&& e) : mData{std::move(e)}
    {}

    /// @brief Copy-construct a new Result from another instance.
    /// @uptrace{SWS_CORE_00725}
    Result(const Result& other) : mData{other.mData}
    {}

    /// @brief Move-construct a new Result from another instance.
    /// @uptrace{SWS_CORE_00726}
    Result(Result&& other) noexcept(
        std::is_nothrow_move_constructible<T>::value&& std::is_nothrow_move_constructible<E>::value)
        : mData{std::move(other.mData)}
    {}

    /// @brief Destructor for Result
    /// @uptrace{SWS_CORE_00727}
    ~Result() = default;

    /// @brief Copy-assign another Result to this instance.
    /// @uptrace{SWS_CORE_00741}
    Result& operator=(const Result& other)
    {
        mData = other.mData;
        return *this;
    }

    /// @brief Move-assign another Result to this instance.
    /// @uptrace{SWS_CORE_00742}
    Result& operator=(Result&& other) noexcept(
        std::is_nothrow_move_constructible<T>::value&& std::is_nothrow_move_assignable<T>::value&&
            std::is_nothrow_move_constructible<E>::value&& std::is_nothrow_move_assignable<E>::value)
    {
        mData = std::move(other.mData);
        return *this;
    }

    /// @brief Put a new value into this instance, constructed in-place from the given arguments.
    /// @uptrace{SWS_CORE_00743}
    template <typename... Args>
    void EmplaceValue(Args&&... args)
    {
        mData.template emplace<T>(args...);
    }

    /// @brief Put a new error into this instance, constructed in-place from the given arguments.
    /// @uptrace{SWS_CORE_00744}
    template <typename... Args>
    void EmplaceError(Args&&... args)
    {
        mData.template emplace<E>(args...);
    }

    /// @brief Exchange the contents of this instance with those of other.
    /// @uptrace{SWS_CORE_00745}
    void Swap(Result& other) noexcept(
        std::is_nothrow_move_constructible<T>::value&& std::is_nothrow_move_assignable<T>::value&&
            std::is_nothrow_move_constructible<E>::value&& std::is_nothrow_move_assignable<E>::value)
    {
        using std::swap;
        swap(mData, other.mData);
    }

    /// @brief Check whether *this contains a value.
    /// @uptrace{SWS_CORE_00751}
    bool HasValue() const noexcept
    {
        return mData.index() == 0;
    }

    /// @brief Check whether *this contains a value.
    /// @uptrace{SWS_CORE_00752}
    explicit operator bool() const noexcept
    {
        return HasValue();
    }

    /// @brief Access the contained value.
    /// @uptrace{SWS_CORE_00753}
    const T& operator*() const&
    {
        return Value();
    }

    /// @brief Access the contained value.
    /// @uptrace{SWS_CORE_00759}
    T&& operator*() &&
    {
        return std::move(*this).Value();
    }

    /// @brief Access the contained value.
    /// @uptrace{SWS_CORE_00754}
    const T* operator->() const
    {
        return std::addressof(Value());
    }

    /// @brief Access the contained value.
    /// @uptrace{SWS_CORE_00755}
    const T& Value() const&
    {
        const T* ptr = mData.template get<T>();
        if (ptr)
            return *ptr;
        std::cout << "__ value() called but NOT a value!\n";
        std::terminate();
    }

    /// @brief Access the contained value.
    /// @uptrace{SWS_CORE_00756}
    T&& Value() &&
    {
        T* ptr = mData.template get<T>();
        if (ptr)
            return std::move(*ptr);
        std::cout << "__ value() called but NOT a value!\n";
        std::terminate();
    }

    /// @brief Access the contained error.
    /// @uptrace{SWS_CORE_00757}
    const E& Error() const&
    {
        const E* ptr = mData.template get<E>();
        if (ptr)
            return *ptr;
        std::cout << "__ error() called but NOT an error!\n";
        std::terminate();
    }

    /// @brief Access the contained error.
    /// @uptrace{SWS_CORE_00758}
    E&& Error() &&
    {
        E* ptr = mData.template get<E>();
        if (ptr)
            return std::move(*ptr);
        std::cout << "__ error() called but NOT an error!\n";
        std::terminate();
    }

    /// @brief Return the contained value as an Optional.
    /// @uptrace{SWS_CORE_00770}
    Optional<T> Ok() const& noexcept
    {
        return HasValue() ? Optional<T>(Value()) : nullopt;
    }

    /// @brief Return the contained value as an Optional.
    /// @uptrace{SWS_CORE_00771}
    Optional<T> Ok() && noexcept
    {
        return HasValue() ? Optional<T>(std::move(Value())) : nullopt;
    }

    /// @brief Return the contained error as an Optional.
    /// @uptrace{SWS_CORE_00772}
    Optional<E> Err() const& noexcept
    {
        return HasValue() ? nullopt : Optional<E>(Error());
    }

    /// @brief Return the contained error as an Optional.
    /// @uptrace{SWS_CORE_00773}
    Optional<E> Err() && noexcept
    {
        return HasValue() ? nullopt : Optional<E>(std::move(Error()));
    }

    /// @brief Return the contained value or the given default value.
    /// @uptrace{SWS_CORE_00761}
    template <typename U>
    T ValueOr(U&& defaultValue) const&
    {
        return HasValue() ? Value() : static_cast<T>(std::forward<U>(defaultValue));
    }

    /// @brief Return the contained value or the given default value.
    /// @uptrace{SWS_CORE_00762}
    template <typename U>
    T ValueOr(U&& defaultValue) &&
    {
        return HasValue() ? std::move(Value()) : static_cast<T>(std::forward<U>(defaultValue));
    }

    /// @brief Return the contained error or the given default error.
    /// @uptrace{SWS_CORE_00763}
    template <typename G>
    E ErrorOr(G&& defaultError) const&
    {
        return HasValue() ? static_cast<E>(std::forward<G>(defaultError)) : Error();
    }

    /// @brief Return the contained error or the given default error.
    /// @uptrace{SWS_CORE_00764}
    template <typename G>
    E ErrorOr(G&& defaultError) &&
    {
        return HasValue() ? static_cast<E>(std::forward<G>(defaultError)) : std::move(Error());
    }

    /// @brief Return whether this instance contains the given error.
    /// @uptrace{SWS_CORE_00765}
    template <typename G>
    bool CheckError(G&& error) const
    {
        return HasValue() ? false : (Error() == static_cast<E>(std::forward<G>(error)));
    }

#ifndef ARA_NO_EXCEPTIONS
    /// @brief Return the contained value or throw an exception.
    /// @uptrace{SWS_CORE_00766}
    const T& ValueOrThrow() const& noexcept(false)
    {
        const T* ptr = mData.template get<T>();
        if (ptr)
            return *ptr;

        const E* e = mData.template get<E>();
        e->ThrowAsException();
    }

    /// @brief Return the contained value or throw an exception.
    /// @uptrace{SWS_CORE_00769}
    T&& ValueOrThrow() && noexcept(false)
    {
        T* ptr = mData.template get<T>();
        if (ptr)
            return std::move(*ptr);

        E* e = mData.template get<E>();
        e->ThrowAsException();
    }
#endif

    /// @brief Return the contained value or return the result of a function call.
    /// @uptrace{SWS_CORE_00767}
    template <typename F>
    T Resolve(F&& f) const
    {
        return HasValue() ? Value() : std::forward<F>(f)(Error());
    }

  private:
    template <bool Condition, typename U = void>
    using enable_if_t = typename std::enable_if<Condition, U>::type;

    template <typename U>
    using result_of_t = typename std::result_of<U>::type;

    /// @brief Trait that detects whether a type is a Result<...>
    template <typename U>
    struct is_result : std::false_type
    {};

    template <typename U, typename G>
    struct is_result<Result<U, G>> : std::true_type
    {};

    template <typename F>
    using CallableReturnsResult = enable_if_t<is_result<result_of_t<F(const T&)>>::value>;

    template <typename F>
    using CallableReturnsNoResult = enable_if_t<!is_result<result_of_t<F(const T&)>>::value>;

  public:
    /// @brief Apply the given Callable to the value of this instance, and return a new Result with the result of the
    /// call.
    /// @uptrace{SWS_CORE_00768}
    template <typename F, typename = CallableReturnsResult<F>>
    auto Bind(F&& f) const -> decltype(f(Value()))
    {
        using U = decltype(f(Value()));
        return HasValue() ? std::forward<F>(f)(Value()) : U(Error());
    }

    /// @brief Apply the given Callable to the value of this instance, and return a new Result with the result of the
    /// call.
    /// @uptrace{SWS_CORE_00768}
    template <typename F, typename = CallableReturnsNoResult<F>>
    auto Bind(F&& f) const -> Result<decltype(f(Value())), E>
    {
        using U = decltype(f(Value()));
        using R = Result<U, E>;
        return HasValue() ? std::forward<F>(f)(Value()) : R(Error());
    }

    template <typename, typename>
    friend class Result;
};

/// @brief Compare two Result instances for equality.
/// @uptrace{SWS_CORE_00780}
template <typename T, typename E>
bool operator==(const Result<T, E>& lhs, const Result<T, E>& rhs)
{
    if (bool(lhs) != bool(rhs))
        return false;
    if (lhs)
        return lhs.Value() == rhs.Value();
    return lhs.Error() == rhs.Error();
}

/// @brief Compare two Result instances for inequality.
/// @uptrace{SWS_CORE_00781}
template <typename T, typename E>
bool operator!=(const Result<T, E>& lhs, const Result<T, E>& rhs)
{
    if (bool(lhs) != bool(rhs))
        return true;
    if (lhs)
        return lhs.Value() != rhs.Value();
    return lhs.Error() != rhs.Error();
}

/// @brief Compare a Result instance for equality to a value.
/// @uptrace{SWS_CORE_00782}
template <typename T, typename E>
bool operator==(const Result<T, E>& lhs, const T& value)
{
    return lhs ? *lhs == value : false;
}

/// @brief Compare a Result instance for equality to a value.
/// @uptrace{SWS_CORE_00783}
template <typename T, typename E>
bool operator==(const T& value, const Result<T, E>& rhs)
{
    return rhs ? value == *rhs : false;
}

/// @brief Compare a Result instance for inequality to a value.
/// @uptrace{SWS_CORE_00784}
template <typename T, typename E>
bool operator!=(const Result<T, E>& lhs, const T& value)
{
    return lhs ? *lhs != value : true;
}

/// @brief Compare a Result instance for inequality to a value.
/// @uptrace{SWS_CORE_00785}
template <typename T, typename E>
bool operator!=(const T& value, const Result<T, E>& rhs)
{
    return rhs ? value != *rhs : true;
}

/// @brief Compare a Result instance for equality to an error.
/// @uptrace{SWS_CORE_00786}
template <typename T, typename E>
bool operator==(const Result<T, E>& lhs, const E& error)
{
    return lhs ? false : lhs.Error() == error;
}

/// @brief Compare a Result instance for equality to an error.
/// @uptrace{SWS_CORE_00787}
template <typename T, typename E>
bool operator==(const E& error, const Result<T, E>& rhs)
{
    return rhs ? false : error == rhs.Error();
}

/// @brief Compare a Result instance for inequality to an error.
/// @uptrace{SWS_CORE_00788}
template <typename T, typename E>
bool operator!=(const Result<T, E>& lhs, const E& error)
{
    return lhs ? true : lhs.Error() != error;
}

/// @brief Compare a Result instance for inequality to an error.
/// @uptrace{SWS_CORE_00789}
template <typename T, typename E>
bool operator!=(const E& error, const Result<T, E>& rhs)
{
    return rhs ? true : error != rhs.Error();
}

/// @brief Swap the contents of the two given arguments.
/// @uptrace{SWS_CORE_00796}
template <typename T, typename E>
inline void swap(Result<T, E>& lhs, Result<T, E>& rhs) noexcept(noexcept(lhs.Swap(rhs)))
{
    lhs.Swap(rhs);
}

/// @brief Specialization of class Result for "void" values.
/// @uptrace{SWS_CORE_00801}
template <typename E>
class Result<void, E> final
{
  private:
    // Cannot use 'void' for variant directly,
    // so define our own(default-constructible) dummy for value type instead.
    struct value
    {};

    using T = value;

    ara::core::Variant<T, E> mData;

  public:
    /// @brief Type alias for the type T of values, always "void" for this specialization.
    /// @uptrace{SWS_CORE_00811}
    using value_type = void;
    /// @brief Type alias for the type E of errors.
    /// @uptrace{SWS_CORE_00812}
    using error_type = E;

    /// @brief Build a new Result with "void" as value.
    /// @uptrace{SWS_CORE_00831}
    static Result FromValue()
    {
        return Result();
    }

    /// @brief Build a new Result from the specified error(given as lvalue).
    /// @uptrace{SWS_CORE_00834}
    static Result FromError(const E& e)
    {
        return Result(e);
    }

    /// @brief Build a new Result from the specified error(given as rvalue).
    /// @uptrace{SWS_CORE_00835}
    static Result FromError(E&& e)
    {
        return Result(std::move(e));
    }

    /// @brief Build a new Result from an error that is constructed in-place from the given arguments.
    /// @uptrace{SWS_CORE_00836}
    template <typename... Args>
    static Result FromError(Args&&... args)
    {
        return Result(E{std::forward<Args>(args)...});
    }

    /// @brief Construct a new Result with a "void" value.
    /// @uptrace{SWS_CORE_00821}
    Result() noexcept : mData(T{})
    {}

    /// @brief Construct a new Result from the specified error(given as lvalue).
    /// @uptrace{SWS_CORE_00823}
    explicit Result(const E& e) : mData(e)
    {}

    /// @brief Construct a new Result from the specified error(given as rvalue).
    /// @uptrace{SWS_CORE_00824}
    explicit Result(E&& e) : mData(std::move(e))
    {}

    /// @brief Copy-construct a new Result from another instance.
    /// @uptrace{SWS_CORE_00825}
    Result(const Result& other) : mData{other.mData}
    {}

    /// @brief Move-construct a new Result from another instance.
    /// @uptrace{SWS_CORE_00826}
    Result(Result&& other) noexcept(std::is_nothrow_move_constructible<E>::value) : mData{std::move(other.mData)}
    {}

    /// @brief Destructor for Result.
    /// @uptrace{SWS_CORE_00827}
    ~Result()
    {}

    /// @brief Copy-assign another Result to this instance.
    /// @uptrace{SWS_CORE_00841}
    Result& operator=(const Result& other)
    {
        mData = other.mData;
        return *this;
    }

    /// @brief Move-assign another Result to this instance.
    /// @uptrace{SWS_CORE_00842}
    Result& operator=(Result&& other) noexcept(
        std::is_nothrow_move_constructible<E>::value&& std::is_nothrow_move_assignable<E>::value)
    {
        mData = std::move(other.mData);
        return *this;
    }

    /// @brief Put a new value into this instance, constructed in-place from the given arguments.
    /// @uptrace{SWS_CORE_00843}
    template <typename... Args>
    void EmplaceValue(Args&&... args) noexcept
    {
        mData = T(args...);
    }

    /// @brief Put a new error into this instance, constructed in-place from the given arguments.
    /// @uptrace{SWS_CORE_00844}
    template <typename... Args>
    void EmplaceError(Args&&... args)
    {
        mData.template emplace<E>(args...);
    }

    /// @brief Exchange the contents of this instance with those of other.
    /// @uptrace{SWS_CORE_00845}
    void Swap(Result& other) noexcept(
        std::is_nothrow_move_constructible<E>::value&& std::is_nothrow_move_assignable<E>::value)
    {
        using std::swap;
        swap(mData, other.mData);
    }

    /// @brief Check whether *this contains a value.
    /// @uptrace{SWS_CORE_00851}
    bool HasValue() const noexcept
    {
        return mData.index() == 0;
    }

    /// @brief Check whether *this contains a value.
    /// @uptrace{SWS_CORE_00852}
    explicit operator bool() const noexcept
    {
        return HasValue();
    }

    /// @brief Do nothing. This function only exists for helping with generic programming.
    /// @uptrace{SWS_CORE_00853}
    void operator*() const
    {
        return Value();
    }

    /// @brief Do nothing. This function only exists for helping with generic programming.
    /// @uptrace{SWS_CORE_00855}
    void Value() const
    {
        const T* ptr = mData.template get<T>();
        if (ptr)
            return;
        std::cout << "__ value() called but NOT a (void) value!\n";
        std::terminate();
    }

    /// @brief Access the contained error.
    /// @uptrace{SWS_CORE_00857}
    const E& Error() const&
    {
        const E* ptr = mData.template get<E>();
        if (ptr)
            return *ptr;
        std::cout << "__ error() called but NOT an error!\n";
        std::terminate();
    }

    /// @brief Access the contained error.
    /// @uptrace{SWS_CORE_00858}
    E&& Error() &&
    {
        E* ptr = mData.template get<E>();
        if (ptr)
            return std::move(*ptr);
        std::cout << "__ error() called but NOT an error!\n";
        std::terminate();
    }

    /// @brief Return the contained error as an Optional.
    /// @uptrace{SWS_CORE_00868}
    Optional<E> Err() const& noexcept
    {
        return HasValue() ? nullopt : Optional<E>(Error());
    }

    /// @brief Return the contained error as an Optional.
    /// @uptrace{SWS_CORE_00869}
    Optional<E> Err() && noexcept
    {
        return HasValue() ? nullopt : Optional<E>(std::move(Error()));
    }

    /// @brief Do nothing. This function only exists for helping with generic programming.
    /// @uptrace{SWS_CORE_00861}
    template <typename U>
    void ValueOr(U&& defaultValue) const
    {
        return HasValue() ? Value() : static_cast<void>(std::forward<U>(defaultValue));
    }

    /// @brief Return the contained error or the given default error.
    /// @uptrace{SWS_CORE_00863}
    template <typename G>
    E ErrorOr(G&& defaultError) const&
    {
        return HasValue() ? static_cast<E>(std::forward<G>(defaultError)) : Error();
    }

    /// @brief Return the contained error or the given default error.
    /// @uptrace{SWS_CORE_00864}
    template <typename G>
    E ErrorOr(G&& defaultError) &&
    {
        return HasValue() ? static_cast<E>(std::forward<G>(defaultError)) : std::move(Error());
    }

    /// @brief Return whether this instance contains the given error.
    /// @uptrace{SWS_CORE_00865}
    template <typename G>
    bool CheckError(G&& error) const
    {
        return HasValue() ? false : (Error() == static_cast<E>(std::forward<G>(error)));
    }

#ifndef ARA_NO_EXCEPTIONS
    /// @brief Return the contained value or throw an exception.
    /// @uptrace{SWS_CORE_00866}
    void ValueOrThrow() const noexcept(false)
    {
        const T* ptr = mData.template get<T>();
        if (ptr)
            return;

        const E* e = mData.template get<E>();
        e->ThrowAsException();
    }
#endif

    /// @brief Do nothing or call a function.
    /// @uptrace{SWS_CORE_00867}
    template <typename F>
    void Resolve(F&& f) const
    {
        return HasValue() ? Value() : std::forward<F>(f)(Error());
    }

  private:
    template <bool Condition, typename U = void>
    using enable_if_t = typename std::enable_if<Condition, U>::type;

    template <typename U>
    using result_of_t = typename std::result_of<U>::type;

    /// @brief Trait that detects whether a type is a Result<...>
    template <typename U>
    struct is_result : std::false_type
    {};

    template <typename U, typename G>
    struct is_result<Result<U, G>> : std::true_type
    {};

    template <typename F>
    using CallableReturnsResult = enable_if_t<is_result<result_of_t<F()>>::value>;

    template <typename F>
    using CallableReturnsNoResult = enable_if_t<!is_result<result_of_t<F()>>::value>;

  public:
    /// @brief Call the given Callable, and return a new Result with the result of the call.
    /// @uptrace{SWS_CORE_00870}
    template <typename F, typename = CallableReturnsResult<F>>
    auto Bind(F&& f) const -> decltype(f())
    {
        using U = decltype(f());
        return HasValue() ? std::forward<F>(f)() : U(Error());
    }

    /// @brief Call the given Callable, and return a new Result with the result of the call.
    /// @uptrace{SWS_CORE_00870}
    template <typename F, typename = CallableReturnsNoResult<F>>
    auto Bind(F&& f) const -> Result<decltype(f()), E>
    {
        using U = decltype(f());
        using R = Result<U, E>;
        return HasValue() ? std::forward<F>(f)() : R(Error());
    }

    template <typename, typename>
    friend class Result;
};

}  // namespace core
}  // namespace ara

#endif  // ARA_CORE_RESULT_H_
