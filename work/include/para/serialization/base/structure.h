// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef PARA_SERIALIZATION_STRUCTURE_H_
#define PARA_SERIALIZATION_STRUCTURE_H_

#include <type_traits>

#include "para/serialization/base/macros.h"
#include "para/serialization/types/member_pointer.h"

namespace para
{
namespace serialization
{
// User-defined structures are structs or classes that have been annotated so that the serialization engine understands
// how to read and write them. Defines the set of members belonging to a type that should be serialized/deserialized.
// This macro must be invoked once within the struct/class definition, preferrably in the private section for classes
// with private data.
#define PARA_STRUCTURE(type, ... /*members*/)                   \
    template <typename, typename>                               \
    friend struct ::para::serialization::Encoding;              \
    template <typename, typename>                               \
    friend struct ::para::serialization::HasInternalMemberList; \
    template <typename, typename>                               \
    friend struct ::para::serialization::MemberListTraits;      \
    using PARA__MEMBERS = ::para::serialization::MemberList<_PARA_MEMBER_LIST(type, __VA_ARGS__)>

// Defines the set of members belonging to a type that should be serialized/deserialized without changing the type
// itself. This is useful for making external library types with public data serializable.
#define PARA_EXTERNAL_STRUCTURE(type, ... /*members*/)                                                     \
    template <typename, typename>                                                                          \
    struct PARA__MEMBER_TRAITS;                                                                            \
    template <typename T>                                                                                  \
    struct PARA__MEMBER_TRAITS<T, _PARA_ENABLE_IF_TYPE_MATCH(T, type)>                                     \
    {                                                                                                      \
        using MemberList = ::para::serialization::MemberList<_PARA_MEMBER_LIST(T, __VA_ARGS__)>;           \
    };                                                                                                     \
    template <template <typename...> class TT, typename... Ts>                                             \
    struct PARA__MEMBER_TRAITS<TT<Ts...>, _PARA_ENABLE_IF_TEMPLATE_MATCH(TT, type, Ts...)>                 \
    {                                                                                                      \
        using MemberList = ::para::serialization::MemberList<_PARA_MEMBER_LIST(TT<Ts...>, __VA_ARGS__)>;   \
    };                                                                                                     \
    template <typename T>                                                                                  \
    inline _PARA_ENABLE_IF_TYPE_MATCH(T, type, PARA__MEMBER_TRAITS<T, void>) PARA__GetExternalMemberTraits \
        [[gnu::used]] (T*)                                                                                 \
    {                                                                                                      \
        return {};                                                                                         \
    }                                                                                                      \
    template <template <typename...> class TT, typename... Ts>                                             \
    inline _PARA_ENABLE_IF_TEMPLATE_MATCH(TT, type, Ts..., PARA__MEMBER_TRAITS<TT<Ts...>, void>)           \
        PARA__GetExternalMemberTraits [[gnu::used]] (TT<Ts...>*)                                           \
    {                                                                                                      \
        return {};                                                                                         \
    }

// Deprecated. PARA_EXTERNAL_STRUCTURE can handle both type and template arguments now. Aliases PARA_EXTERNAL_STRUCTURE
// for legacy code.
#define PARA_EXTERNAL_TEMPLATE PARA_EXTERNAL_STRUCTURE

// Tags the given type as an unbounded buffer.
// This macro must be invoked once within the struct/class definition, preferrably in the private section next to
// PARA_STRUCTURE.
#define PARA_UNBOUNDED_BUFFER(type) using PARA__UNBOUNDED_BUFFER = type

// Tags the given type as an unbounded buffer without changing the type itself.
#define PARA_EXTERNAL_UNBOUNDED_BUFFER(type)                                                             \
    template <typename, typename>                                                                        \
    struct PARA__UNBOUNDED_BUFFER;                                                                       \
    template <typename T>                                                                                \
    struct PARA__UNBOUNDED_BUFFER<T, _PARA_ENABLE_IF_TYPE_MATCH(T, type)>                                \
    {};                                                                                                  \
    template <template <typename...> class TT, typename... Ts>                                           \
    struct PARA__UNBOUNDED_BUFFER<TT<Ts...>, _PARA_ENABLE_IF_TEMPLATE_MATCH(TT, type, Ts...)>            \
    {};                                                                                                  \
    template <typename T>                                                                                \
    inline _PARA_ENABLE_IF_TYPE_MATCH(T, type, PARA__UNBOUNDED_BUFFER<T, void>) PARA__GetUnboundedBuffer \
        [[gnu::used]] (T*)                                                                               \
    {                                                                                                    \
        return {};                                                                                       \
    }                                                                                                    \
    template <template <typename...> class TT, typename... Ts>                                           \
    inline _PARA_ENABLE_IF_TEMPLATE_MATCH(TT, type, Ts..., PARA__UNBOUNDED_BUFFER<TT<Ts...>, void>)      \
        PARA__GetUnboundedBuffer [[gnu::used]] (TT<Ts...>*)                                              \
    {                                                                                                    \
        return {};                                                                                       \
    }

// Utility macros used by the macros above.

// Enable if |type| is a type or elaborated template type matching type T.
#define _PARA_ENABLE_IF_TYPE_MATCH(T, type, ... /*Return*/) \
    std::enable_if_t<decltype(::para::serialization::MatchType<T, type>())::value, ##__VA_ARGS__>

// Enable if |type| is an un-elaborated template type matching type TT.
#define _PARA_ENABLE_IF_TEMPLATE_MATCH(TT, type, Ts, ... /*Return*/) \
    std::enable_if_t<decltype(::para::serialization::MatchTemplate<TT, type, Ts>())::value, ##__VA_ARGS__>

// Generates a pair of template arguments (member pointer type and value) to be passed
// to MemberPointer<MemberPointerType, MemberPointerValue, ...> from the given type name and member name.
#define _PARA_MEMBER_POINTER(type, member) decltype(&type::member), &type::member

// Generates a MemberPointer type definition, given a type name and a variable number of member names.
// The first member name is handled here, while the remaining member names are passed to _PARA_MEMBER_POINTER_NEXT for
// recursive expansion.
#define _PARA_MEMBER_POINTER_FIRST(type, ...)                                                     \
    ::para::serialization::MemberPointer<_PARA_MEMBER_POINTER(type, _PARA_FIRST_ARG(__VA_ARGS__)) \
                                             _PARA_MEMBER_POINTER_NEXT(type, _PARA_REST_ARG(__VA_ARGS__))>

// Recursively handles the remaining member names in the template argument list for MemberPointer.
#define _PARA_MEMBER_POINTER_NEXT(type, ...)                    \
    _PARA_IF_ELSE(_PARA_HAS_ARGS(__VA_ARGS__))                  \
    (, _PARA_MEMBER_POINTER(type, _PARA_FIRST_ARG(__VA_ARGS__)) \
           _PARA_DEFER3(__PARA_MEMBER_POINTER_NEXT)()(type, _PARA_REST_ARG(__VA_ARGS__)))(/*done*/)

// Indirection to enable recursive macro expansion of _PARA_MEMBER_POINTER_NEXT.
#define __PARA_MEMBER_POINTER_NEXT() _PARA_MEMBER_POINTER_NEXT

// Defines a list of MemberPointer types given a type and list of member names.
#define _PARA_MEMBER_LIST(type, ...) PARA_MAP_ARGS(_PARA_MEMBER_POINTER_FIRST, (type), __VA_ARGS__)

}  // namespace serialization
}  // namespace para

#endif  // PARA_SERIALIZATION_STRUCTURE_H_