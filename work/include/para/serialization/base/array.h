// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef PARA_SERIALIZATION_BASE_ARRAY_H_
#define PARA_SERIALIZATION_BASE_ARRAY_H_

#include <array>
#include <numeric>

#include "ara/core/array.h"
#include "para/serialization/base/encoding.h"
#include "para/serialization/base/utility.h"

namespace para
{
namespace serialization
{
// std::array<T, N> and T[N] encoding format for non-arithmetic types:
// +-----//-----+
// | N ELEMENTS |
// +-----//-----+
// Elements must be valid encodings of type T.
template <typename T, std::size_t Length>
struct Encoding<std::array<T, Length>, EnableIfNotArithmetic<T>> : EncodingIO<std::array<T, Length>>
{
    using Type = std::array<T, Length>;

    static std::string TypeName()
    {
        std::stringstream ss;
        ss << "arr<" << Encoding<T>::TypeName() << "," << Length << ">";
        return ss.str();
    }

    static constexpr std::size_t Size(const Type& value)
    {
        std::size_t sum = 0;
        for (std::size_t i = 0; i < Length; i++) {
            sum += Encoding<T>::Size(value[i]);
        }
        return sum;
    }

    static constexpr std::size_t Size(const LengthField field, const Type& value)
    {
        std::size_t sum = 0;
        for (std::size_t i = 0; i < Length; i++) {
            sum += Encoding<T>::Size(field, value[i]);
        }
        return sum;
    }

    template <typename Writer>
    static constexpr Status<void> WritePayload(const Type& value, Writer* writer)
    {
        for (SizeType i = 0; i < Length; i++) {
            auto status = Encoding<T>::Write(value[i], writer);
            if (!status)
                return status;
        }
        return {};
    }

    template <typename Writer>
    static constexpr Status<void> WritePayload(const LengthField field, const Endianness endian,
                                               const Alignment alignment, const Type& value, Writer* writer)
    {
        for (SizeType i = 0; i < Length; i++) {
            auto status = Encoding<T>::Write(field, endian, alignment, value[i], writer);
            if (!status)
                return status;
        }
        return {};
    }

    template <typename Reader>
    static constexpr Status<void> ReadPayload(Type* value, Reader* reader)
    {
        for (SizeType i = 0; i < Length; i++) {
            auto status = Encoding<T>::Read(&(*value)[i], reader);
            if (!status)
                return status;
        }
        return {};
    }

    template <typename Reader>
    static constexpr Status<void> ReadPayload(const LengthField field, const Endianness endian,
                                              const Alignment alignment, Type* value, Reader* reader)
    {
        for (SizeType i = 0; i < Length; i++) {
            auto status = Encoding<T>::Read(field, endian, alignment, &(*value)[i], reader);
            if (!status)
                return status;
        }
        return {};
    }
};

template <typename T, std::size_t Length>
struct Encoding<T[Length], EnableIfNotArithmetic<T>> : EncodingIO<T[Length]>
{
    using Type = T[Length];

    static std::string TypeName()
    {
        std::stringstream ss;
        ss << "arr<" << Encoding<T>::TypeName() << "," << Length << ">";
        return ss.str();
    }

    static constexpr std::size_t Size(const Type& value)
    {
        std::size_t sum = 0;
        for (std::size_t i = 0; i < Length; i++) {
            sum += Encoding<T>::Size(value[i]);
        }
        return sum;
    }

    static constexpr std::size_t Size(const LengthField field, const Type& value)
    {
        std::size_t sum = 0;
        for (std::size_t i = 0; i < Length; i++) {
            sum += Encoding<T>::Size(field, value[i]);
        }
        return sum;
    }

    template <typename Writer>
    static constexpr Status<void> WritePayload(const Type& value, Writer* writer)
    {
        for (SizeType i = 0; i < Length; i++) {
            auto status = Encoding<T>::Write(value[i], writer);
            if (!status)
                return status;
        }
        return {};
    }

    template <typename Writer>
    static constexpr Status<void> WritePayload(const LengthField field, const Endianness endian,
                                               const Alignment alignment, const Type& value, Writer* writer)
    {
        for (SizeType i = 0; i < Length; i++) {
            auto status = Encoding<T>::Write(field, endian, alignment, value[i], writer);
            if (!status)
                return status;
        }
        return {};
    }

    template <typename Reader>
    static constexpr Status<void> ReadPayload(Type* value, Reader* reader)
    {
        for (SizeType i = 0; i < Length; i++) {
            auto status = Encoding<T>::Read(&(*value)[i], reader);
            if (!status)
                return status;
        }
        return {};
    }

    template <typename Reader>
    static constexpr Status<void> ReadPayload(const LengthField field, const Endianness endian,
                                              const Alignment alignment, Type* value, Reader* reader)
    {
        for (SizeType i = 0; i < Length; i++) {
            auto status = Encoding<T>::Read(field, endian, alignment, &(*value)[i], reader);
            if (!status)
                return status;
        }
        return {};
    }
};

// std::array<T, N> and T[N] encoding format for arithmetic types:
// +---//----+
// | L ByteS |
// +---//----+
// Where L = N * sizeof(T).
// Elements are stored as direct little-endian representation of the arithmetic value, each element is sizeof(T) bytes
// in size.
template <typename T, std::size_t Length>
struct Encoding<std::array<T, Length>, EnableIfArithmetic<T>> : EncodingIO<std::array<T, Length>>
{
    using Type = std::array<T, Length>;

    static std::string TypeName()
    {
        std::stringstream ss;
        ss << "arr<" << Encoding<T>::TypeName() << "," << Length << ">";
        return ss.str();
    }

    static constexpr std::size_t Size(const Type& /*value*/)
    {
        return Length * Encoding<T>::Size();
    }

    static constexpr std::size_t Size(const LengthField /*field*/, const Type& /*value*/)
    {
        return Length * Encoding<T>::Size();
    }

    template <typename Writer>
    static constexpr Status<void> WritePayload(const Type& value, Writer* writer)
    {
        return writer->Write(value.begin(), value.end());
    }

    template <typename Writer>
    static constexpr Status<void> WritePayload(const LengthField field, const Endianness endian,
                                               const Alignment alignment, const Type& value, Writer* writer)
    {
        if (endian) {
            for (SizeType i = 0; i < Length; i++) {
                auto status = Encoding<T>::Write(field, endian, alignment, value[i], writer);
                if (!status)
                    return status;
            }
            return {};
        } else {
            return writer->Write(value.begin(), value.end());
        }
    }

    template <typename Reader>
    static constexpr Status<void> ReadPayload(Type* value, Reader* reader)
    {
        return reader->Read(&(*value)[0], &(*value)[Length]);
    }

    template <typename Reader>
    static constexpr Status<void> ReadPayload(const LengthField field, const Endianness endian,
                                              const Alignment alignment, Type* value, Reader* reader)
    {
        auto status = reader->Read(&(*value)[0], Length * Encoding<T>::Size());
        if (!status)
            return status;

        if (endian)
            utility::HostEndian<Type>::FromBig(*value);

        return status;
    }
};

template <typename T, std::size_t Length>
struct Encoding<T[Length], EnableIfArithmetic<T>> : EncodingIO<T[Length]>
{
    using Type = T[Length];

    static std::string TypeName()
    {
        std::stringstream ss;
        ss << "arr<" << Encoding<T>::TypeName() << "," << Length << ">";
        return ss.str();
    }

    static constexpr std::size_t Size(const Type& /*value*/)
    {
        return Length * Encoding<T>::Size();
    }

    static constexpr std::size_t Size(const LengthField /*field*/, const Type& value)
    {
        return Length * Encoding<T>::Size();
    }

    template <typename Writer>
    static constexpr Status<void> WritePayload(const Type& value, Writer* writer)
    {
        return writer->Write(&value[0], &value[Length]);
    }

    template <typename Writer>
    static constexpr Status<void> WritePayload(const LengthField field, const Endianness endian,
                                               const Alignment alignment, const Type& value, Writer* writer)
    {
        if (endian) {
            for (SizeType i = 0; i < Length; i++) {
                auto status = Encoding<T>::Write(field, endian, alignment, value[i], writer);
                if (!status)
                    return status;
            }
            return {};
        } else {
            return writer->Write(&value[0], &value[Length]);
        }
    }

    template <typename Reader>
    static constexpr Status<void> ReadPayload(Type* value, Reader* reader)
    {
        return reader->Read(&(*value)[0], &(*value)[Length]);
    }

    template <typename Reader>
    static constexpr Status<void> ReadPayload(const LengthField field, const Endianness endian,
                                              const Alignment alignment, Type* value, Reader* reader)
    {
        auto status = reader->Read(&(*value)[0], Length * Encoding<T>::Size());
        if (!status)
            return status;

        if (endian)
            utility::HostEndian<Type>::FromBig(*value);

        return status;
    }
};

// ara::core::Array<T, N> and T[N] encoding format for non-arithmetic types:
// +-----//-----+
// | N ELEMENTS |
// +-----//-----+
// Elements must be valid encodings of type T.
template <typename T, std::size_t Length>
struct Encoding<ara::core::Array<T, Length>, EnableIfNotArithmetic<T>> : EncodingIO<ara::core::Array<T, Length>>
{
    using Type = ara::core::Array<T, Length>;

    static std::string TypeName()
    {
        std::stringstream ss;
        ss << "arr<" << Encoding<T>::TypeName() << "," << Length << ">";
        return ss.str();
    }

    static constexpr std::size_t Size(const Type& value)
    {
        std::size_t sum = 0;
        for (std::size_t i = 0; i < Length; i++) {
            sum += Encoding<T>::Size(value[i]);
        }
        return sum;
    }

    static constexpr std::size_t Size(const LengthField field, const Type& value)
    {
        std::size_t sum = 0;
        for (std::size_t i = 0; i < Length; i++) {
            sum += Encoding<T>::Size(field, value[i]);
        }
        return sum;
    }

    template <typename Writer>
    static constexpr Status<void> WritePayload(const Type& value, Writer* writer)
    {
        for (SizeType i = 0; i < Length; i++) {
            auto status = Encoding<T>::Write(value[i], writer);
            if (!status)
                return status;
        }
        return {};
    }

    template <typename Writer>
    static constexpr Status<void> WritePayload(const LengthField field, const Endianness endian,
                                               const Alignment alignment, const Type& value, Writer* writer)
    {
        for (SizeType i = 0; i < Length; i++) {
            auto status = Encoding<T>::Write(field, endian, alignment, value[i], writer);
            if (!status)
                return status;
        }
        return {};
    }

    template <typename Reader>
    static constexpr Status<void> ReadPayload(Type* value, Reader* reader)
    {
        for (SizeType i = 0; i < Length; i++) {
            auto status = Encoding<T>::Read(&(*value)[i], reader);
            if (!status)
                return status;
        }
        return {};
    }

    template <typename Reader>
    static constexpr Status<void> ReadPayload(const LengthField field, const Endianness endian,
                                              const Alignment alignment, Type* value, Reader* reader)
    {
        for (SizeType i = 0; i < Length; i++) {
            auto status = Encoding<T>::Read(field, endian, alignment, &(*value)[i], reader);
            if (!status)
                return status;
        }
        return {};
    }
};

// ara::core::Array<T, N> and T[N] encoding format for arithmetic types:
// +---//----+
// | L ByteS |
// +---//----+
// Where L = N * sizeof(T).
// Elements are stored as direct little-endian representation of the arithmetic value, each element is sizeof(T) bytes
// in size.
template <typename T, std::size_t Length>
struct Encoding<ara::core::Array<T, Length>, EnableIfArithmetic<T>> : EncodingIO<ara::core::Array<T, Length>>
{
    using Type = ara::core::Array<T, Length>;

    static std::string TypeName()
    {
        std::stringstream ss;
        ss << "arr<" << Encoding<T>::TypeName() << "," << Length << ">";
        return ss.str();
    }

    static constexpr std::size_t Size(const Type& /*value*/)
    {
        return Length * Encoding<T>::Size();
    }

    static constexpr std::size_t Size(const LengthField /*field*/, const Type& /*value*/)
    {
        return Length * Encoding<T>::Size();
    }

    template <typename Writer>
    static constexpr Status<void> WritePayload(const Type& value, Writer* writer)
    {
        return writer->Write(value.begin(), value.end());
    }

    template <typename Writer>
    static constexpr Status<void> WritePayload(const LengthField field, const Endianness endian,
                                               const Alignment alignment, const Type& value, Writer* writer)
    {
        if (endian) {
            for (SizeType i = 0; i < Length; i++) {
                auto status = Encoding<T>::Write(field, endian, alignment, value[i], writer);
                if (!status)
                    return status;
            }
            return {};
        } else {
            return writer->Write(value.begin(), value.end());
        }
    }

    template <typename Reader>
    static constexpr Status<void> ReadPayload(Type* value, Reader* reader)
    {
        return reader->Read(&(*value)[0], &(*value)[Length]);
    }

    template <typename Reader>
    static constexpr Status<void> ReadPayload(const LengthField field, const Endianness endian,
                                              const Alignment alignment, Type* value, Reader* reader)
    {
        auto status = reader->Read(&(*value)[0], Length * Encoding<T>::Size());
        if (!status)
            return status;

        if (endian)
            utility::HostEndian<Type>::FromBig(*value);

        return status;
    }
};

}  // namespace serialization
}  // namespace para

#endif  // PARA_SERIALIZATION_BASE_ARRAY_H_