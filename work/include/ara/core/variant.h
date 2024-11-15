// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef ARA_CORE_VARIANT_H_
#define ARA_CORE_VARIANT_H_

#include <cstdint>
#include <tuple>
#include <type_traits>

namespace ara
{
namespace core
{
// Type tag denoting an empty variant.
struct EmptyVariant
{};

namespace detail
{
// Type for matching tagged overloads.
template <typename T>
struct TypeTag
{};

// Determines the type of the I-th element of Types....
template <std::size_t I, typename... Types>
using TypeForIndex = std::tuple_element_t<I, std::tuple<Types...>>;

// Determines the type tag for the I-th element of Types....
template <std::size_t I, typename... Types>
using TypeTagForIndex = TypeTag<TypeForIndex<I, Types...>>;

// Similar to std::is_constructible but evaluates to false for pointer to boolean construction:
// avoiding this conversion helps prevent subtle bugs in Variants with bool elements.
template <typename...>
struct IsConstructible;

template <typename T, typename U>
struct IsConstructible<T, U>
    : std::integral_constant<bool, std::is_constructible<T, U>::value && !(std::is_same<std::decay_t<T>, bool>::value &&
                                                                           std::is_pointer<std::decay_t<U>>::value)>
{};

template <typename T, typename... Args>
struct IsConstructible<T, Args...> : std::is_constructible<T, Args...>
{};

// Enable if T(Args...) is well formed.
template <typename R, typename T, typename... Args>
using EnableIfConstructible = typename std::enable_if<IsConstructible<T, Args...>::value, R>::type;
// Enable if T(Args...) is not well formed.
template <typename R, typename T, typename... Args>
using EnableIfNotConstructible = typename std::enable_if<!IsConstructible<T, Args...>::value, R>::type;

// Determines whether T is an element of Types...;
template <typename... Types>
struct HasType : std::false_type
{};

template <typename T, typename U>
struct HasType<T, U> : std::is_same<std::decay_t<T>, std::decay_t<U>>
{};

template <typename T, typename First, typename... Rest>
struct HasType<T, First, Rest...> : std::integral_constant<bool, HasType<T, First>::value || HasType<T, Rest...>::value>
{};

// Defines set operations on a set of Types...
template <typename... Types>
struct Set
{
    // Default specialization catches the empty set, which is always a subset.
    template <typename...>
    struct IsSubset : std::true_type
    {};

    template <typename T>
    struct IsSubset<T> : HasType<T, Types...>
    {};

    template <typename First, typename... Rest>
    struct IsSubset<First, Rest...> : std::integral_constant<bool, IsSubset<First>::value && IsSubset<Rest...>::value>
    {};
};

// Determines the number of elements of Types... that are constructible from From.
template <typename... Types>
struct ConstructibleCount;

template <typename From, typename To>
struct ConstructibleCount<From, To> : std::integral_constant<std::size_t, IsConstructible<To, From>::value>
{};

template <typename From, typename First, typename... Rest>
struct ConstructibleCount<From, First, Rest...>
    : std::integral_constant<std::size_t,
                             IsConstructible<First, From>::value + ConstructibleCount<From, Rest...>::value>
{};

// Enable if T is an element of Types...
template <typename R, typename T, typename... Types>
using EnableIfElement = typename std::enable_if<HasType<T, Types...>::value, R>::type;
// Enable if T is not an element of Types...
template <typename R, typename T, typename... Types>
using EnableIfNotElement = typename std::enable_if<!HasType<T, Types...>::value, R>::type;

// Enable if T is convertible to an element of Types... T is considered convertible IIF a single element of Types... is
// assignable from T and T is not a direct element of Types...
template <typename R, typename T, typename... Types>
using EnableIfConvertible =
    typename std::enable_if<!HasType<T, Types...>::value && ConstructibleCount<T, Types...>::value == 1, R>::type;

// Enable if T is assignable to an element of Types... T is considered assignable IFF a single element of Types... is
// constructible from T or T is a direct element of Types.... Note that T is REQUIRED to be an element of Types... when
// multiple elements are constructible from T to prevent ambiguity in conversion.
template <typename R, typename T, typename... Types>
using EnableIfAssignable =
    typename std::enable_if<HasType<T, Types...>::value || ConstructibleCount<T, Types...>::value == 1, R>::type;

// Selects a type for SFINAE constructor selection.
template <bool CondA, typename SelectA, typename SelectB>
using Select = std::conditional_t<CondA, SelectA, SelectB>;

// Recursive union type.
template <typename... Types>
union Union
{};

// Specialization handling a singular type, terminating template recursion.
template <typename Type>
union Union<Type>
{
    Union()
    {}

    ~Union()
    {}

    template <typename T>
    Union(std::int32_t index, std::int32_t* index_out, TypeTag<Type>, T&& value) : mFirst(std::forward<T>(value))
    {
        *index_out = index;
    }

    template <typename T, typename = EnableIfAssignable<void, T, Type>>
    Union(std::int32_t index, std::int32_t* index_out, T&& value) : mFirst(std::forward<T>(value))
    {
        *index_out = index;
    }

    Union(const Union& other, std::int32_t index)
    {
        if (index == 0)
            new (&mFirst) Type(other.mFirst);
    }

    Union(Union&& other, std::int32_t index)
    {
        if (index == 0)
            new (&mFirst) Type(std::move(other.mFirst));
    }

    Union(const Union&) = delete;
    Union(Union&&) = delete;
    void operator=(const Union&) = delete;
    void operator=(Union&&) = delete;

    Type& get(TypeTag<Type>)
    {
        return mFirst;
    }

    const Type& get(TypeTag<Type>) const
    {
        return mFirst;
    }

    EmptyVariant get(TypeTag<EmptyVariant>) const
    {
        return {};
    }

    constexpr std::int32_t index(TypeTag<Type>) const
    {
        return 0;
    }

    template <typename... Args>
    std::int32_t Construct(TypeTag<Type>, Args&&... args)
    {
        new (&mFirst) Type(std::forward<Args>(args)...);
        return 0;
    }

    template <typename... Args>
    EnableIfConstructible<std::int32_t, Type, Args...> Construct(Args&&... args)
    {
        new (&mFirst) Type(std::forward<Args>(args)...);
        return 0;
    }

    void Destruct(std::int32_t target_index)
    {
        if (target_index == index(TypeTag<Type>{})) {
            (&get(TypeTag<Type>{}))->~Type();
        }
    }

    template <typename T>
    bool Assign(TypeTag<Type>, std::int32_t target_index, T&& value)
    {
        if (target_index == 0) {
            mFirst = std::forward<T>(value);
            return true;
        } else {
            return false;
        }
    }

    template <typename T>
    EnableIfConstructible<bool, Type, T> Assign(std::int32_t target_index, T&& value)
    {
        if (target_index == 0) {
            mFirst = std::forward<T>(value);
            return true;
        } else {
            return false;
        }
    }

    template <typename T>
    EnableIfNotConstructible<bool, Type, T> Assign(std::int32_t /*target_index*/, T&& /*value*/)
    {
        return false;
    }

    template <typename Op>
    decltype(auto) Visit(std::int32_t target_index, Op&& op)
    {
        if (target_index == index(TypeTag<Type>{}))
            return std::forward<Op>(op)(get(TypeTag<Type>{}));
        else
            return std::forward<Op>(op)(get(TypeTag<EmptyVariant>{}));
    }

    template <typename Op>
    decltype(auto) Visit(std::int32_t target_index, Op&& op) const
    {
        if (target_index == index(TypeTag<Type>{}))
            return std::forward<Op>(op)(get(TypeTag<Type>{}));
        else
            return std::forward<Op>(op)(get(TypeTag<EmptyVariant>{}));
    }

    template <typename... Args>
    bool Become(std::int32_t target_index, Args&&... args)
    {
        if (target_index == index(TypeTag<Type>{})) {
            Construct(TypeTag<Type>{}, std::forward<Args>(args)...);
            return true;
        } else {
            return false;
        }
    }

  private:
    Type mFirst;
};

// Specialization that recursively unions types from the paramater pack.
template <typename First, typename... Rest>
union Union<First, Rest...>
{
    Union()
    {}

    ~Union()
    {}

    template <typename T>
    Union(std::int32_t index, std::int32_t* index_out, TypeTag<First>, T&& value) : mFirst(std::forward<T>(value))
    {
        *index_out = index;
    }

    template <typename T, typename U>
    Union(std::int32_t index, std::int32_t* index_out, TypeTag<T>, U&& value)
        : mRest(index + 1, index_out, TypeTag<T>{}, std::forward<U>(value))
    {}

    Union(const Union& other, std::int32_t index)
    {
        if (index == 0)
            new (&mFirst) First(other.mFirst);
        else
            new (&mRest) Union<Rest...>(other.mRest, index - 1);
    }

    Union(Union&& other, std::int32_t index)
    {
        if (index == 0)
            new (&mFirst) First(std::move(other.mFirst));
        else
            new (&mRest) Union<Rest...>(std::move(other.mRest), index - 1);
    }

    Union(const Union&) = delete;
    Union(Union&&) = delete;
    void operator=(const Union&) = delete;
    void operator=(Union&&) = delete;

    struct FirstType
    {};

    struct RestType
    {};

    template <typename T>
    using SelectConstructor = Select<ConstructibleCount<T, First>::value == 1, FirstType, RestType>;

    template <typename T>
    Union(std::int32_t index, std::int32_t* index_out, T&& value)
        : Union(index, index_out, std::forward<T>(value), SelectConstructor<T>{})
    {}

    template <typename T>
    Union(std::int32_t index, std::int32_t* index_out, T&& value, FirstType) : mFirst(std::forward<T>(value))
    {
        *index_out = index;
    }

    template <typename T>
    Union(std::int32_t index, std::int32_t* index_out, T&& value, RestType)
        : mRest(index + 1, index_out, std::forward<T>(value))
    {}

    First& get(TypeTag<First>)
    {
        return mFirst;
    }

    const First& get(TypeTag<First>) const
    {
        return mFirst;
    }

    constexpr std::int32_t index(TypeTag<First>) const
    {
        return 0;
    }

    template <typename T>
    T& get(TypeTag<T>)
    {
        return mRest.get(TypeTag<T>{});
    }

    template <typename T>
    const T& get(TypeTag<T>) const
    {
        return mRest.get(TypeTag<T>{});
    }

    template <typename T>
    constexpr std::int32_t index(TypeTag<T>) const
    {
        return 1 + mRest.index(TypeTag<T>{});
    }

    template <typename... Args>
    std::int32_t Construct(TypeTag<First>, Args&&... args)
    {
        new (&mFirst) First(std::forward<Args>(args)...);
        return 0;
    }

    template <typename T, typename... Args>
    std::int32_t Construct(TypeTag<T>, Args&&... args)
    {
        return 1 + mRest.Construct(TypeTag<T>{}, std::forward<Args>(args)...);
    }

    template <typename... Args>
    EnableIfConstructible<std::int32_t, First, Args...> Construct(Args&&... args)
    {
        new (&mFirst) First(std::forward<Args>(args)...);
        return 0;
    }

    template <typename... Args>
    EnableIfNotConstructible<std::int32_t, First, Args...> Construct(Args&&... args)
    {
        return 1 + mRest.Construct(std::forward<Args>(args)...);
    }

    void Destruct(std::int32_t target_index)
    {
        if (target_index == index(TypeTag<First>{})) {
            (get(TypeTag<First>{})).~First();
        } else {
            mRest.Destruct(target_index - 1);
        }
    }

    template <typename T>
    bool Assign(TypeTag<First>, std::int32_t target_index, T&& value)
    {
        if (target_index == 0) {
            mFirst = std::forward<T>(value);
            return true;
        } else {
            return false;
        }
    }

    template <typename T, typename U>
    bool Assign(TypeTag<T>, std::int32_t target_index, U&& value)
    {
        return mRest.Assign(TypeTag<T>{}, target_index - 1, std::forward<U>(value));
    }

    template <typename T>
    EnableIfConstructible<bool, First, T> Assign(std::int32_t target_index, T&& value)
    {
        if (target_index == 0) {
            mFirst = std::forward<T>(value);
            return true;
        } else {
            return mRest.Assign(target_index - 1, std::forward<T>(value));
        }
    }

    template <typename T>
    EnableIfNotConstructible<bool, First, T> Assign(std::int32_t target_index, T&& value)
    {
        return mRest.Assign(target_index - 1, std::forward<T>(value));
    }

    // Recursively traverses the union and calls Op on the active value when the active type is found.
    // If the union is empty Op is called on EmptyVariant.
    template <typename Op>
    decltype(auto) Visit(std::int32_t target_index, Op&& op)
    {
        if (target_index == index(TypeTag<First>{}))
            return std::forward<Op>(op)(get(TypeTag<First>{}));
        else
            return mRest.Visit(target_index - 1, std::forward<Op>(op));
    }

    template <typename Op>
    decltype(auto) Visit(std::int32_t target_index, Op&& op) const
    {
        if (target_index == index(TypeTag<First>{}))
            return std::forward<Op>(op)(get(TypeTag<First>{}));
        else
            return mRest.Visit(target_index - 1, std::forward<Op>(op));
    }

    template <typename... Args>
    bool Become(std::int32_t target_index, Args&&... args)
    {
        if (target_index == index(TypeTag<First>{})) {
            Construct(TypeTag<First>{}, std::forward<Args>(args)...);
            return true;
        } else {
            return mRest.Become(target_index - 1, std::forward<Args>(args)...);
        }
    }

  private:
    First mFirst;
    Union<Rest...> mRest;
};

}  // namespace detail

/// @uptrace{SWS_CORE_01601}
template <typename... Types>
class Variant
{
  private:
    // Convenience types.
    template <typename T>
    using TypeTag = detail::TypeTag<T>;
    template <typename T>
    using DecayedTypeTag = TypeTag<std::decay_t<T>>;
    template <std::size_t I>
    using TypeForIndex = detail::TypeForIndex<I, Types...>;
    template <std::size_t I>
    using TypeTagForIndex = detail::TypeTagForIndex<I, Types...>;
    template <typename T>
    using HasType = detail::HasType<T, Types...>;
    template <typename R, typename T>
    using EnableIfElement = detail::EnableIfElement<R, T, Types...>;
    template <typename R, typename T>
    using EnableIfConvertible = detail::EnableIfConvertible<R, T, Types...>;
    template <typename R, typename T>
    using EnableIfAssignable = detail::EnableIfAssignable<R, T, Types...>;

    struct Direct
    {};

    struct Convert
    {};

    template <typename T>
    using SelectConstructor = detail::Select<HasType<T>::value, Direct, Convert>;

    // Constructs by type tag when T is an direct element of Types...
    template <typename T>
    explicit Variant(T&& value, Direct) : mValue(0, &mIndex, DecayedTypeTag<T>{}, std::forward<T>(value))
    {}

    // Conversion constructor when T is not a direct element of Types...
    template <typename T>
    explicit Variant(T&& value, Convert) : mValue(0, &mIndex, std::forward<T>(value))
    {}

  public:
    /// @brief Variants are default construcible, regardless of whether the elements are default constructible.
    ///        Default consruction yields an empty Variant.
    /// @uptrace{SWS_CM_01051}
    Variant()
    {}

    explicit Variant(EmptyVariant value)
    {
        Construct(value);
    }

    /// @brief Destructor of Variant
    /// @uptrace{SWS_CM_01059}
    ~Variant()
    {
        Destruct();
    }

    /// @brief Copy constructor of Variant
    /// @uptrace{SWS_CM_01053}
    Variant(const Variant& other) : mIndex{other.mIndex}, mValue{other.mValue, other.mIndex}
    {}

    /// @brief Move constructor of Variant
    /// @uptrace{SWS_CM_01052}
    Variant(Variant&& other) : mIndex{other.mIndex}, mValue{std::move(other.mValue), other.mIndex}
    {}

    /// @brief Copy and move construction from Variant types.
    ///        Each element of OtherTypes must be convertible to an element of Types.
    /// @uptrace{SWS_CM_01053}
    template <typename... OtherTypes>
    explicit Variant(const Variant<OtherTypes...>& other)
    {
        other.Visit([this](const auto& value) {
            this->Construct(value);
        });
    }

    /// @brief Copy and move construction from Variant types.
    ///        Each element of OtherTypes must be convertible to an element of Types.
    /// @uptrace{SWS_CM_01054}
    template <typename... OtherTypes>
    explicit Variant(Variant<OtherTypes...>&& other)
    {
        other.Visit([this](auto&& value) {
            this->Construct(std::move(value));
        });
    }

    /// @brief Variant default copy assignment operator
    /// @uptrace{SWS_CM_01061}
    Variant& operator=(const Variant& other)
    {
        other.Visit([this](const auto& value) {
            *this = value;
        });
        return *this;
    }

    /// @brief Variant default move assignment operator
    /// @uptrace{SWS_CM_01060}
    Variant& operator=(Variant&& other)
    {
        other.Visit([this](auto&& value) {
            *this = std::move(value);
        });
        return *this;
    }

    /// @brief Construction from non-Variant types.
    template <typename T, typename = EnableIfAssignable<void, T>>
    explicit Variant(T&& value) : Variant(std::forward<T>(value), SelectConstructor<T>{})
    {}

    /// @brief Performs assignment from type T belonging to Types.
    ///        This overload takes priority to prevent implicit conversion in cases where T is implicitly convertible to
    ///        multiple elements of Types.
    template <typename T>
    EnableIfElement<Variant&, T> operator=(T&& value)
    {
        Assign(DecayedTypeTag<T>{}, std::forward<T>(value));
        return *this;
    }

    /// @brief Performs assignment from type T not belonging to Types.
    ///        This overload matches in cases where conversion is the only viable option.
    template <typename T>
    EnableIfConvertible<Variant&, T> operator=(T&& value)
    {
        Assign(std::forward<T>(value));
        return *this;
    }

    /// @brief Handles assignment from the empty type.
    ///        This overload supports assignment in visitors using generic lambdas.
    Variant& operator=(EmptyVariant)
    {
        Destruct();
        return *this;
    }

    /// @brief Assignment from Variant types.
    ///        Each element of OtherTypes must be convertible to an element of Types.
    ///        Forwards through non-Variant assignment operators to apply conversion checks.
    template <typename... OtherTypes>
    Variant& operator=(const Variant<OtherTypes...>& other)
    {
        other.Visit([this](const auto& value) {
            *this = value;
        });
        return *this;
    }

    /// @brief Assignment from Variant types.
    ///        Each element of OtherTypes must be convertible to an element of Types.
    ///        Forwards through non-Variant assignment operators to apply conversion checks.
    template <typename... OtherTypes>
    Variant& operator=(Variant<OtherTypes...>&& other)
    {
        other.Visit([this](auto&& value) {
            *this = std::move(value);
        });
        return *this;
    }

    /// @brief Becomes the target type, constructing a new element from the given arguments if necessary.
    //         No action is taken if the active element is already the target type.
    //         Otherwise the active element is destroyed and replaced by constructing an element of the new type using
    //         |Args|. An invalid target type index results in an empty Variant.
    template <typename... Args>
    void Become(std::int32_t target_index, Args&&... args)
    {
        if (target_index != index()) {
            Destruct();
            mIndex = mValue.Become(target_index, std::forward<Args>(args)...) ? target_index : kEmptyIndex;
        }
    }

    /// @brief Invokes |Op| on the active element.
    ///        If the Variant is empty |Op| is invoked on EmptyVariant.
    template <typename Op>
    decltype(auto) Visit(Op&& op)
    {
        return mValue.Visit(mIndex, std::forward<Op>(op));
    }

    template <typename Op>
    decltype(auto) Visit(Op&& op) const
    {
        return mValue.Visit(mIndex, std::forward<Op>(op));
    }

    /// @brief Index returned when the Variant is empty.
    enum : std::int32_t
    {
        kEmptyIndex = -1
    };

    /// @brief Returns the index of the given type.
    template <typename T>
    constexpr std::int32_t index_of() const
    {
        static_assert(HasType<T>::value, "T is not an element type of Variant.");
        return mValue.index(DecayedTypeTag<T>{});
    }

    /// @brief Returns the index of the active type. If the Variant is empty -1 is returned.
    /// @uptrace{SWS_CM_01063}
    std::int32_t index() const
    {
        return mIndex;
    }

    /// @brief Returns true if the given type is active, false otherwise.
    template <typename T>
    bool is() const
    {
        static_assert(HasType<T>::value, "T is not an element type of Variant.");
        return index() == index_of<T>();
    }

    /// @brief Returns true if the Variant is empty, false otherwise.
    bool empty() const
    {
        return index() == kEmptyIndex;
    }

    /// @brief Returns true if the Variant is empty, false otherwise.
    /// @uptrace{SWS_CM_01064}
    bool valueless_by_exception() const noexcept
    {
        return index() == kEmptyIndex;
    }

    /// @brief Element accessors.
    ///        Returns a pointer to the active value if the given type/index is active, otherwise nullptr is returned.
    template <typename T>
    T* get()
    {
        if (is<T>())
            return &mValue.get(DecayedTypeTag<T>{});
        else
            return nullptr;
    }

    /// @brief Element accessors.
    ///        Returns a pointer to the active value if the given type/index is active, otherwise nullptr is returned.
    template <typename T>
    const T* get() const
    {
        if (is<T>())
            return &mValue.get(DecayedTypeTag<T>{});
        else
            return nullptr;
    }

    /// @brief Element accessors.
    ///        Returns a pointer to the active value if the given type/index is active, otherwise nullptr is returned.
    template <std::size_t I>
    TypeForIndex<I>* get()
    {
        if (is<TypeForIndex<I>>())
            return &mValue.get(TypeTagForIndex<I>{});
        else
            return nullptr;
    }

    /// @brief Element accessors.
    ///        Returns a pointer to the active value if the given type/index is active, otherwise nullptr is returned.
    template <std::size_t I>
    const TypeForIndex<I>* get() const
    {
        if (is<TypeForIndex<I>>())
            return &mValue.get(TypeTagForIndex<I>{});
        else
            return nullptr;
    }

    /// @brief Variant function to create a new value in-place, in an existing Variant object.
    /// @uptrace{SWS_CM_01066}
    template <typename T, typename... Args>
    void emplace(Args&&... args)
    {
        Destruct();
        Construct(TypeTag<T>{}, std::forward<Args>(args)...);
    }

    /// @brief Variant function to create a new value in-place, in an existing Variant object by destoying and
    /// initializing the contained value
    /// @uptrace{SWS_CM_01068}
    template <std::size_t I, typename T>
    void emplace(T&& value)
    {
        Become(I);
        if (mIndex == I) {
            Assign(std::forward<T>(value));
        }
    }

    template <std::size_t I, typename... Args>
    void emplace(Args&&... args)
    {
        emplace(args...);
    }

    /// @brief Variant function to swap two Variants
    /// @uptrace{SWS_CM_01065}
    void swap(Variant<Types...>& other) noexcept
    {
        Variant temp(std::move(other));
        other = *this;
        *this = temp;
    }

  private:
    std::int32_t mIndex = kEmptyIndex;
    detail::Union<std::decay_t<Types>...> mValue;

    // Constructs an element from the given arguments and sets the Variant to the resulting type.
    template <typename... Args>
    void Construct(Args&&... args)
    {
        mIndex = mValue.template Construct(std::forward<Args>(args)...);
    }

    void Construct(EmptyVariant)
    {}

    // Destroys the active element of the Variant.
    void Destruct()
    {
        mValue.Destruct(mIndex);
        mIndex = kEmptyIndex;
    }

    // Assigns the Variant when non-empty and the current type matches the target type,
    // otherwise destroys the current value and constructs a element of the new type.
    // Tagged assignment is used when T is an element of the Variant to prevent implicit conversion in cases where T is
    // implicitly convertible to multiple element types.
    template <typename T, typename U>
    void Assign(TypeTag<T>, U&& value)
    {
        if (!mValue.template Assign(TypeTag<T>{}, mIndex, std::forward<U>(value))) {
            Destruct();
            Construct(TypeTag<T>{}, std::forward<U>(value));
        }
    }

    template <typename T>
    void Assign(T&& value)
    {
        if (!mValue.template Assign(mIndex, std::forward<T>(value))) {
            Destruct();
            Construct(std::forward<T>(value));
        }
    }
};

/// @brief Utility type to extract/convert values from a variant.
///        This class simplifies conditional logic to get/move/swap/action values from a variant when one or more
///        elements are compatible with the destination type.
template <typename... ValidTypes>
struct IfAnyOf
{
    /// @brief Calls Op on the underlying value of the variant and returns true when the variant is a valid type,
    /// otherwise does nothing and returns false.
    template <typename Op, typename... Types>
    static bool Call(Variant<Types...>* variant, Op&& op)
    {
        static_assert(detail::Set<Types...>::template IsSubset<ValidTypes...>::value,
                      "ValidTypes may only contain element types from the Variant.");
        return variant->Visit(CallOp<Op>{std::forward<Op>(op)});
    }

    template <typename Op, typename... Types>
    static bool Call(const Variant<Types...>* variant, Op&& op)
    {
        static_assert(detail::Set<Types...>::template IsSubset<ValidTypes...>::value,
                      "ValidTypes may only contain element types from the Variant.");
        return variant->Visit(CallOp<Op>{std::forward<Op>(op)});
    }

    /// @brief Gets/converts the underlying value of the variant to type T and returns true when the variant is a valid
    /// type,
    ///        otherwise does nothing and returns false.
    template <typename T, typename... Types>
    static bool Get(const Variant<Types...>* variant, T* value_out)
    {
        return Call(variant, [value_out](const auto& value) {
            *value_out = value;
        });
    }

    /// @brief Moves the underlying value of the variant and returns true when the variant is a valid type,
    ///        otherwise does nothing and returns false.
    template <typename T, typename... Types>
    static bool Take(Variant<Types...>* variant, T* value_out)
    {
        return Call(variant, [value_out](auto&& value) {
            *value_out = std::move(value);
        });
    }

    /// @brief Swaps the underlying value of the variant with |*value_out| and returns true when the variant is a valid
    /// type,
    ///        otherwise does nothing and returns false.
    template <typename T, typename... Types>
    static bool Swap(Variant<Types...>* variant, T* value_out)
    {
        return Call(variant, [value_out](auto&& value) {
            std::swap(*value_out, value);
        });
    }

  private:
    template <typename Op>
    struct CallOp
    {
        Op&& op;

        template <typename U>
        detail::EnableIfNotElement<bool, U, ValidTypes...> operator()(U&&)
        {
            return false;
        }

        template <typename U>
        detail::EnableIfElement<bool, U, ValidTypes...> operator()(const U& value)
        {
            std::forward<Op>(op)(value);
            return true;
        }

        template <typename U>
        detail::EnableIfElement<bool, U, ValidTypes...> operator()(U&& value)
        {
            std::forward<Op>(op)(std::forward<U>(value));
            return true;
        }
    };
};

}  // namespace core
}  // namespace ara

// Overloads of std::get<T> and std::get<I> for ara::core::Variant
namespace std
{

template <typename T, typename... Types>
inline T& get(::ara::core::Variant<Types...>& v)
{
    return *v.template get<T>();
}

template <typename T, typename... Types>
inline T&& get(::ara::core::Variant<Types...>&& v)
{
    return std::move(*v.template get<T>());
}

template <typename T, typename... Types>
inline const T& get(const ::ara::core::Variant<Types...>& v)
{
    return *v.template get<T>();
}

template <std::size_t I, typename... Types>
inline ::ara::core::detail::TypeForIndex<I, Types...>& get(::ara::core::Variant<Types...>& v)
{
    return *v.template get<I>();
}

template <std::size_t I, typename... Types>
inline ::ara::core::detail::TypeForIndex<I, Types...>&& get(::ara::core::Variant<Types...>&& v)
{
    return std::move(*v.template get<I>());
}

template <std::size_t I, typename... Types>
inline const ::ara::core::detail::TypeForIndex<I, Types...>& get(const ::ara::core::Variant<Types...>& v)
{
    return *v.template get<I>();
}

}  // namespace std

#endif  // ARA_CORE_VARIANT_H_