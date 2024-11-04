// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef PARA_SERIALIZATION_BASE_ENUM_H_
#define PARA_SERIALIZATION_BASE_ENUM_H_

#include <type_traits>

#include "para/serialization/base/encoding.h"

namespace para
{
namespace serialization
{
// Enable if T is an enumeration type.
template <typename T, typename ReturnType = void>
using EnableIfEnum = typename std::enable_if<std::is_enum<T>::value, ReturnType>::type;

// enum encoding format matches the encoding format of the underlying integer type.
template <typename T>
struct Encoding<T, EnableIfEnum<T>> : EncodingIO<T>
{
    static std::string TypeName()
    {
        return Encoding<IntegerType>::TypeName();
    }

    static constexpr std::size_t Size(const T& value)
    {
        return Encoding<IntegerType>::Size(static_cast<IntegerType>(value));
    }

    static constexpr std::size_t Size(const LengthField /*field*/, const T& value)
    {
        return Encoding<IntegerType>::Size(static_cast<IntegerType>(value));
    }

    template <typename Writer>
    static constexpr Status<void> WritePayload(const T& value, Writer* writer)
    {
        return Encoding<IntegerType>::WritePayload(reinterpret_cast<const IntegerType&>(value), writer);
    }

    template <typename Writer>
    static constexpr Status<void> WritePayload(const LengthField field, const Endianness endian,
                                               const Alignment alignment, const T& value, Writer* writer)
    {
        return Encoding<IntegerType>::WritePayload(field, endian, alignment,
                                                   reinterpret_cast<const IntegerType&>(value), writer);
    }

    template <typename Reader>
    static constexpr Status<void> ReadPayload(T* value, Reader* reader)
    {
        return Encoding<IntegerType>::ReadPayload(reinterpret_cast<IntegerType*>(value), reader);
    }

    template <typename Reader>
    static constexpr Status<void> ReadPayload(const LengthField field, const Endianness endian,
                                              const Alignment alignment, T* value, Reader* reader)
    {
        return Encoding<IntegerType>::ReadPayload(field, endian, alignment, reinterpret_cast<IntegerType*>(value),
                                                  reader);
    }

  private:
    using IntegerType = typename std::underlying_type<T>::type;
};

}  // namespace serialization
}  // namespace para

#endif  // PARA_SERIALIZATION_BASE_ENUM_H_
