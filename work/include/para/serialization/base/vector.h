// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef PARA_SERIALIZATION_BASE_VECTOR_H_
#define PARA_SERIALIZATION_BASE_VECTOR_H_

#include <numeric>
#include <vector>

#include "ara/core/vector.h"
#include "para/serialization/base/encoding.h"
#include "para/serialization/base/encoding_helper.h"
#include "para/serialization/base/utility.h"

namespace para
{
namespace serialization
{

template <typename T>
using EnableIfBoolOrNotArithmetic = typename std::enable_if_t<std::is_same<T, bool>::value || !IsArithmetic<T>::value>;

template <typename T>
using EnableIfNotBoolAndArithmetic = typename std::enable_if_t<!std::is_same<T, bool>::value && IsArithmetic<T>::value>;

// std::vector<T>, ara::core::Vector<T> encoding format for non-arthmetic or bool types:
// +----------+-----//-----+
// | UINT32:L | N ELEMENTS |
// +----------+-----//-----+
// Elements must be valid encodings of type T.
template <typename T, typename Allocator>
struct Encoding<std::vector<T, Allocator>, EnableIfBoolOrNotArithmetic<T>> : EncodingIO<std::vector<T, Allocator>>
{
    using Type = std::vector<T, Allocator>;

    static std::string TypeName()
    {
        std::stringstream ss;
        ss << "vec<" << Encoding<T>::TypeName() << ">";
        return ss.str();
    }

    static constexpr std::size_t SizePayload(const Type& value)
    {
        return std::accumulate(value.cbegin(), value.cend(), 0U, [](const std::size_t& sum, const T& element) {
            return sum + Encoding<T>::Size(element);
        });
    }

    static constexpr std::size_t SizePayload(const LengthField field, const Type& value)
    {
        return std::accumulate(value.cbegin(), value.cend(), 0U, [&](const std::size_t& sum, const T& element) {
            return sum + Encoding<T>::Size(field, element);
        });
    }

    static constexpr std::size_t Size(const Type& value)
    {
        return 4U + SizePayload(value);
    }

    static constexpr std::size_t Size(const LengthField field, const Type& value)
    {
        return EncodingHelper::GetLengthSize(field) + SizePayload(field, value);
    }

    template <typename Writer>
    static constexpr Status<void> WritePayload(const Type& value, Writer* writer)
    {
        // write length
        const SizeType length = SizePayload(value);
        auto status = EncodingHelper::WriteLength(length, writer);
        if (!status)
            return status;
        // write data
        for (const T& element : value) {
            status = Encoding<T>::Write(element, writer);
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
        const SizeType length = SizePayload(field, value);
        auto status = EncodingHelper::WriteLength(field, endian, length, writer);
        if (!status)
            return status;
        // write data
        for (const T& element : value) {
            status = Encoding<T>::Write(field, endian, alignment, element, writer);
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
        // read data
        value->clear();
        startPos = reader->size();
        endPos = length + reader->size();
        for (SizeType i = startPos; i < endPos; i = reader->size()) {
            T element;
            status = Encoding<T>::Read(&element, reader);
            if (!status)
                return status;
            value->push_back(std::move(element));
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
        // read data
        value->clear();
        startPos = reader->size();
        endPos = length + reader->size();
        for (SizeType i = startPos; i < endPos; i = reader->size()) {
            T element;
            status = Encoding<T>::Read(field, endian, alignment, &element, reader);
            if (!status)
                return status;
            value->push_back(std::move(element));
        }
        // skip padding
        if (alignment == 0)
            return {};
        auto read = reader->size();
        return reader->Skip(read % alignment);
    }
};

// std::vector<T>, ara::core::Vector<T> encoding format for arithmetic (and non-bool) types:
// +----------+---//----+
// | UINT32:L | L ByteS |
// +----------+---//----+
// Where L = N * sizeof(T).
// Elements are stored as direct little-endian representation of the integral value; each element is sizeof(T) bytes in
// size.
template <typename T, typename Allocator>
struct Encoding<std::vector<T, Allocator>, EnableIfNotBoolAndArithmetic<T>> : EncodingIO<std::vector<T, Allocator>>
{
    using Type = std::vector<T, Allocator>;

    static std::string TypeName()
    {
        std::stringstream ss;
        ss << "vec<" << Encoding<T>::TypeName() << ">";
        return ss.str();
    }

    static constexpr std::size_t SizePayload(const Type& value)
    {
        return value.size() * Encoding<T>::Size();
    }

    static constexpr std::size_t SizePayload(const LengthField field, const Type& value)
    {
        return SizePayload(value);
    }

    static constexpr std::size_t Size(const Type& value)
    {
        return 4U + SizePayload(value);
    }

    static constexpr std::size_t Size(const LengthField field, const Type& value)
    {
        return EncodingHelper::GetLengthSize(field) + SizePayload(value);
    }

    template <typename Writer>
    static constexpr Status<void> WritePayload(const Type& value, Writer* writer)
    {
        // write length
        const SizeType length = SizePayload(value);
        auto status = EncodingHelper::WriteLength(length, writer);
        if (!status)
            return status;

        if (value.size() > 0) {
            auto payloadData = value.data();
            return writer->Write(payloadData, length);
        }
        return {};
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

        if (value.size() > 0) {
            if (endian) {
                for (const T& element : value) {
                    status = Encoding<T>::Write(field, endian, alignment, element, writer);
                    if (!status)
                        return status;
                }
            } else {
                auto payloadData = value.data();
                status = writer->Write(payloadData, length);
                if (!status)
                    return status;
            }
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
        auto status = EncodingHelper::ReadLength(&length, reader);
        if (!status)
            return status;
        if (length % Encoding<T>::Size() != 0)
            return ErrorStatus::InvalidContainerLength;
        // calculate length
        const SizeType items = length / Encoding<T>::Size();
        status = reader->Ensure(length);
        if (!status)
            return status;
        // read data
        value->clear();
        if (length > 0) {
            value->resize(items);
            return reader->Read(value->data(), length);
        }

        return {};
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
        if (length % sizeof(T) != 0)
            return ErrorStatus::InvalidContainerLength;
        // calculate length
        const SizeType items = length / sizeof(T);
        status = reader->Ensure(length);
        if (!status)
            return status;
        // read data
        value->clear();
        if (length > 0) {
            value->resize(items);
            status = reader->Read(value->data(), length);
            if (!status)
                return status;

            if (endian)
                utility::HostEndian<Type>::FromBig(*value);
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

#endif  // PARA_SERIALIZATION_BASE_VECTOR_H_