// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef PARA_SERIALIZATION_TYPES_OPTIONAL_H_
#define PARA_SERIALIZATION_TYPES_OPTIONAL_H_

#include <initializer_list>
#include <type_traits>
#include <utility>

#include "para/serialization/traits/is_comparable.h"
#include "para/serialization/traits/is_template_base_of.h"

namespace para
{
// Type tag to disambiguate in-place constructors.
struct InPlace
{};

template <typename T>
class Optional
{
    // Ensure that T is not InPlace.
    static_assert(!std::is_same<std::decay_t<T>, InPlace>::value, "T cannot be InPlace!");

    // Befriend other Optional types.
    template <typename U>
    friend class Optional;

  public:
    // Default constructs an empty Optional type.
    constexpr Optional() noexcept : mState{}
    {}

    // Constructs a non-empty Optional from lvalue and rvalue type T.
    constexpr Optional(const T& value) : mState{value}
    {}

    constexpr Optional(T&& value) : mState{std::move(value)}
    {}

    // Copy and move constructors.
    constexpr Optional(const Optional& other) : mState{other.mState}
    {}

    constexpr Optional(Optional&& other) noexcept(std::is_nothrow_move_constructible<T>::value)
        : mState{std::move(other.mState)}
    {}

    // Constructs a non-empty Optional from a type U such that T{U()} is valid.
    template <typename U, typename Enabled = std::enable_if_t<std::is_constructible<T, U>::value>>
    constexpr Optional(U&& value) : mState{InPlace{}, std::forward<U>(value)}
    {}

    // In-place constructor with arbitrary argument forwarding.
    template <typename... Args>
    explicit constexpr Optional(InPlace, Args&&... args) : mState{InPlace{}, std::forward<Args>(args)...}
    {}

    // In-place constructor with initializer list and arbitrary argument
    // forwarding.
    template <typename U, typename... Args,
              typename Enabled = std::enable_if_t<std::is_constructible<T, std::initializer_list<U>>::value>>
    constexpr Optional(InPlace, std::initializer_list<U> il, Args&&... args)
        : mState{InPlace{}, il, std::forward<Args>(args)...}
    {}

    // Constructs a non-empty Optional from a compatible initializer list.
    template <typename U,
              typename Enabled = std::enable_if_t<std::is_constructible<T, std::initializer_list<U>>::value>>
    constexpr Optional(std::initializer_list<U> il) : mState{InPlace{}, il}
    {}

    // Default destructs the Optional. State and Storage nested types below handle destruction for trivial and
    // non-trivial types.
    ~Optional() = default;

    // Copy assignment operator.
    constexpr Optional& operator=(const Optional& other)
    {
        if (this != &other) {
            if (!other.empty()) {
                Assign(other.mState.storage.value);
            } else {
                Destruct();
            }
        }
        return *this;
    }

    // Move assignment operator.
    constexpr Optional& operator=(Optional&& other) noexcept(std::is_nothrow_move_assignable<T>::value &&
                                                             std::is_nothrow_move_constructible<T>::value)
    {
        if (this != &other) {
            if (!other.empty()) {
                Assign(other.take());
                other.Destruct();
            } else {
                Destruct();
            }
        }
        return *this;
    }

    // Copy assignment from a different Optional type.
    template <typename U>
    constexpr std::enable_if_t<std::is_constructible<T, const U&>::value, Optional&> operator=(const Optional<U>& other)
    {
        if (!other.empty()) {
            Assign(other.get());
        } else {
            Destruct();
        }
        return *this;
    }

    // Move assignment from a different Optional type.
    template <typename U>
    constexpr std::enable_if_t<std::is_constructible<T, U&&>::value, Optional&> operator=(Optional<U>&& other)
    {
        if (!other.empty()) {
            Assign(other.take());
            other.Destruct();
        } else {
            Destruct();
        }
        return *this;
    }

    // Copy/move assignment from type U.
    template <typename U>
    std::enable_if_t<std::is_constructible<T, U>::value, Optional&> operator=(U&& value)
    {
        Assign(std::forward<U>(value));
        return *this;
    }

    // Returns true if the Optional is empty.
    constexpr bool empty() const noexcept
    {
        return mState.empty;
    }

    // Returns true if the Optional is non-empty.
    explicit constexpr operator bool() const noexcept
    {
        return !empty();
    }

    // Returns the underlying value. These accessors may only be called when
    // non-empty.
    constexpr const T& get() const
    {
        return mState.storage.value;
    }

    constexpr T& get()
    {
        return mState.storage.value;
    }

    constexpr T&& take()
    {
        return std::move(mState.storage.value);
    }

    // Clears the optional to the empty state, destroying the underlying value if necessary.
    void clear()
    {
        Destruct();
    }

  private:
    // Handles assignment/construction for assignment operators.
    template <typename U>
    constexpr void Assign(U&& value)
    {
        if (empty()) {
            ::new (&mState.storage.value) T(std::forward<U>(value));
            mState.empty = false;
        } else {
            mState.storage.value = std::forward<U>(value);
        }
    }

    // Destroys the stored value if non-empty.
    constexpr void Destruct() noexcept
    {
        if (!empty()) {
            mState.storage.value.T::~T();
            mState.empty = true;
        }
    }

    // Type tag used by the nested State type to explicitly select the trivial constructor in the nested Storage type.
    struct TrivialInit
    {};

    // Base union type used to implement lazy-initialized storage.
    // This type is specialized to handle trivially and non-trivially destructible types.
    template <typename U, typename Enabled = void>
    union Storage;

    // Specialization that handles non-trivially destructible types.
    template <typename U>
    union Storage<U, std::enable_if_t<!std::is_trivially_destructible<U>::value>>
    {
        // Dummy type that is active when the value type is inactive.
        // Since this type is trivially-destructible no special care is necessary when activating the value type.
        unsigned char dummy;

        // Value type that is active when the dummy type is inactive.
        // This type must be properly destroyed before deactivation.
        U value;

        // Trivial constructor that makes the dummy type active.
        constexpr Storage(TrivialInit) noexcept : dummy{}
        {}

        // Constructs the value type, making it active.
        template <typename... Args>
        constexpr Storage(Args&&... args) noexcept : value(std::forward<Args>(args)...)
        {}

        // Non-trivial destructor.
        // This doesn't do anything useful except enable to compiler to catch attempts to use this type in a non-trivial
        // context.
        ~Storage()
        {}
    };

    // Specialization that handles trivially-destructible types.
    template <typename U>
    union Storage<U, std::enable_if_t<std::is_trivially_destructible<U>::value>>
    {
        // Dummy type that is active when the value type is inactive.
        // Since this type is trivially-destructible no special care is necessary when activating the value type.
        unsigned char dummy;

        // Value type that is active when the dummy type is inactive.
        // Since this type is also trivially-destructible no special care is required at deactivation.
        U value;

        // Trivial constructor that makes the dummy type active.
        constexpr Storage(TrivialInit) noexcept : dummy{}
        {}

        // Constructs the value type, making it active.
        template <typename... Args>
        constexpr Storage(Args&&... args) noexcept : value(std::forward<Args>(args)...)
        {}

        // Trivial destructor.
        ~Storage() = default;
    };

    // Base type to store a lazy-initialized value and track whether it is initialized.
    // This type is specialized to handle trivailly and non-trivially destructible value types.
    template <typename U, typename Enabled = void>
    struct State;

    // Specialization that handles non-trivially destructible types.
    template <typename U>
    struct State<U, std::enable_if_t<!std::is_trivially_destructible<U>::value>>
    {
        // Default constructor that sets the state to empty and trivially initializes the storage for the value type.
        constexpr State() noexcept : empty{true}, storage{TrivialInit{}}
        {}

        // Copy constructor.
        // Copy constructs the value type if the other state is non-empty.
        constexpr State(const State& other) : empty{other.empty}, storage{TrivialInit{}}
        {
            if (!other.empty)
                ::new (&storage.value) U(other.storage.value);
        }

        // Move constructor.
        // Move constructs the value type if the other state is non-empty.
        constexpr State(State&& other) : empty{other.empty}, storage{TrivialInit{}}
        {
            if (!other.empty)
                ::new (&storage.value) U(std::move(other.storage.value));
        }

        // Value constructors.
        // Sets the state to non-empty and copies or moves the value to storage.
        explicit constexpr State(const U& value) : empty{false}, storage{value}
        {}

        explicit constexpr State(U&& value) : empty{false}, storage{std::move(value)}
        {}

        // In-place constructor for more complex initialization.
        template <typename... Args>
        explicit State(InPlace, Args&&... args) : empty{false}, storage{std::forward<Args>(args)...}
        {}

        // In-place initializer list constructor.
        template <typename V, typename... Args,
                  typename = std::enable_if_t<std::is_constructible<U, std::initializer_list<V>>::value>>
        explicit State(InPlace, std::initializer_list<V> il, Args&&... args)
            : empty{false}, storage{il, std::forward<Args>(args)...}
        {}

        // Non-trivial destructor. Destroys the value in storage if non-empty.
        ~State()
        {
            if (!empty)
                storage.value.U::~U();
        }

        // Tracks whether the storage value is empty (un-initialized) or non-empty (initialized).
        bool empty;

        // Storage for the lazy-initialized value.
        Storage<U> storage;
    };

    // Specialization that handles trivially-destructible types.
    // This specialization conforms with the requirements for constexpr expressions.
    template <typename U>
    struct State<U, std::enable_if_t<std::is_trivially_destructible<U>::value>>
    {
        // Default constructor that sets the state to empty and trivially initializes the storage for the value type.
        constexpr State() noexcept : empty{true}, storage{TrivialInit{}}
        {}

        // Copy constructor.
        // Copy constructs the value type if the other state is non-empty.
        constexpr State(const State& other) : empty{other.empty}, storage{other.storage.value}
        {}

        // Move constructor.
        // Move constructs the value type if the other state is non-empty.
        constexpr State(State&& other) : empty{other.empty}, storage{std::move(other.storage.value)}
        {}

        // Value constructors.
        // Sets the state to non-empty and copies or moves the value to storage.
        explicit constexpr State(const U& value) : empty{false}, storage{value}
        {}

        explicit constexpr State(U&& value) : empty{false}, storage{std::move(value)}
        {}

        // In-place constructor for more complex initialization.
        template <typename... Args>
        explicit constexpr State(InPlace, Args&&... args) : empty{false}, storage{std::forward<Args>(args)...}
        {}

        // In-place initializer list constructor.
        template <typename V, typename... Args,
                  typename = std::enable_if_t<std::is_constructible<U, std::initializer_list<V>>::value>>
        explicit constexpr State(InPlace, std::initializer_list<V> il, Args&&... args)
            : empty{false}, storage{il, std::forward<Args>(args)...}
        {}

        // Trivial destructor.
        ~State() = default;

        // Tracks whether the storage value is empty (un-initialized) or non-empty (initialized).
        bool empty;

        // Storage for the lazy-initialized value.
        Storage<U> storage;
    };

    // Tracks the value and empty/non-empty state.
    State<T> mState;
};

// Relational operators.
template <typename T, typename U, typename Enabled = para::serialization::EnableIfComparableEqual<T, U>>
constexpr bool operator==(const Optional<T>& a, const Optional<U>& b)
{
    if (a.empty() != b.empty())
        return false;
    else if (a.empty())
        return true;
    else
        return a.get() == b.get();
}

template <typename T, typename U, typename Enabled = para::serialization::EnableIfComparableEqual<T, U>>
constexpr bool operator!=(const Optional<T>& a, const Optional<U>& b)
{
    return !(a == b);
}

template <typename T, typename U, typename Enabled = para::serialization::EnableIfComparableLess<T, U>>
constexpr bool operator<(const Optional<T>& a, const Optional<U>& b)
{
    if (b.empty())
        return false;
    else if (a.empty())
        return true;
    else
        return a.get() < b.get();
}

template <typename T, typename U, typename Enabled = para::serialization::EnableIfComparableLess<T, U>>
constexpr bool operator>(const Optional<T>& a, const Optional<U>& b)
{
    return b < a;
}

template <typename T, typename U, typename Enabled = para::serialization::EnableIfComparableLess<T, U>>
constexpr bool operator<=(const Optional<T>& a, const Optional<U>& b)
{
    return !(b < a);
}

template <typename T, typename U, typename Enabled = para::serialization::EnableIfComparableLess<T, U>>
constexpr bool operator>=(const Optional<T>& a, const Optional<U>& b)
{
    return !(a < b);
}

template <typename T, typename U,
          typename Enabled = std::enable_if_t<para::serialization::IsComparableEqual<T, U>::value &&
                                              !para::serialization::IsTemplateBaseOf<Optional, U>::value>>
constexpr bool operator==(const Optional<T>& a, const U& b)
{
    return !a.empty() ? a.get() == b : false;
}

template <typename T, typename U,
          typename Enabled = std::enable_if_t<para::serialization::IsComparableEqual<T, U>::value &&
                                              !para::serialization::IsTemplateBaseOf<Optional, T>::value>>
constexpr bool operator==(const T& a, const Optional<U>& b)
{
    return !b.empty() ? a == b.get() : false;
}

template <typename T, typename U,
          typename Enabled = std::enable_if_t<para::serialization::IsComparableEqual<T, U>::value &&
                                              !para::serialization::IsTemplateBaseOf<Optional, U>::value>>
constexpr bool operator!=(const Optional<T>& a, const U& b)
{
    return !(a == b);
}

template <typename T, typename U,
          typename Enabled = std::enable_if_t<para::serialization::IsComparableEqual<T, U>::value &&
                                              !para::serialization::IsTemplateBaseOf<Optional, T>::value>>
constexpr bool operator!=(const T& a, const Optional<U>& b)
{
    return !(a == b);
}

template <typename T, typename U,
          typename Enabled = std::enable_if_t<para::serialization::IsComparableLess<T, U>::value &&
                                              !para::serialization::IsTemplateBaseOf<Optional, U>::value>>
constexpr bool operator<(const Optional<T>& a, const U& b)
{
    return !a.empty() ? a.get() < b : true;
}

template <typename T, typename U,
          typename Enabled = std::enable_if_t<para::serialization::IsComparableLess<T, U>::value &&
                                              !para::serialization::IsTemplateBaseOf<Optional, T>::value>>
constexpr bool operator<(const T& a, const Optional<U>& b)
{
    return !b.empty() ? a < b.get() : false;
}

template <typename T, typename U,
          typename Enabled = std::enable_if_t<para::serialization::IsComparableLess<T, U>::value &&
                                              !para::serialization::IsTemplateBaseOf<Optional, U>::value>>
constexpr bool operator>(const Optional<T>& a, const U& b)
{
    return !a.empty() ? b < a.get() : false;
}

template <typename T, typename U,
          typename Enabled = std::enable_if_t<para::serialization::IsComparableLess<T, U>::value &&
                                              !para::serialization::IsTemplateBaseOf<Optional, T>::value>>
constexpr bool operator>(const T& a, const Optional<U>& b)
{
    return !b.empty() ? b.get() < a : true;
}

template <typename T, typename U,
          typename Enabled = std::enable_if_t<para::serialization::IsComparableLess<T, U>::value &&
                                              !para::serialization::IsTemplateBaseOf<Optional, U>::value>>
constexpr bool operator<=(const Optional<T>& a, const U& b)
{
    return !a.empty() ? !(b < a.get()) : true;
}

template <typename T, typename U,
          typename Enabled = std::enable_if_t<para::serialization::IsComparableLess<T, U>::value &&
                                              !para::serialization::IsTemplateBaseOf<Optional, T>::value>>
constexpr bool operator<=(const T& a, const Optional<U>& b)
{
    return !b.empty() ? !(b.get() < a) : false;
}

template <typename T, typename U,
          typename Enabled = std::enable_if_t<para::serialization::IsComparableLess<T, U>::value &&
                                              !para::serialization::IsTemplateBaseOf<Optional, U>::value>>
constexpr bool operator>=(const Optional<T>& a, const U& b)
{
    return !a.empty() ? !(a.get() < b) : false;
}

template <typename T, typename U,
          typename Enabled = std::enable_if_t<para::serialization::IsComparableLess<T, U>::value &&
                                              !para::serialization::IsTemplateBaseOf<Optional, T>::value>>
constexpr bool operator>=(const T& a, const Optional<U>& b)
{
    return !b.empty() ? !(a < b.get()) : true;
}

}  // namespace para

#endif  // PARA_SERIALIZATION_TYPES_OPTIONAL_H_
