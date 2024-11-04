// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef PARA_SERIALIZATION_TRAITS_IS_COMPARABLE_H_
#define PARA_SERIALIZATION_TRAITS_IS_COMPARABLE_H_

#include <type_traits>

#include "para/serialization/traits/void.h"

namespace para
{
namespace serialization
{
// Trait that determines whether the given types T and U have a defined equal comparision operator with type T on the
// left and type U on the right.
template <typename T, typename U, typename = void>
struct IsComparableEqual : std::false_type
{};

template <typename T, typename U>
struct IsComparableEqual<T, U, Void<decltype(std::declval<const T&>() == std::declval<const U&>())>> : std::true_type
{};

// Enable if T is comparable to U for equality.
template <typename T, typename U, typename Return = void>
using EnableIfComparableEqual = typename std::enable_if<IsComparableEqual<T, U>::value, Return>::type;

// Trait that determines whether the given types T and U have a defined less than comparision operator with type T on
// the left and type U on the right.
template <typename T, typename U, typename = void>
struct IsComparableLess : std::false_type
{};

template <typename T, typename U>
struct IsComparableLess<T, U, Void<decltype(std::declval<const T&>() < std::declval<const U&>())>> : std::true_type
{};

// Enable if T is comparable to U for less than inequality.
template <typename T, typename U, typename Return = void>
using EnableIfComparableLess = typename std::enable_if<IsComparableLess<T, U>::value, Return>::type;

}  // namespace serialization
}  // namespace para

#endif  // PARA_SERIALIZATION_TRAITS_IS_COMPARABLE_H_
