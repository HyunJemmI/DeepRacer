// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef PARA_SERIALIZATION_BASE_ENCODING_HELPER_H_
#define PARA_SERIALIZATION_BASE_ENCODING_HELPER_H_

#include "para/serialization/base/encoding.h"
#include "para/serialization/base/status.h"

namespace para
{
namespace serialization
{

struct EncodingHelper
{
    template <typename Writer>
    static constexpr Status<void> WriteLength(SizeType length, Writer* writer)
    {
        return EncodingIO<SizeType>::Write(length, writer);
    }

    template <typename Writer>
    static constexpr Status<void> WriteLength(const LengthField field, const Endianness endian, SizeType length,
                                              Writer* writer)
    {
        switch (field) {
            case 0:
                // no write length
                return {};
            case 1:
                if (length > std::numeric_limits<std::uint8_t>::max())
                    return ErrorStatus::WriteLimitReached;
                return EncodingIO<std::uint8_t>::Write(field, endian, 0, length, writer);
            case 2:
                if (length > std::numeric_limits<std::uint16_t>::max())
                    return ErrorStatus::WriteLimitReached;
                return EncodingIO<std::uint16_t>::Write(field, endian, 0, length, writer);
            default:
                if (length > std::numeric_limits<std::uint32_t>::max())
                    return ErrorStatus::WriteLimitReached;
                return EncodingIO<std::uint32_t>::Write(field, endian, 0, length, writer);
        }
    }

    template <typename Reader>
    static constexpr Status<void> ReadLength(SizeType* size, Reader* reader)
    {
        return EncodingIO<SizeType>::Read(size, reader);
    }

    template <typename Reader>
    static constexpr Status<void> ReadLength(const LengthField field, const Endianness endian, SizeType* size,
                                             Reader* reader)
    {
        std::uint8_t length_1byte = 0;
        std::uint16_t length_2byte = 0;
        std::uint32_t length_4byte = 0;
        Status<void> status{};
        switch (field) {
            case 0:
                // no read length
                return {};
            case 1:
                status = EncodingIO<std::uint8_t>::Read(field, endian, 0, &length_1byte, reader);
                if (!status)
                    return status;
                if (length_1byte > std::numeric_limits<std::uint8_t>::max())
                    return ErrorStatus::ReadLimitReached;
                *size = length_1byte;
                return {};
            case 2:
                status = EncodingIO<std::uint16_t>::Read(field, endian, 0, &length_2byte, reader);
                if (!status)
                    return status;
                if (length_2byte > std::numeric_limits<std::uint16_t>::max())
                    return ErrorStatus::ReadLimitReached;
                *size = length_2byte;
                return {};
            default:
                status = EncodingIO<std::uint32_t>::Read(field, endian, 0, &length_4byte, reader);
                if (!status)
                    return status;
                if (length_4byte > std::numeric_limits<std::uint32_t>::max())
                    return ErrorStatus::ReadLimitReached;
                *size = length_4byte;
                return {};
        }
    }

    static constexpr std::size_t GetLengthSize(const LengthField field)
    {
        switch (field) {
            case 0:
                return 0U;
            case 1:
                return 1U;
            case 2:
                return 2U;
            default:
                return 4U;
        }
    }
};

}  // namespace serialization
}  // namespace para

#endif  // PARA_SERIALIZATION_BASE_ENCODING_HELPER_H_