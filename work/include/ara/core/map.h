// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef ARA_CORE_MAP_H_
#define ARA_CORE_MAP_H_

#include <map>
#include <memory>
#include <type_traits>
#include <utility>

namespace ara
{
namespace core
{
/// @brief A container that contains key-value pairs with unique keys
/// @tparam Key  the type of keys in this Map
/// @tparam T  the type of values in this Map
/// @tparam Compare  the type of comparison Callable
/// @tparam Allocator  the type of Allocator to use for this container
/// @uptrace{SWS_CORE_01400}
template <typename Key, typename T, typename Compare = std::less<Key>,
          typename Allocator = std::allocator<std::pair<const Key, T>>>
using Map = std::map<Key, T, Compare, Allocator>;

/// @brief Add overload of std::swap for Map.
/// @tparam Key  the type of keys in the Maps
/// @tparam T  the type of values in the Maps
/// @tparam Compare  the type of comparison Callables in the Maps
/// @tparam Allocator  the type of Allocators in the Maps
/// @param lhs  the first argument of the swap invocation
/// @param rhs  the second argument of the swap invocation
/// @uptrace{SWS_CORE_01496}
template <typename Key, typename T, typename Compare, typename Allocator,
          typename = typename std::enable_if<
              !std::is_same<Map<Key, T, Compare, Allocator>, std::map<Key, T, Compare, Allocator>>::value>::type>
void swap(Map<Key, T, Compare, Allocator>& lhs, Map<Key, T, Compare, Allocator>& rhs)
{
    lhs.swap(rhs);
}

}  // namespace core
}  // namespace ara

#endif  // ARA_CORE_MAP_H_
