// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef PARA_SERIALIZATION_TYPES_MEMBER_POINTER_H_
#define PARA_SERIALIZATION_TYPES_MEMBER_POINTER_H_

#include <functional>
#include <tuple>

#include "para/serialization/base/encoding.h"
#include "para/serialization/base/utility.h"
#include "para/serialization/traits/is_detected.h"

namespace para
{
namespace serialization
{
// Captures the type and value of a pointer to member.
template <typename T, T, typename U = void*, U = nullptr, typename Enable = void>
struct MemberPointer;

template <typename T, typename Class, T Class::*Pointer>
struct MemberPointer<T Class::*, Pointer>
{
    // Type of the memebr pointed to by this pointer.
    using Type = T;

    // Resolves a pointer to member with the given instance, yielding a pointer or reference to the member in that
    // instnace.
    static constexpr Type* Resolve(Class* instance)
    {
        return &(instance->*Pointer);
    }

    static constexpr const Type& Resolve(const Class& instance)
    {
        return (instance.*Pointer);
    }

    static std::string TypeName()
    {
        return Encoding<Type>::TypeName();
    }

    static constexpr std::size_t Size(const Class& instance)
    {
        return Encoding<Type>::Size(Resolve(instance));
    }

    static constexpr std::size_t Size(const LengthField field, const Class& instance)
    {
        return Encoding<Type>::Size(field, Resolve(instance));
    }

    //    static std::size_t SizeWithJson(const JsonGenericValue& value)
    //    {
    //        return Encoding<Type>::SizeWithJson(value);
    //    }

    template <typename Writer, typename MemberList>
    static constexpr Status<void> Write(const Class& instance, Writer* writer, MemberList /*member_list*/)
    {
        return Encoding<Type>::Write(Resolve(instance), writer);
    }

    template <typename Writer, typename MemberList>
    static constexpr Status<void> Write(const LengthField field, const Endianness endian, const Alignment alignment,
                                        const Class& instance, Writer* writer, MemberList /*member_list*/)
    {
        return Encoding<Type>::Write(field, endian, alignment, Resolve(instance), writer);
    }

    //    template <typename Writer, typename MemberList>
    //    static Status<void> Write(const JsonGenericValue& jsonValue, Writer* writer, MemberList /*member_list*/)
    //    {
    //        return Encoding<Type>::Write(jsonValue, writer);
    //    }

    template <typename Writer, typename MemberList>
    static constexpr Status<void> WritePayload(const Class& instance, Writer* writer, MemberList /*member_list*/)
    {
        return Encoding<Type>::WritePayload(Resolve(instance), writer);
    }

    template <typename Writer, typename MemberList>
    static constexpr Status<void> WritePayload(const LengthField field, const Endianness endian,
                                               const Alignment alignment, const Class& instance, Writer* writer,
                                               MemberList /*member_list*/)
    {
        return Encoding<Type>::WritePayload(field, endian, alignment, Resolve(instance), writer);
    }

    template <typename Reader, typename MemberList>
    static constexpr Status<void> Read(Class* instance, Reader* reader, MemberList /*member_list*/)
    {
        return Encoding<Type>::Read(Resolve(instance), reader);
    }

    template <typename Reader, typename MemberList>
    static constexpr Status<void> Read(const LengthField field, const Endianness endian, const Alignment alignment,
                                       Class* instance, Reader* reader, MemberList /*member_list*/)
    {
        return Encoding<Type>::Read(field, endian, alignment, Resolve(instance), reader);
    }

    template <typename Reader, typename MemberList>
    static constexpr Status<void> ReadPayload(Class* instance, Reader* reader, MemberList /*member_list*/)
    {
        return Encoding<Type>::ReadPayload(Resolve(instance), reader);
    }

    template <typename Reader, typename MemberList>
    static constexpr Status<void> ReadPayload(const LengthField field, const Endianness endian,
                                              const Alignment alignment, Class* instance, Reader* reader,
                                              MemberList /*member_list*/)
    {
        return Encoding<Type>::ReadPayload(field, endian, alignment, Resolve(instance), reader);
    }
};

// Test expression for the external unbounded logical buffer tag.
template <typename Class>
using ExternalUnboundedBufferTest = decltype(PARA__GetUnboundedBuffer(std::declval<Class*>()));

// Test expression for the internal unbounded logical buffer tag.
template <typename Class>
using InternalUnboundedBufferTest = typename Class::PARA__UNBOUNDED_BUFFER;

// Evaluates to true if Class is tagged as an unbounded logical buffer.
template <typename Class>
struct IsUnboundedBuffer
    : Or<IsDetected<ExternalUnboundedBufferTest, Class>, IsDetected<InternalUnboundedBufferTest, Class>>
{};

// Captures a list of MemberPointers.
template <typename... MemberPointers>
struct MemberList
{
    using Members = std::tuple<MemberPointers...>;

    enum : std::size_t
    {
        Count = sizeof...(MemberPointers)
    };

    template <std::size_t Index>
    using At = typename std::tuple_element<Index, Members>::type;
};

// Utility to retrieve a traits type that defines a MemberList for type T using ADL.
// The macros PARA_STRUCTURE, PARA_EXTERNAL_STRUCTURE, and PARA_EXTERNAL_TEMPLATE define
// the appropriate traits type and a defintion of PARA__GetExternalMemberTraits that this utility finds using ADL.
template <typename T>
using ExternalMemberTraits = decltype(PARA__GetExternalMemberTraits(std::declval<T*>()));

// Determines whether type T has a nested type named PARA__MEMBERS of template type MemberList.
template <typename T, typename = void>
struct HasInternalMemberList
{
  private:
    template <typename U>
    static constexpr bool Test(const typename U::PARA__MEMBERS*)
    {
        return IsTemplateBaseOf<MemberList, typename U::PARA__MEMBERS>::value;
    }

    template <typename U>
    static constexpr bool Test(...)
    {
        return false;
    }

  public:
    enum : bool
    {
        value = Test<T>(0)
    };

    // Always true if T does not have a PARA__MEMBERS member type.
    // If T does have the member type then only true if T is also default constructible.
    static_assert(!value || std::is_default_constructible<T>::value,
                  "Serializable types must be default constructible.");
};

// Determines whether type T has a properly defined traits type that can be
// discovered by ExternalMemberTraits above.
template <typename, typename = void>
struct HasExternalMemberList : std::false_type
{};

template <typename T>
struct HasExternalMemberList<T, Void<typename ExternalMemberTraits<T>::MemberList>>
    : std::integral_constant<bool, IsTemplateBaseOf<MemberList, typename ExternalMemberTraits<T>::MemberList>::value>
{
    static_assert(std::is_default_constructible<T>::value, "Serializable types must be default constructible.");
};

// Determines whether a type has either an internal or external MemberList as defined by the predicates above.
template <typename T>
struct HasMemberList : std::integral_constant<bool, HasInternalMemberList<T>::value || HasExternalMemberList<T>::value>
{};

// Enable utilities for member list predicates.
template <typename T, typename ReturnType = void>
using EnableIfHasInternalMemberList = typename std::enable_if<HasInternalMemberList<T>::value, ReturnType>::type;
template <typename T, typename ReturnType = void>
using EnableIfHasExternalMemberList = typename std::enable_if<HasExternalMemberList<T>::value, ReturnType>::type;
template <typename T, typename ReturnType = void>
using EnableIfHasMemberList = typename std::enable_if<HasMemberList<T>::value, ReturnType>::type;
template <typename T, typename ReturnType = void>
using EnableIfNotHasMemberList = typename std::enable_if<!HasMemberList<T>::value, ReturnType>::type;

// Traits type that retrieves the internal or external MemberList associated with type T.
template <typename T, typename = void>
struct MemberListTraits;

template <typename T>
struct MemberListTraits<T, EnableIfHasInternalMemberList<T>>
{
    using MemberList = typename T::PARA__MEMBERS;
};

template <typename T>
struct MemberListTraits<T, EnableIfHasExternalMemberList<T>>
{
    using MemberList = typename ExternalMemberTraits<T>::MemberList;
};

// Determines whether type T has a nested type named PARA__VALUE of template type MemberList.
template <typename T, typename = void>
struct IsValueWrapper
{
  private:
    template <typename U>
    static constexpr bool Test(const typename U::PARA__VALUE*)
    {
        return IsTemplateBaseOf<MemberList, typename U::PARA__VALUE>::value;
    }

    template <typename U>
    static constexpr bool Test(...)
    {
        return false;
    }

  public:
    enum : bool
    {
        value = Test<T>(0)
    };
};

// Enable utilities for value wrapper predicates.
template <typename T, typename ReturnType = void>
using EnableIfIsValueWrapper = typename std::enable_if<IsValueWrapper<T>::value, ReturnType>::type;
template <typename T, typename ReturnType = void>
using EnableIfIsNotValueWrapper = typename std::enable_if<!IsValueWrapper<T>::value, ReturnType>::type;

// Traits type that retrieves the internal MemberList and Pointer associated with type T.
template <typename T, typename = void>
struct ValueWrapperTraits;

template <typename T>
struct ValueWrapperTraits<T, EnableIfIsValueWrapper<T>>
{
    using MemberList = typename T::PARA__VALUE;
    using Pointer = typename MemberList::template At<0>;
};

}  // namespace serialization
}  // namespace para

#endif  // PARA_SERIALIZATION_TYPES_MEMBER_POINTER_H_