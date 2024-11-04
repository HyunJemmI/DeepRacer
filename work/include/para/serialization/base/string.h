// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef PARA_SERIALIZATION_BASE_STRING_H_
#define PARA_SERIALIZATION_BASE_STRING_H_

#include <string>

#include "ara/core/string.h"
#include "para/serialization/base/encoding.h"
#include "para/serialization/base/encoding_helper.h"

namespace para
{
namespace serialization
{

using Bom = std::array<std::uint8_t, 3>;

static constexpr Bom kBom_UTF8{0xEF, 0xBB, 0xBF};
static constexpr std::size_t BomSize = 3U;

// std::basic_string<...> encoding format:
// +----------+---//----+
// | UINT32:L | N BYTES |
// +----------+---//----+
template <typename CharType, typename Traits, typename Allocator>
struct Encoding<std::basic_string<CharType, Traits, Allocator>>
    : EncodingIO<std::basic_string<CharType, Traits, Allocator>>
{
    using Type = std::basic_string<CharType, Traits, Allocator>;

    enum : std::uint32_t
    {
        CharSize = sizeof(CharType)
    };

    static std::string TypeName()
    {
        return "stdstr";
    }

    static std::size_t SizePayload(const Type& value)
    {
        return value.length() * CharSize;
    }

    static std::size_t Size(const Type& value)
    {
        return 4U + SizePayload(value);
    }

    static std::size_t Size(const LengthField field, const Type& value)
    {
        return EncodingHelper::GetLengthSize(field) + SizePayload(value);
    }

    template <typename Writer>
    static constexpr Status<void> WritePayload(const Type& value, Writer* writer)
    {
        // write length
        const SizeType length = SizePayload(value);
        auto status = Encoding<SizeType>::Write(length, writer);
        if (!status)
            return status;
        // write string
        return writer->Write(&value[0], length);
    }

    template <typename Writer>
    static constexpr Status<void> WritePayload(const LengthField field, const Endianness endian,
                                               const Alignment alignment, const Type& value, Writer* writer)
    {
        // write length
        const SizeType length = SizePayload(value);
        auto status = EncodingHelper::WriteLength(field, endian, length, writer);
        if (!status)
            return status;
        // write string
        status = writer->Write(&value[0], length);
        if (!status)
            return status;
        // add padding
        if (alignment == 0)
            return {};
        auto written = writer->size();
        return writer->Skip(written % alignment);
    }

    template <typename Reader>
    static constexpr Status<void> ReadPayload(Type* value, Reader* reader)
    {
        // read length
        SizeType length = 0;
        auto status = Encoding<SizeType>::Read(&length, reader);
        if (!status)
            return status;
        else if (length % CharSize != 0)
            return ErrorStatus::InvalidStringLength;
        // read string
        const SizeType items = length / CharSize;
        status = reader->Ensure(items);
        if (!status)
            return status;
        value->resize(items);
        return reader->Read(&(*value)[0], items);
    }

    template <typename Reader>
    static constexpr Status<void> ReadPayload(const LengthField field, const Endianness endian,
                                              const Alignment alignment, Type* value, Reader* reader)
    {
        // read length
        SizeType length = 0;
        auto status = EncodingHelper::ReadLength(field, endian, &length, reader);
        if (!status)
            return status;
        else if (length % CharSize != 0)
            return ErrorStatus::InvalidStringLength;
        // read string
        const SizeType items = length / CharSize;
        status = reader->Ensure(items);
        if (!status)
            return status;
        value->resize(items);
        status = reader->Read(&(*value)[0], items);
        if (!status)
            return status;
        // skip padding
        if (alignment == 0)
            return {};
        auto read = reader->size();
        return reader->Skip(read % alignment);
    }
};

// ara::core::String<...> encoding format:
// +----------+-----------+---//----+------+
// | UINT32:L | UTF-8 BOM | N ByteS | '\0' |
// +----------+-----------+---//----+------+
template <>
struct Encoding<ara::core::String> : EncodingIO<ara::core::String>
{
    using Type = ara::core::String;

    enum : std::uint32_t
    {
        CharSize = sizeof(char)
    };

    static std::string TypeName()
    {
        return "str";
    }

    static std::size_t SizePayload(const Type& value)
    {
        return value.length() * CharSize + 3U + sizeof('\0');
    }

    static std::size_t Size(const Type& value)
    {
        return 4U + SizePayload(value);
    }

    static std::size_t Size(const LengthField field, const Type& value)
    {
        return EncodingHelper::GetLengthSize(field) + SizePayload(value);
    }

    template <typename Writer>
    static constexpr Status<void> WritePayload(const Type& value, Writer* writer)
    {
        // write length
        const SizeType context = value.length();
        const SizeType length = SizePayload(value);
        auto status = Encoding<SizeType>::Write(length, writer);
        if (!status)
            return status;
        // write bom (UTF-8)
        status = WriteBom(writer);
        if (!status)
            return status;
        // write string
        status = writer->Write(&value.c_str()[0], context);
        if (!status)
            return status;
        // write '\0'
        status = Encoding<char>::Write('\0', writer);
        if (!status)
            return status;
        return {};
    }

    template <typename Writer>
    static constexpr Status<void> WritePayload(const LengthField field, const Endianness endian,
                                               const Alignment alignment, const Type& value, Writer* writer)
    {
        // write length
        const SizeType context = value.length();
        const SizeType length = SizePayload(value);
        auto status = EncodingHelper::WriteLength(field, endian, length, writer);
        if (!status)
            return status;
        // write bom (UTF-8)
        status = WriteBom(writer);
        if (!status)
            return status;
        // write string
        status = writer->Write(&value.c_str()[0], context);
        if (!status)
            return status;
        // write '\0'
        status = Encoding<char>::Write('\0', writer);
        if (!status)
            return status;
        // add padding
        if (alignment == 0)
            return {};
        auto written = writer->size();
        return writer->Skip(written % alignment);
    }

    template <typename Reader>
    static constexpr Status<void> ReadPayload(Type* value, Reader* reader)
    {
        // read length
        SizeType length = 0;
        auto status = Encoding<SizeType>::Read(&length, reader);
        if (!status)
            return status;
        else if (length % CharSize != 0)
            return ErrorStatus::InvalidStringLength;
        // read bom
        status = ReadBom(reader);
        if (!status)
            return status;
        // read string
        const SizeType items = length / CharSize - BomSize;
        status = reader->Ensure(items);
        if (!status)
            return status;
        value->resize(items - 1);
        return reader->Read(&const_cast<Type::value_type*>((*value).c_str())[0], items);
    }

    template <typename Reader>
    static constexpr Status<void> ReadPayload(const LengthField field, const Endianness endian,
                                              const Alignment alignment, Type* value, Reader* reader)
    {
        // read length
        SizeType length = 0;
        auto status = EncodingHelper::ReadLength(field, endian, &length, reader);
        if (!status)
            return status;
        else if (length % CharSize != 0)
            return ErrorStatus::InvalidStringLength;
        // read bom
        status = ReadBom(reader);
        if (!status)
            return status;
        // read string, 3U is size of bom
        const SizeType items = length / CharSize - BomSize;
        status = reader->Ensure(items);
        if (!status)
            return status;

        value->resize(items - 1);
        status = reader->Read(&const_cast<Type::value_type*>((*value).c_str())[0], items);
        if (!status)
            return status;
        // skip padding
        if (alignment == 0)
            return {};
        auto read = reader->size();
        return reader->Skip(read % alignment);
    }

  private:
    template <typename Writer>
    static constexpr Status<void> WriteBom(Writer* writer)
    {
        std::array<uint8_t, 3> bomArr{0xEF, 0xBB, 0xBF};
        return writer->Write(bomArr.data(), 3);
    }

    template <typename Reader>
    static constexpr Status<void> ReadBom(Reader* reader)
    {
        Bom bom{};
        auto status = reader->Read(bom.data(), 3);
        if (!status)
            return status;
        else if (bom[0] != kBom_UTF8[0])
            return ErrorStatus::InvalidStringLength;
        else if (bom[1] != kBom_UTF8[1])
            return ErrorStatus::InvalidStringLength;
        else if (bom[2] != kBom_UTF8[2])
            return ErrorStatus::InvalidStringLength;

        return {};
    }
};

}  // namespace serialization
}  // namespace para

#endif  // PARA_SERIALIZATION_BASE_STRING_H_