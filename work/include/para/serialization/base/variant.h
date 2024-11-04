// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef PARA_SERIALIZATION_BASE_VARIANT_H_
#define PARA_SERIALIZATION_BASE_VARIANT_H_

#include <sstream>

#include "ara/core/variant.h"
#include "para/serialization/base/encoding.h"
#include "para/serialization/base/encoding_helper.h"

namespace para
{
namespace serialization
{
// ara::core::EmptyVariant encoding format:
// +----------+-------------+
// | UINT32:L | INT32:I = 0 |
// +----------+-------------+
template <>
struct Encoding<ara::core::EmptyVariant> : EncodingIO<ara::core::EmptyVariant>
{
    using Type = ara::core::EmptyVariant;

    static constexpr std::size_t Size(Type /*value*/)
    {
        return 4U;
    }

    static constexpr std::size_t Size(const LengthField field, Type /*value*/)
    {
        return EncodingHelper::GetLengthSize(field);
    }

    template <typename Writer>
    static constexpr Status<void> WritePayload(Type /*value*/, Writer* /*writer*/)
    {
        return {};
    }

    template <typename Writer>
    static constexpr Status<void> WritePayload(const LengthField /*field*/, const Endianness /*endian*/,
                                               const Alignment /*alignment*/, Type /*value*/, Writer* /*writer*/)
    {
        return {};
    }

    template <typename Reader>
    static constexpr Status<void> ReadPayload(Type* /*value*/, Reader* /*reader*/)
    {
        return {};
    }

    template <typename Reader>
    static constexpr Status<void> ReadPayload(const LengthField /*field*/, const Endianness /*endian*/,
                                              const Alignment /*alignment*/, Type* /*value*/, Reader* /*reader*/)
    {
        return {};
    }
};

// ara::core::Variant<Ts...> encoding format:
// +----------+---------+----//-----+
// | UINT32:L | INT32:I | ELEMENT I |
// +----------+---------+----//-----+
// Elements are expected to be valid encodings for their element type.
template <typename... Ts>
struct Encoding<ara::core::Variant<Ts...>> : EncodingIO<ara::core::Variant<Ts...>>
{
    using Type = ara::core::Variant<Ts...>;

    static constexpr std::int32_t Offset = 1U;

    template <typename Last>
    static std::string TypeNameImpl()
    {
        return Encoding<Last>::TypeName();
    }

    template <typename First, typename Second, typename... Rest>
    static std::string TypeNameImpl()
    {
        std::stringstream ss;
        ss << Encoding<First>::TypeName() << ",";
        ss << TypeNameImpl<Second, Rest...>();

        return ss.str();
    }

    static std::string TypeName()
    {
        std::stringstream ss;
        ss << "ara::core::Variant<";
        ss << TypeNameImpl<Ts...>();
        ss << ">";

        return ss.str();
    }

    static constexpr std::size_t Size(const Type& value)
    {
        const SizeType lengthField = 4U;
        return Encoding<std::int32_t>::Size(value.Visit([](const auto& element) {
            using Element = typename std::decay<decltype(element)>::type;
            SizeType length = Encoding<Element>::Size(element);
            SizeType padding = 0;
            SizeType totalBytes = 0;
            if (length < lengthField) {
                padding = lengthField - length;
            } else {
                padding = length % lengthField;
            }
            // add length field + type selector field
            totalBytes = length + padding + lengthField + lengthField;
            return totalBytes;
        }));
    }

    static constexpr std::size_t Size(const LengthField field, const Type& value)
    {
        const SizeType lengthField = EncodingHelper::GetLengthSize(field);
        return Encoding<std::int32_t>::Size(value.Visit([&](const auto& element) {
            using Element = typename std::decay<decltype(element)>::type;
            SizeType length = Encoding<Element>::Size(field, element);
            SizeType padding = 0;
            SizeType totalBytes = 0;
            if (length < lengthField) {
                padding = lengthField - length;
            } else {
                padding = length % lengthField;
            }
            // add length field + type selector field
            totalBytes = length + padding + lengthField + lengthField;
            return totalBytes;
        }));
    }

    template <typename Writer>
    static constexpr Status<void> WritePayload(const Type& value, Writer* writer)
    {
        // write length
        SizeType padding = 0;
        SizeType lengthField = 4U;
        auto status = value.Visit([&](const auto& element) {
            using Element = typename std::decay<decltype(element)>::type;
            SizeType length = Encoding<Element>::Size(element);
            SizeType writeLength = 0;
            if (length < lengthField) {
                padding = lengthField - length;
            } else {
                padding = length % lengthField;
            }
            writeLength = length + padding;
            return Encoding<SizeType>::Write(writeLength, writer);
        });
        if (!status)
            return status;
        // write type selector
        // If Variant is empty, then index is -1 but index = -1 is fixed as 0 in SOME/IP payload.
        // i.e. needs to add 1 to real index on communication
        status = Encoding<std::int32_t>::Write(value.index() + Offset, writer);
        if (!status)
            return status;
        // write element
        status = value.Visit([&](const auto& element) {
            using Element = typename std::decay<decltype(element)>::type;
            return Encoding<Element>::Write(element, writer);
        });
        if (!status)
            return status;
        // add padding
        return writer->Skip(padding);
    }

    template <typename Writer>
    static constexpr Status<void> WritePayload(const LengthField field, const Endianness endian,
                                               const Alignment /*alignment*/, const Type& value, Writer* writer)
    {
        // write length
        SizeType padding = 0;
        SizeType lengthField = EncodingHelper::GetLengthSize(field);
        auto status = value.Visit([&](const auto& element) {
            using Element = typename std::decay<decltype(element)>::type;
            SizeType length = Encoding<Element>::Size(field, element);
            SizeType writeLength = 0;
            if (length < lengthField) {
                padding = lengthField - length;
            } else {
                padding = length % lengthField;
            }
            writeLength = length + padding;
            return EncodingHelper::WriteLength(field, endian, writeLength, writer);
        });
        if (!status)
            return status;
        // write type selector
        // If Variant is empty, then index is -1 but index = -1 is fixed as 0 in SOME/IP payload.
        // i.e. needs to add 1 to real index on communication
        status = EncodingHelper::WriteLength(field, endian, value.index() + Offset, writer);
        if (!status)
            return status;
        // write element
        status = value.Visit([&](const auto& element) {
            using Element = typename std::decay<decltype(element)>::type;
            return Encoding<Element>::Write(field, endian, 0, element, writer);
        });
        if (!status)
            return status;
        // add padding
        if (lengthField == 0)
            return {};
        return writer->Skip(padding);
    }

    template <typename Reader>
    static constexpr Status<void> ReadPayload(Type* value, Reader* reader)
    {
        SizeType length = 0;
        SizeType endPos = 0;
        std::int32_t typeSelector = 0;
        // read length
        auto status = Encoding<SizeType>::Read(&length, reader);
        if (!status)
            return status;
        // read type selector
        // If Variant is empty, then index is -1 but index = -1 is fixed as 0 in SOME/IP payload.
        // i.e. needs to subtract 1 to real index on communication
        status = Encoding<std::int32_t>::Read(&typeSelector, reader);
        typeSelector -= Offset;
        if (!status) {
            return status;
        } else if (typeSelector < Type::kEmptyIndex || typeSelector >= static_cast<std::int32_t>(sizeof...(Ts))) {
            return ErrorStatus::UnexpectedVariantType;
        }
        if (typeSelector > -1) {
            endPos = length + reader->size();
        }
        // read element
        value->Become(typeSelector);
        status = value->Visit([&](auto&& element) {
            using Element = typename std::decay<decltype(element)>::type;
            return Encoding<Element>::Read(&element, reader);
        });
        if (!status)
            return status;
        // skip padding by variant native
        if (typeSelector == -1)
            return {};
        SizeType padding = 0;
        if (endPos > reader->size()) {
            padding = endPos - reader->size();
            return reader->Skip(padding);
        }
        return {};
    }

    template <typename Reader>
    static constexpr Status<void> ReadPayload(const LengthField field, const Endianness endian,
                                              const Alignment /*alignment*/, Type* value, Reader* reader)
    {
        SizeType length = 0;
        SizeType type = 0;
        SizeType endPos = 0;
        std::int32_t typeSelector = 0;
        // read length
        auto status = EncodingHelper::ReadLength(field, endian, &length, reader);
        if (!status)
            return status;
        // read type selector
        // If Variant is empty, then index is -1 but index = -1 is fixed as 0 in SOME/IP payload.
        // i.e. needs to subtract 1 to real index on communication
        status = EncodingHelper::ReadLength(field, endian, &type, reader);
        typeSelector = static_cast<std::int32_t>(type) - Offset;
        if (!status) {
            return status;
        } else if (typeSelector < Type::kEmptyIndex || typeSelector >= static_cast<std::int32_t>(sizeof...(Ts))) {
            return ErrorStatus::UnexpectedVariantType;
        }
        if (typeSelector > -1) {
            endPos = length + reader->size();
        }
        // read element
        value->Become(typeSelector);
        status = value->Visit([&](auto&& element) {
            using Element = typename std::decay<decltype(element)>::type;
            return Encoding<Element>::Read(field, endian, 0, &element, reader);
        });
        if (!status) {
            return status;
        }
        // skip padding
        if (typeSelector == -1) {
            return {};
        }
        SizeType padding = 0;
        if (endPos > reader->size()) {
            padding = endPos - reader->size();
            return reader->Skip(padding);
        }
        return {};
    }
};

}  // namespace serialization
}  // namespace para

#endif  // PARA_SERIALIZATION_BASE_VARIANT_H_