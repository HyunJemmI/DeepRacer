// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef PARA_SERIALIZATION_TRAITS_IS_TEMPLATE_BASE_OF_H_
#define PARA_SERIALIZATION_TRAITS_IS_TEMPLATE_BASE_OF_H_

#include <type_traits>

namespace para
{
namespace serialization
{
namespace detail
{

// Utility to deduce the template type from a derived type.
template <template <typename...> class TT, typename... Ts>
std::true_type DeduceTemplateType(const TT<Ts...>*);
template <template <typename...> class TT>
std::false_type DeduceTemplateType(...);

}  // namespace detail

// Utility determining whether template type TT<...> is a base of type T.
template <template <typename...> class TT, typename T>
using IsTemplateBaseOf = decltype(detail::DeduceTemplateType<TT>(std::declval<T*>()));

}  // namespace serialization
}  // namespace para

#endif  // PARA_SERIALIZATION_TRAITS_IS_TEMPLATE_BASE_OF_H_
