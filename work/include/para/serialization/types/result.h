// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef PARA_SERIALIZATION_TYPE_RESULT_H_
#define PARA_SERIALIZATION_TYPE_RESULT_H_

#include <type_traits>

namespace para
{
namespace serialization
{
// Result is a template type that contains either an error value of type ErrorEnum or a value of type T.
// Functions can use Result as a return type to unambiguously signal success with a value or failure with an error code.
template <typename ErrorEnum, typename T>
class Result
{
    static_assert(std::is_enum<ErrorEnum>::value, "ErrorEnum must be an enum class.");
    static_assert(!std::is_same<std::decay_t<ErrorEnum>, std::decay_t<T>>::value,
                  "ErrorEnum and T must not be the same type.");

  public:
    constexpr Result() : mError{ErrorEnum::None}, mState{State::kEmpty}
    {}

    constexpr Result(const T& value) : mValue{value}, mState{State::kValue}
    {}

    constexpr Result(T&& value) : mValue{std::move(value)}, mState{State::kValue}
    {}

    constexpr Result(const Result& other) : mError{ErrorEnum::None}, mState{State::kEmpty}
    {
        *this = other;
    }

    constexpr Result(Result&& other) : mError{ErrorEnum::None}, mState{State::kEmpty}
    {
        *this = std::move(other);
    }

    constexpr Result(ErrorEnum error) : mError{error}, mState{error == ErrorEnum::None ? State::kEmpty : State::kError}
    {}

    ~Result()
    {
        Destruct();
    }

    constexpr Result& operator=(const Result& other)
    {
        if (this != &other) {
            if (other.has_value())
                Assign(other.mValue);
            else
                Assign(other.mError);
        }
        return *this;
    }

    constexpr Result& operator=(Result&& other)
    {
        if (this != &other) {
            if (other.has_value())
                Assign(std::move(other.mValue));
            else
                Assign(other.mError);
            other.Destruct();
        }
        return *this;
    }

    constexpr Result& operator=(const T& value)
    {
        Assign(value);
        return *this;
    }

    constexpr Result& operator=(T&& value)
    {
        Assign(std::move(value));
        return *this;
    }

    constexpr Result& operator=(ErrorEnum error)
    {
        Assign(error);
        return *this;
    }

    constexpr bool has_value() const
    {
        return mState == State::kValue;
    }

    constexpr bool has_error() const
    {
        return mState == State::kError;
    }

    constexpr explicit operator bool() const
    {
        return has_value();
    }

    constexpr ErrorEnum error() const
    {
        if (has_error())
            return mError;
        else
            return ErrorEnum::None;
    }

    constexpr const T& get() const
    {
        return *value();
    }

    constexpr T& get()
    {
        return *value();
    }

    constexpr T&& take()
    {
        return std::move(*value());
    }

    constexpr void clear()
    {
        Destruct();
    }

  private:
    constexpr void Assign(const T& value)
    {
        if (has_value()) {
            mValue = value;
        } else {
            new (&mValue) T(value);
            mState = State::kValue;
        }
    }

    constexpr void Assign(T&& value)
    {
        if (has_value()) {
            mValue = std::move(value);
        } else {
            new (&mValue) T(std::move(value));
            mState = State::kValue;
        }
    }

    constexpr void Assign(ErrorEnum error)
    {
        Destruct();
        if (error != ErrorEnum::None) {
            mError = error;
            mState = State::kError;
        }
    }

    constexpr void Destruct()
    {
        if (has_value())
            mValue.~T();
        mError = ErrorEnum::None;
        mState = State::kEmpty;
    }

    constexpr const T* value() const
    {
        return has_value() ? &mValue : nullptr;
    }

    constexpr T* value()
    {
        return has_value() ? &mValue : nullptr;
    }
    enum class State
    {
        kEmpty,
        kError,
        kValue,
    };

    union
    {
        ErrorEnum mError;
        T mValue;
    };

    State mState;
};

// Specialization of Result for void types.
template <typename ErrorEnum>
class Result<ErrorEnum, void>
{
    static_assert(std::is_enum<ErrorEnum>::value, "ErrorEnum must be an enum class.");

  public:
    constexpr Result() : mError{ErrorEnum::None}
    {}

    constexpr Result(ErrorEnum error) : mError{error}
    {}

    constexpr Result(const Result& other) : mError{other.mError}
    {}

    constexpr Result(Result&& other) : mError{other.mError}
    {
        other.clear();
    }

    ~Result() = default;

    constexpr Result& operator=(const Result& other)
    {
        if (this != &other) {
            mError = other.mError;
        }
        return *this;
    }

    constexpr Result& operator=(Result&& other)
    {
        if (this != &other) {
            mError = other.mError;
            other.clear();
        }
        return *this;
    }

    constexpr bool has_error() const
    {
        return mError != ErrorEnum::None;
    }

    constexpr explicit operator bool() const
    {
        return !has_error();
    }

    constexpr ErrorEnum error() const
    {
        return mError;
    }

    constexpr void clear()
    {
        mError = ErrorEnum::None;
    }

  private:
    ErrorEnum mError;
};

}  // namespace serialization
}  // namespace para

#endif  // PARA_SERIALIZATION_TYPE_RESULT_H_
