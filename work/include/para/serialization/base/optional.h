// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef PARA_SERIALIZATION_BASE_OPTIONAL_H_
#define PARA_SERIALIZATION_BASE_OPTIONAL_H_

#include "para/serialization/base/encoding.h"
#include "para/serialization/types/optional.h"

namespace para
{
namespace serialization
{
// Optional<T> encoding formats:
// Empty Optional<T>:
// +-----------------+
// | UINT32:NIL = -1 |
// +-----------------+
// Non-empty Optional<T>
// +-----------------+---//----+
// | UINT32:CONT = 0 | ELEMENT |
// +-----------------+---//----+
// Element must be a valid encoding of type T.
// Note: Optional is not standardized currently in R20-11 SOME/IP.
template <typename T>
struct Encoding<Optional<T>> : EncodingIO<Optional<T>>
{
    using Type = Optional<T>;

    static constexpr std::int32_t kNil = -1;
    static constexpr std::int32_t kContained = 0;

    static constexpr std::size_t Size(const Type& value)
    {
        return value ? Encoding<T>::Size(value.get()) : 4U;
    }

    static constexpr std::size_t Size(const LengthField field, const Type& value)
    {
        return value ? Encoding<T>::Size(field, value.get()) : 4U;
    }

    template <typename Writer>
    static constexpr Status<void> WritePayload(const Type& value, Writer* writer)
    {
        if (value) {
            auto status = Encoding<std::int32_t>::Write(kContained, writer);
            if (!status)
                return status;
            return Encoding<T>::WritePayload(value.get(), writer);
        } else {
            return Encoding<std::int32_t>::WritePayload(kNil, writer);
        }
    }

    template <typename Writer>
    static constexpr Status<void> WritePayload(const LengthField field, const Endianness endian,
                                               const Alignment alignment, const Type& value, Writer* writer)
    {
        if (value) {
            auto status = Encoding<std::int32_t>::Write(kContained, writer);
            if (!status)
                return status;
            return Encoding<T>::WritePayload(field, endian, alignment, value.get(), writer);
        } else {
            return Encoding<std::int32_t>::WritePayload(kNil, writer);
        }
    }

    template <typename Reader>
    static constexpr Status<void> ReadPayload(Type* value, Reader* reader)
    {
        std::int32_t flag = 0;
        auto status = Encoding<std::int32_t>::Read(&flag, reader);
        if (!status)
            return status;
        if (flag == kNil) {
            value->clear();
        } else {
            T temp;
            auto status = Encoding<T>::ReadPayload(&temp, reader);
            if (!status)
                return status;
            *value = std::move(temp);
        }
        return {};
    }

    template <typename Reader>
    static constexpr Status<void> ReadPayload(const LengthField field, const Endianness endian,
                                              const Alignment alignment, Type* value, Reader* reader)
    {
        std::int32_t flag = 0;
        auto status = Encoding<std::int32_t>::Read(&flag, reader);
        if (!status)
            return status;
        if (flag == kNil) {
            value->clear();
        } else {
            T temp;
            auto status = Encoding<T>::ReadPayload(field, endian, alignment, &temp, reader);
            if (!status)
                return status;
            *value = std::move(temp);
        }
        return {};
    }
};

}  // namespace serialization
}  // namespace para

#endif  // PARA_SERIALIZATION_BASE_OPTIONAL_H_
