// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef PARA_SERIALIZATION_BASE_MAP_H_
#define PARA_SERIALIZATION_BASE_MAP_H_

#include <map>
#include <numeric>
#include <unordered_map>

#include "ara/core/map.h"
#include "para/serialization/base/encoding.h"
#include "para/serialization/base/encoding_helper.h"

namespace para
{
namespace serialization
{
// std::map<Key, T>, std::unordered_map<Key, T> and ara::core::Map<Key, T> encoding format:
// +----------+--------//---------+
// | UINT32:L | N KEY/VALUE PAIRS |
// +----------+--------//---------+
// Each pair must be a valid encoding of Key followed by a valid encoding of T.
template <typename Key, typename T, typename Compare, typename Allocator>
struct Encoding<std::map<Key, T, Compare, Allocator>> : EncodingIO<std::map<Key, T, Compare, Allocator>>
{
    using Type = std::map<Key, T, Compare, Allocator>;

    static std::string TypeName()
    {
        std::stringstream ss;
        ss << "map<" << Encoding<Key>::TypeName() << "," << Encoding<T>::TypeName() << ">";
        return ss.str();
    }

    static constexpr std::uint32_t SizePayload(const Type& value)
    {
        return std::accumulate(value.cbegin(), value.cend(), 0U,
                               [](const std::size_t& sum, const std::pair<Key, T>& element) {
                                   return sum + Encoding<Key>::Size(element.first) + Encoding<T>::Size(element.second);
                               });
    }

    static constexpr std::uint32_t SizePayload(const LengthField field, const Type& value)
    {
        return std::accumulate(
            value.cbegin(), value.cend(), 0U, [&](const std::size_t& sum, const std::pair<Key, T>& element) {
                return sum + Encoding<Key>::Size(field, element.first) + Encoding<T>::Size(field, element.second);
            });
    }

    static constexpr std::uint32_t Size(const Type& value)
    {
        return 4U + SizePayload(value);
    }

    static constexpr std::uint32_t Size(const LengthField field, const Type& value)
    {
        return EncodingHelper::GetLengthSize(field) + SizePayload(field, value);
    }

    template <typename Writer>
    static constexpr Status<void> WritePayload(const Type& value, Writer* writer)
    {
        // write length
        const SizeType payloadLength = SizePayload(value);
        auto status = EncodingHelper::WriteLength(payloadLength, writer);
        if (!status)
            return status;
        // write map
        for (const auto& element : value) {
            status = Encoding<Key>::Write(element.first, writer);
            if (!status)
                return status;
            status = Encoding<T>::Write(element.second, writer);
            if (!status)
                return status;
        }
        return {};
    }

    template <typename Writer>
    static constexpr Status<void> WritePayload(const LengthField field, const Endianness endian,
                                               const Alignment alignment, const Type& value, Writer* writer)
    {
        // write length
        const SizeType payloadLength = SizePayload(field, value);
        auto status = EncodingHelper::WriteLength(field, endian, payloadLength, writer);
        if (!status)
            return status;
        // write map
        for (const auto& element : value) {
            status = Encoding<Key>::Write(field, endian, 0, element.first, writer);
            if (!status)
                return status;
            status = Encoding<T>::Write(field, endian, 0, element.second, writer);
            if (!status)
                return status;
        }
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
        SizeType startPos = 0;
        SizeType endPos = 0;
        auto status = Encoding<SizeType>::Read(&length, reader);
        if (!status)
            return status;
        // read map
        value->clear();
        startPos = reader->size();
        endPos = length + reader->size();
        for (SizeType i = startPos; i < endPos; i = reader->size()) {
            std::pair<Key, T> element;
            status = Encoding<Key>::Read(&element.first, reader);
            if (!status)
                return status;
            status = Encoding<T>::Read(&element.second, reader);
            if (!status)
                return status;
            value->emplace(std::move(element));
        }
        return {};
    }

    template <typename Reader>
    static constexpr Status<void> ReadPayload(const LengthField field, const Endianness endian,
                                              const Alignment alignment, Type* value, Reader* reader)
    {
        // read length
        SizeType length = 0;
        SizeType startPos = 0;
        SizeType endPos = 0;
        auto status = EncodingHelper::ReadLength(field, endian, &length, reader);
        if (!status)
            return status;
        // read map, it shall not exists intermediate padding
        value->clear();
        startPos = reader->size();
        endPos = length + reader->size();
        for (SizeType i = startPos; i < endPos; i = reader->size()) {
            std::pair<Key, T> element;
            status = Encoding<Key>::Read(field, endian, 0, &element.first, reader);
            if (!status)
                return status;
            status = Encoding<T>::Read(field, endian, 0, &element.second, reader);
            if (!status)
                return status;
            value->emplace(std::move(element));
        }
        // skip padding
        if (alignment == 0)
            return {};
        auto read = reader->size();
        return reader->Skip(read % alignment);
    }
};

template <typename Key, typename T, typename Hash, typename KeyEqual, typename Allocator>
struct Encoding<std::unordered_map<Key, T, Hash, KeyEqual, Allocator>>
    : EncodingIO<std::unordered_map<Key, T, Hash, KeyEqual, Allocator>>
{
    using Type = std::unordered_map<Key, T, Hash, KeyEqual, Allocator>;

    static std::string TypeName()
    {
        std::stringstream ss;
        ss << "unordered_map<" << Encoding<Key>::TypeName() << "," << Encoding<Key>::TypeName() << ">";
        return ss.str();
    }

    static constexpr std::uint32_t SizePayload(const Type& value)
    {
        return std::accumulate(value.cbegin(), value.cend(), 0U,
                               [](const std::size_t& sum, const std::pair<Key, T>& element) {
                                   return sum + Encoding<Key>::Size(element.first) + Encoding<T>::Size(element.second);
                               });
    }

    static constexpr std::uint32_t SizePayload(const LengthField field, const Type& value)
    {
        return std::accumulate(
            value.cbegin(), value.cend(), 0U, [&](const std::size_t& sum, const std::pair<Key, T>& element) {
                return sum + Encoding<Key>::Size(field, element.first) + Encoding<T>::Size(field, element.second);
            });
    }

    static constexpr std::uint32_t Size(const Type& value)
    {
        return 4U + SizePayload(value);
    }

    static constexpr std::uint32_t Size(const LengthField field, const Type& value)
    {
        return EncodingHelper::GetLengthSize(field) + SizePayload(field, value);
    }

    template <typename Writer>
    static constexpr Status<void> WritePayload(const Type& value, Writer* writer)
    {
        // write length
        const SizeType payloadLength = SizePayload(value);
        auto status = EncodingHelper::WriteLength(payloadLength, writer);
        if (!status)
            return status;
        // write map
        for (const auto& element : value) {
            status = Encoding<Key>::Write(element.first, writer);
            if (!status)
                return status;
            status = Encoding<T>::Write(element.second, writer);
            if (!status)
                return status;
        }
        return {};
    }

    template <typename Writer>
    static constexpr Status<void> WritePayload(const LengthField field, const Endianness endian,
                                               const Alignment alignment, const Type& value, Writer* writer)
    {
        // write length
        const SizeType payloadLength = SizePayload(field, value);
        auto status = EncodingHelper::WriteLength(field, endian, payloadLength, writer);
        if (!status)
            return status;
        // write map
        for (const auto& element : value) {
            status = Encoding<Key>::Write(field, endian, 0, element.first, writer);
            if (!status)
                return status;
            status = Encoding<T>::Write(field, endian, 0, element.second, writer);
            if (!status)
                return status;
        }
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
        SizeType startPos = 0;
        SizeType endPos = 0;
        auto status = Encoding<SizeType>::Read(&length, reader);
        if (!status)
            return status;
        // read map
        value->clear();
        startPos = reader->size();
        endPos = length + reader->size();
        for (SizeType i = startPos; i < endPos; i = reader->size()) {
            std::pair<Key, T> element;
            status = Encoding<Key>::Read(&element.first, reader);
            if (!status)
                return status;
            status = Encoding<T>::Read(&element.second, reader);
            if (!status)
                return status;
            value->emplace(std::move(element));
        }
        return {};
    }

    template <typename Reader>
    static constexpr Status<void> ReadPayload(const LengthField field, const Endianness endian,
                                              const Alignment alignment, Type* value, Reader* reader)
    {
        // read length
        SizeType length = 0;
        SizeType startPos = 0;
        SizeType endPos = 0;
        auto status = EncodingHelper::ReadLength(field, endian, &length, reader);
        if (!status)
            return status;
        // read map, it shall not exists intermediate padding
        value->clear();
        startPos = reader->size();
        endPos = length + reader->size();
        for (SizeType i = startPos; i < endPos; i = reader->size()) {
            std::pair<Key, T> element;
            status = Encoding<Key>::Read(field, endian, 0, &element.first, reader);
            if (!status)
                return status;
            status = Encoding<T>::Read(field, endian, 0, &element.second, reader);
            if (!status)
                return status;
            value->emplace(std::move(element));
        }
        // skip padding
        if (alignment == 0)
            return {};
        auto read = reader->size();
        return reader->Skip(read % alignment);
    }
};

}  // namespace serialization
}  // namespace para

#endif  // PARA_SERIALIZATION_BASE_MAP_H_