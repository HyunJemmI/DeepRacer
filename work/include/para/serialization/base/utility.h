// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef PARA_SERIALIZATION_BASE_UTILITY_H_
#define PARA_SERIALIZATION_BASE_UTILITY_H_

#include <array>
#include <cstddef>
#include <type_traits>

#include "para/serialization/traits/is_template_base_of.h"
#include "para/serialization/traits/void.h"
#include "para/serialization/types/external.h"

namespace para
{
namespace serialization
{

// Counting template for recursive template definitions.
template <std::size_t>
struct Index
{};

// Passthrough type.
template <typename T>
using Identity = T;

// Trait to determine if all the types in a parameter pack are integral types.
template <typename...>
struct IsIntegral;

template <typename T>
struct IsIntegral<T> : std::is_integral<T>
{};

template <typename First, typename... Rest>
struct IsIntegral<First, Rest...> : std::integral_constant<bool, IsIntegral<First>::value && IsIntegral<Rest...>::value>
{};

// Trait to determine if all types in a parameter pack are arithmetic types.
template <typename...>
struct IsArithmetic;

template <typename T>
struct IsArithmetic<T> : std::is_arithmetic<T>
{};

template <typename First, typename... Rest>
struct IsArithmetic<First, Rest...>
    : std::integral_constant<bool, IsArithmetic<First>::value && IsArithmetic<Rest...>::value>
{};

// Trait to determine if all types in a parameter pack are enum types.
template <typename...>
struct IsEnum;

template <typename T>
struct IsEnum<T> : std::is_enum<T>
{};

// Trait to determine if all types in a parameter pack are array types.
template <typename...>
struct IsArray;

template <typename T>
struct IsArray<T> : std::is_array<T>
{};

template <typename T, std::size_t N>
struct IsArray<T[N]> : std::true_type
{};

template <typename T, std::size_t N>
struct IsArray<std::array<T, N>> : std::true_type
{};

template <typename T, std::size_t N>
struct IsArray<const std::array<T, N>> : std::true_type
{};

template <typename T, std::size_t N>
struct IsArray<ara::core::Array<T, N>> : std::true_type
{};

template <typename T, std::size_t N>
struct IsArray<const ara::core::Array<T, N>> : std::true_type
{};

// Enable if every entry of Types is an integral type.
template <typename... Types>
using EnableIfIntegral = typename std::enable_if<IsIntegral<Types...>::value>::type;

// Enable if any entry of Types is not an integral type.
template <typename... Types>
using EnableIfNotIntegral = typename std::enable_if<!IsIntegral<Types...>::value>::type;

// Enable if every entry of Types is an arithmetic type.
template <typename... Types>
using EnableIfArithmetic = typename std::enable_if<IsArithmetic<Types...>::value>::type;

// Enable if every entry of Types is not an arithmetic type.
template <typename... Types>
using EnableIfNotArithmetic = typename std::enable_if<!IsArithmetic<Types...>::value>::type;

template <typename T, typename U>
using EnableIfConvertible = typename std::enable_if<std::is_convertible<T, U>::value>::type;

// Utility type to retrieve the first type in a parameter pack.
template <typename...>
struct FirstType
{};

template <typename First, typename... Rest>
struct FirstType<First, Rest...>
{
    using Type = First;
};

template <typename... Ts>
using First = typename FirstType<Ts...>::Type;

// Determines the value type and extent of C/C++ array types.
template <typename T>
struct ArrayTraits : std::false_type
{};

template <typename T, std::size_t Length_>
struct ArrayTraits<T[Length_]> : std::true_type
{
    enum : std::size_t
    {
        Length = Length_
    };

    using ElementType = T;
    using Type = T[Length];
};

template <typename T, std::size_t Length_>
struct ArrayTraits<std::array<T, Length_>> : std::true_type
{
    enum : std::size_t
    {
        Length = Length_
    };

    using ElementType = T;
    using Type = std::array<T, Length>;
};

template <typename T, std::size_t Length_>
struct ArrayTraits<const std::array<T, Length_>> : std::true_type
{
    enum : std::size_t
    {
        Length = Length_
    };

    using ElementType = T;
    using Type = const std::array<T, Length>;
};

// Enable if T is an array type.
template <typename T, typename Return = void>
using EnableIfArray = typename std::enable_if<ArrayTraits<T>::value, Return>::type;

// Logical AND over template parameter pack.
template <typename... T>
struct And : std::true_type
{};

template <typename A>
struct And<A> : A
{};

template <typename A, typename B>
struct And<A, B> : std::integral_constant<bool, A::value && B::value>
{};

template <typename A, typename B, typename... Rest>
struct And<A, B, Rest...> : And<A, And<B, Rest...>>
{};

// Logical OR over template parameter pack.
template <typename... T>
struct Or : std::true_type
{};

template <typename A>
struct Or<A> : A
{};

template <typename A, typename B>
struct Or<A, B> : std::integral_constant<bool, A::value || B::value>
{};

template <typename A, typename B, typename... Rest>
struct Or<A, B, Rest...> : Or<A, Or<B, Rest...>>
{};

// Utility to determine whether a set of one or more types is a true set, containing no duplicates, according to the
// given comparison template. The comparison template must accept two type arguments and evaluate to true if its
// arguments are the same according to the intended criteria.
template <template <typename, typename> class, typename...>
struct IsUnique;

template <template <typename, typename> class Same>
struct IsUnique<Same> : std::true_type
{};

template <template <typename, typename> class Same, typename T>
struct IsUnique<Same, T> : std::true_type
{};

template <template <typename, typename> class Same, typename First, typename Second>
struct IsUnique<Same, First, Second> : std::integral_constant<bool, !Same<First, Second>::value>
{};

template <template <typename, typename> class Same, typename First, typename Second, typename... Rest>
struct IsUnique<Same, First, Second, Rest...>
    : And<IsUnique<Same, First, Second>, IsUnique<Same, First, Rest...>, IsUnique<Same, Second, Rest...>>
{};

// Utility to determine whether all types in a set compare the same using the given comparison.
template <template <typename, typename> class, typename...>
struct IsSame;

template <template <typename, typename> class Same>
struct IsSame<Same> : std::true_type
{};

template <template <typename, typename> class Same, typename T>
struct IsSame<Same, T> : std::true_type
{};

template <template <typename, typename> class Same, typename First, typename... Rest>
struct IsSame<Same, First, Rest...> : And<Same<First, Rest>...>
{};

// Utility types for SFINAE expression matching either regular or template types.
template <typename T, typename U>
std::is_same<T, U> MatchType();

template <template <typename...> class TT, template <typename...> class UU, typename... Ts>
std::is_same<TT<Ts...>, UU<Ts...>> MatchTemplate();

}  // namespace serialization
}  // namespace para

#endif  // PARA_SERIALIZATION_BASE_UTILITY_H_
