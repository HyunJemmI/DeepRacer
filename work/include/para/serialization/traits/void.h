// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef PARA_SERIALIZATION_TRAITS_VOID_H_
#define PARA_SERIALIZATION_TRAITS_VOID_H_

namespace para
{
namespace serialization
{
namespace detail
{

template <typename... Ts>
struct MakeVoid
{
    using Type = void;
};

}  // namespace detail

// Utility type for SFINAE expression evaluation. C++14 equivalent to C++17 std::void_t.
// This approach addresses the issue with older C++14 compilers described in:
// http://open-std.org/JTC1/SC22/WG21/docs/cwg_defects.html#1558
template <typename... Ts>
using Void = typename detail::MakeVoid<Ts...>::Type;

}  // namespace serialization
}  // namespace para

#endif  // PARA_SERIALIZE_TRAITS_VOID_H_
