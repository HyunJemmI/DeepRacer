// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef PARA_SERIALIZATION_TRAITS_IS_DETECTED_H_
#define PARA_SERIALIZATION_TRAITS_IS_DETECTED_H_

#include <type_traits>

#include "para/serialization/traits/void.h"

namespace para
{
namespace serialization
{
namespace detail
{

template <template <typename...> class Trait, typename Expression, typename... Args>
struct IsDetectedType : std::false_type
{};

template <template <typename...> class Trait, typename... Args>
struct IsDetectedType<Trait, Void<Trait<Args...>>, Args...> : std::true_type
{};

}  // namespace detail

// Trait that determines whether the given Trait template is successfully evaluated with the given Args.
// This makes it possible to express SFINAE detection expressions as templated aliases instead of full structure
// definitions with partial specializations.
//
// For example, a callable detector may be implemented as follows:
//
// template <typename Callable, typename... Args>
// using CallableTest =
//     decltype(std::declval<Callable>()(std::declval<Args>()...));
//
// template <typename Callable, typename... Args>
// using IsCallable = IsDetected<CallableTest, Callable, Args...>;
template <template <typename...> class Trait, typename... Args>
using IsDetected = detail::IsDetectedType<Trait, void, Args...>;

}  // namespace serialization
}  // namespace para

#endif  // PARA_SERIALIZATION_TRAITS_IS_DETECTED_H_
