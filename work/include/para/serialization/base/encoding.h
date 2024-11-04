// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef PARA_SERIALIZATION_BASE_ENCODING_H_
#define PARA_SERIALIZATION_BASE_ENCODING_H_

#include <cerrno>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <string>
#include <type_traits>
#include <vector>

#include "para/serialization/base/status.h"
#include "para/serialization/base/utility.h"
#include "para/utility/endian.h"

namespace para
{
namespace serialization
{
// Base size type is 4 byte
using SizeType = std::uint32_t;
// SOME/IP TLV length field type
using LengthField = std::uint8_t;
// Endianness, true = big endian, false = littel endian
using Endianness = bool;
// Alignment, bytes
using Alignment = std::uint8_t;

// Base type for all encoding templates. If type T does not have a specialization this template generates a static
// assert.
template <typename T, typename Enabled = void>
struct Encoding
{
    // Generate a clear compiler error if there is no encoding specified for a type passed to the serialization engine.
    static_assert(sizeof(T) != sizeof(T),
                  "Encoding<T> must be specilaized for type T. Make sure to include the appropriate encoder header.");
};

// Implements general IO for encoding types.
// May also be mixed-in with an Encoding<T> specialization to provide uniform access to Read/Write through the
// specilization itself.
template <typename T>
struct EncodingIO
{
    template <typename Writer>
    static constexpr Status<void> Write(const T& value, Writer* writer)
    {
        return Encoding<T>::WritePayload(value, writer);
    }

    template <typename Writer>
    static constexpr Status<void> Write(const LengthField field, const Endianness endian, const Alignment alignment,
                                        const T& value, Writer* writer)
    {
        return Encoding<T>::WritePayload(field, endian, alignment, value, writer);
    }

    template <typename Reader>
    static constexpr Status<void> Read(T* value, Reader* reader)
    {
        return Encoding<T>::ReadPayload(value, reader);
    }

    template <typename Reader>
    static constexpr Status<void> Read(const LengthField field, const Endianness endian, const Alignment alignment,
                                       T* value, Reader* reader)
    {
        return Encoding<T>::ReadPayload(field, endian, alignment, value, reader);
    }

  protected:
    template <typename As, typename From, typename Writer, typename Enabled = EnableIfArithmetic<As, From>>
    static constexpr Status<void> WriteAs(From value, Writer* writer)
    {
        As temp = static_cast<As>(value);
        return writer->Write(&temp, Encoding<As>::Size());
    }

    template <typename As, typename From, typename Writer, typename Enabled = EnableIfArithmetic<As, From>>
    static constexpr Status<void> WriteAs(const Endianness endian, From value, Writer* writer)
    {
        As temp = static_cast<As>(value);
        if (endian)
            temp = utility::HostEndian<As>::ToBig(temp);
        return writer->Write(&temp, Encoding<As>::Size());
    }

    template <typename As, typename From, typename Reader, typename Enabled = EnableIfArithmetic<As, From>>
    static constexpr Status<void> ReadAs(From* value, Reader* reader)
    {
        As temp = 0;
        auto status = reader->Read(&temp, Encoding<As>::Size());
        if (!status)
            return status;
        *value = static_cast<From>(temp);
        return {};
    }

    template <typename As, typename From, typename Reader, typename Enabled = EnableIfArithmetic<As, From>>
    static constexpr Status<void> ReadAs(const Endianness endian, From* value, Reader* reader)
    {
        As temp = 0;
        auto status = reader->Read(&temp, Encoding<As>::Size());
        if (!status)
            return status;
        *value = static_cast<From>(temp);
        if (endian)
            *value = utility::HostEndian<As>::FromBig(*value);
        return {};
    }
};

// Forwards reference types to the underlying type encoder.
template <typename T>
struct Encoding<T&&> : Encoding<T>
{};

template <typename T>
struct Encoding<const T&> : Encoding<T>
{};

// Assert that the hardware uses two's complement for signed integers.
static_assert(-1 == ~0, "One's complement hardware is not currently supported!");

// bool encoding formats:
// +------+
// | Byte |
// +------+
template <>
struct Encoding<bool> : EncodingIO<bool>
{
    static std::string TypeName()
    {
        return "bool";
    }

    static constexpr std::size_t Size()
    {
        return 1U;
    }

    static constexpr std::size_t Size(bool /*value*/)
    {
        return 1U;
    }

    static constexpr std::size_t Size(const LengthField /*field*/, bool /*value*/)
    {
        return 1U;
    }

    template <typename Writer>
    static constexpr Status<void> WritePayload(bool value, Writer* writer)
    {
        return WriteAs<bool>(value, writer);
    }

    template <typename Writer>
    static constexpr Status<void> WritePayload(const LengthField /*field*/, const Endianness endian,
                                               const Alignment /*alignment*/, bool value, Writer* writer)
    {
        return WriteAs<bool>(endian, value, writer);
    }

    template <typename Reader>
    static constexpr Status<void> ReadPayload(bool* value, Reader* writer)
    {
        return ReadAs<bool>(value, writer);
    }

    template <typename Reader>
    static constexpr Status<void> ReadPayload(const LengthField /*field*/, const Endianness endian,
                                              const Alignment /*alignment*/, bool* value, Reader* writer)
    {
        return ReadAs<bool>(endian, value, writer);
    }
};

// char encoding formats:
// +------+
// | Byte |
// +------+
template <>
struct Encoding<char> : EncodingIO<char>
{
    static std::string TypeName()
    {
        return "s8";
    }

    static constexpr std::size_t Size()
    {
        return 1U;
    }

    static constexpr std::size_t Size(char /*value*/)
    {
        return 1U;
    }

    static constexpr std::size_t Size(const LengthField /*field*/, char /*value*/)
    {
        return 1U;
    }

    template <typename Writer>
    static constexpr Status<void> WritePayload(char value, Writer* writer)
    {
        return WriteAs<std::uint8_t>(value, writer);
    }

    template <typename Writer>
    static constexpr Status<void> WritePayload(const LengthField /*field*/, const Endianness endian,
                                               const Alignment /*alignment*/, char value, Writer* writer)
    {
        return WriteAs<std::uint8_t>(endian, value, writer);
    }

    template <typename Reader>
    static constexpr Status<void> ReadPayload(char* value, Reader* reader)
    {
        return ReadAs<std::uint8_t>(value, reader);
    }

    template <typename Reader>
    static constexpr Status<void> ReadPayload(const LengthField /*field*/, const Endianness endian,
                                              const Alignment /*alignment*/, char* value, Reader* reader)
    {
        return ReadAs<std::uint8_t>(endian, value, reader);
    }
};

// std::uint8_t encoding formats:
// +------+
// | Byte |
// +------+
template <>
struct Encoding<std::uint8_t> : EncodingIO<std::uint8_t>
{
    static std::string TypeName()
    {
        return "u8";
    }

    static constexpr std::size_t Size()
    {
        return 1U;
    }

    static constexpr std::size_t Size(std::uint8_t /*value*/)
    {
        return 1U;
    }

    static constexpr std::size_t Size(const LengthField /*field*/, std::uint8_t /*value*/)
    {
        return 1U;
    }

    template <typename Writer>
    static constexpr Status<void> WritePayload(std::uint8_t value, Writer* writer)
    {
        return WriteAs<std::uint8_t>(value, writer);
    }

    template <typename Writer>
    static constexpr Status<void> WritePayload(const LengthField /*field*/, const Endianness endian,
                                               const Alignment /*alignment*/, std::uint8_t value, Writer* writer)
    {
        return WriteAs<std::uint8_t>(endian, value, writer);
    }

    template <typename Reader>
    static constexpr Status<void> ReadPayload(std::uint8_t* value, Reader* reader)
    {
        return ReadAs<std::uint8_t>(value, reader);
    }

    template <typename Reader>
    static constexpr Status<void> ReadPayload(const LengthField /*field*/, const Endianness endian,
                                              const Alignment /*alignment*/, std::uint8_t* value, Reader* reader)
    {
        return ReadAs<std::uint8_t>(endian, value, reader);
    }
};

// std::int8_t encoding formats:
// +------+
// | Byte |
// +------+
template <>
struct Encoding<std::int8_t> : EncodingIO<std::int8_t>
{
    static std::string TypeName()
    {
        return "s8";
    }

    static constexpr std::size_t Size()
    {
        return 1U;
    }

    static constexpr std::size_t Size(std::int8_t /*value*/)
    {
        return 1U;
    }

    static constexpr std::size_t Size(const LengthField /*field*/, std::int8_t /*value*/)
    {
        return 1U;
    }

    template <typename Writer>
    static constexpr Status<void> WritePayload(std::int8_t value, Writer* writer)
    {
        return WriteAs<std::int8_t>(value, writer);
    }

    template <typename Writer>
    static constexpr Status<void> WritePayload(const LengthField /*field*/, const Endianness endian,
                                               const Alignment /*alignment*/, std::int8_t value, Writer* writer)
    {
        return WriteAs<std::int8_t>(endian, value, writer);
    }

    template <typename Reader>
    static constexpr Status<void> ReadPayload(std::int8_t* value, Reader* reader)
    {
        return ReadAs<std::int8_t>(value, reader);
    }

    template <typename Reader>
    static constexpr Status<void> ReadPayload(const LengthField /*field*/, const Endianness endian,
                                              const Alignment /*alignment*/, std::int8_t* value, Reader* reader)
    {
        return ReadAs<std::int8_t>(endian, value, reader);
    }
};

// std::uint16_t encoding formats:
// +---//----+
// | 2 ByteS |
// +---//----+
template <>
struct Encoding<std::uint16_t> : EncodingIO<std::uint16_t>
{
    static std::string TypeName()
    {
        return "u16";
    }

    static constexpr std::size_t Size()
    {
        return 2U;
    }

    static constexpr std::size_t Size(std::uint16_t /*value*/)
    {
        return 2U;
    }

    static constexpr std::size_t Size(const LengthField /*field*/, std::uint16_t /*value*/)
    {
        return 2U;
    }

    template <typename Writer>
    static constexpr Status<void> WritePayload(std::uint16_t value, Writer* writer)
    {
        return WriteAs<std::uint16_t>(value, writer);
    }

    template <typename Writer>
    static constexpr Status<void> WritePayload(const LengthField /*field*/, const Endianness endian,
                                               const Alignment /*alignment*/, std::uint16_t value, Writer* writer)
    {
        return WriteAs<std::uint16_t>(endian, value, writer);
    }

    template <typename Reader>
    static constexpr Status<void> ReadPayload(std::uint16_t* value, Reader* reader)
    {
        return ReadAs<std::uint16_t>(value, reader);
    }

    template <typename Reader>
    static constexpr Status<void> ReadPayload(const LengthField /*field*/, const Endianness endian,
                                              const Alignment /*alignment*/, std::uint16_t* value, Reader* reader)
    {
        return ReadAs<std::uint16_t>(endian, value, reader);
    }
};

// std::int16_t encoding formats:
// +---//----+
// | 2 ByteS |
// +---//----+
template <>
struct Encoding<std::int16_t> : EncodingIO<std::int16_t>
{
    static std::string TypeName()
    {
        return "s16";
    }

    static constexpr std::size_t Size()
    {
        return 2U;
    }

    static constexpr std::size_t Size(std::int16_t /*value*/)
    {
        return 2U;
    }

    static constexpr std::size_t Size(const LengthField /*field*/, std::int16_t /*value*/)
    {
        return 2U;
    }

    template <typename Writer>
    static constexpr Status<void> WritePayload(std::int16_t value, Writer* writer)
    {
        return WriteAs<std::int16_t>(value, writer);
    }

    template <typename Writer>
    static constexpr Status<void> WritePayload(const LengthField /*field*/, const Endianness endian,
                                               const Alignment /*alignment*/, std::int16_t value, Writer* writer)
    {
        return WriteAs<std::int16_t>(endian, value, writer);
    }

    template <typename Reader>
    static constexpr Status<void> ReadPayload(std::int16_t* value, Reader* reader)
    {
        return ReadAs<std::int16_t>(value, reader);
    }

    template <typename Reader>
    static constexpr Status<void> ReadPayload(const LengthField /*field*/, const Endianness endian,
                                              const Alignment /*alignment*/, std::int16_t* value, Reader* reader)
    {
        return ReadAs<std::int16_t>(endian, value, reader);
    }
};

// std::uint32_t encoding formats:
// +---//----+
// | 4 ByteS |
// +---//----+
template <>
struct Encoding<std::uint32_t> : EncodingIO<std::uint32_t>
{
    static std::string TypeName()
    {
        return "u32";
    }

    static constexpr std::size_t Size()
    {
        return 4U;
    }

    static constexpr std::size_t Size(std::uint32_t /*value*/)
    {
        return 4U;
    }

    static constexpr std::size_t Size(const LengthField /*field*/, std::uint32_t /*value*/)
    {
        return 4U;
    }

    template <typename Writer>
    static constexpr Status<void> WritePayload(std::uint32_t value, Writer* writer)
    {
        return WriteAs<std::uint32_t>(value, writer);
    }

    template <typename Writer>
    static constexpr Status<void> WritePayload(const LengthField /*field*/, const Endianness endian,
                                               const Alignment /*alignment*/, std::uint32_t value, Writer* writer)
    {
        return WriteAs<std::uint32_t>(endian, value, writer);
    }

    template <typename Reader>
    static constexpr Status<void> ReadPayload(std::uint32_t* value, Reader* reader)
    {
        return ReadAs<std::uint32_t>(value, reader);
    }

    template <typename Reader>
    static constexpr Status<void> ReadPayload(const LengthField /*field*/, const Endianness endian,
                                              const Alignment /*alignment*/, std::uint32_t* value, Reader* reader)
    {
        return ReadAs<std::uint32_t>(endian, value, reader);
    }
};

// std::int32_t encoding formats:
// +---//----+
// | 4 ByteS |
// +---//----+
template <>
struct Encoding<std::int32_t> : EncodingIO<std::int32_t>
{
    static std::string TypeName()
    {
        return "s32";
    }

    static constexpr std::size_t Size()
    {
        return 4U;
    }

    static constexpr std::size_t Size(std::int32_t /*value*/)
    {
        return 4U;
    }

    static constexpr std::size_t Size(const LengthField /*field*/, std::int32_t /*value*/)
    {
        return 4U;
    }

    template <typename Writer>
    static constexpr Status<void> WritePayload(std::int32_t value, Writer* writer)
    {
        return WriteAs<std::int32_t>(value, writer);
    }

    template <typename Writer>
    static constexpr Status<void> WritePayload(const LengthField /*field*/, const Endianness endian,
                                               const Alignment /*alignment*/, std::int32_t value, Writer* writer)
    {
        return WriteAs<std::int32_t>(endian, value, writer);
    }

    template <typename Reader>
    static constexpr Status<void> ReadPayload(std::int32_t* value, Reader* reader)
    {
        return ReadAs<std::int32_t>(value, reader);
    }

    template <typename Reader>
    static constexpr Status<void> ReadPayload(const LengthField /*field*/, const Endianness endian,
                                              const Alignment /*alignment*/, std::int32_t* value, Reader* reader)
    {
        return ReadAs<std::int32_t>(endian, value, reader);
    }
};

// std::uint64_t encoding formats:
// +---//----+
// | 8 ByteS |
// +---//----+
template <>
struct Encoding<std::uint64_t> : EncodingIO<std::uint64_t>
{
    static std::string TypeName()
    {
        return "u64";
    }

    static constexpr std::size_t Size()
    {
        return 8U;
    }

    static constexpr std::size_t Size(std::uint64_t /*value*/)
    {
        return 8U;
    }

    static constexpr std::size_t Size(const LengthField /*field*/, std::uint64_t /*value*/)
    {
        return 8U;
    }

    template <typename Writer>
    static constexpr Status<void> WritePayload(std::uint64_t value, Writer* writer)
    {
        return WriteAs<std::uint64_t>(value, writer);
    }

    template <typename Writer>
    static constexpr Status<void> WritePayload(const LengthField /*field*/, const Endianness endian,
                                               const Alignment /*alignment*/, std::uint64_t value, Writer* writer)
    {
        return WriteAs<std::uint64_t>(endian, value, writer);
    }

    template <typename Reader>
    static constexpr Status<void> ReadPayload(std::uint64_t* value, Reader* reader)
    {
        return ReadAs<std::uint64_t>(value, reader);
    }

    template <typename Reader>
    static constexpr Status<void> ReadPayload(const LengthField /*field*/, const Endianness endian,
                                              const Alignment /*alignment*/, std::uint64_t* value, Reader* reader)
    {
        return ReadAs<std::uint64_t>(endian, value, reader);
    }
};

// std::int64_t encoding formats:
// +---//----+
// | 8 ByteS |
// +---//----+
template <>
struct Encoding<std::int64_t> : EncodingIO<std::int64_t>
{
    static std::string TypeName()
    {
        return "s64";
    }

    static constexpr std::size_t Size()
    {
        return 8U;
    }

    static constexpr std::size_t Size(std::int64_t /*value*/)
    {
        return 8U;
    }

    static constexpr std::size_t Size(const LengthField /*field*/, std::int64_t /*value*/)
    {
        return 8U;
    }

    template <typename Writer>
    static constexpr Status<void> WritePayload(std::int64_t value, Writer* writer)
    {
        return WriteAs<std::int64_t>(value, writer);
    }

    template <typename Writer>
    static constexpr Status<void> WritePayload(const LengthField /*field*/, const Endianness endian,
                                               const Alignment /*alignment*/, std::int64_t value, Writer* writer)
    {
        return WriteAs<std::int64_t>(endian, value, writer);
    }

    template <typename Reader>
    static constexpr Status<void> ReadPayload(std::int64_t* value, Reader* reader)
    {
        return ReadAs<std::int64_t>(value, reader);
    }

    template <typename Reader>
    static constexpr Status<void> ReadPayload(const LengthField /*field*/, const Endianness endian,
                                              const Alignment /*alignment*/, std::int64_t* value, Reader* reader)
    {
        return ReadAs<std::int64_t>(endian, value, reader);
    }
};

// float encoding format:
// +---//----+
// | 4 ByteS |
// +---//----+
template <>
struct Encoding<float> : EncodingIO<float>
{
    static std::string TypeName()
    {
        return "f32";
    }

    static constexpr std::size_t Size()
    {
        return 4U;
    }

    static constexpr std::size_t Size(float /*value*/)
    {
        return 4U;
    }

    static constexpr std::size_t Size(const LengthField /*field*/, float /*value*/)
    {
        return 4U;
    }

    template <typename Writer>
    static constexpr Status<void> WritePayload(float value, Writer* writer)
    {
        return WriteAs<float>(value, writer);
    }

    template <typename Writer>
    static constexpr Status<void> WritePayload(const LengthField /*field*/, const Endianness endian,
                                               const Alignment /*alignment*/, float value, Writer* writer)
    {
        return WriteAs<float>(endian, value, writer);
    }

    template <typename Reader>
    static constexpr Status<void> ReadPayload(float* value, Reader* reader)
    {
        return ReadAs<float>(value, reader);
    }

    template <typename Reader>
    static constexpr Status<void> ReadPayload(const LengthField /*field*/, const Endianness endian,
                                              const Alignment /*alignment*/, float* value, Reader* reader)
    {
        return ReadAs<float>(endian, value, reader);
    }
};

// double encoding format:
// +---//----+
// | 8 ByteS |
// +---//----+
template <>
struct Encoding<double> : EncodingIO<double>
{
    static std::string TypeName()
    {
        return "f64";
    }

    static constexpr std::size_t Size()
    {
        return 8U;
    }

    static constexpr std::size_t Size(double /*value*/)
    {
        return 8U;
    }

    static constexpr std::size_t Size(const LengthField /*field*/, double /*value*/)
    {
        return 8U;
    }

    template <typename Writer>
    static constexpr Status<void> WritePayload(double value, Writer* writer)
    {
        return WriteAs<double>(value, writer);
    }

    template <typename Writer>
    static constexpr Status<void> WritePayload(const LengthField /*field*/, const Endianness endian,
                                               const Alignment /*alignment*/, double value, Writer* writer)
    {
        return WriteAs<double>(endian, value, writer);
    }

    template <typename Reader>
    static constexpr Status<void> ReadPayload(double* value, Reader* reader)
    {
        return ReadAs<double>(value, reader);
    }

    template <typename Reader>
    static constexpr Status<void> ReadPayload(const LengthField /*field*/, const Endianness endian,
                                              const Alignment /*alignment*/, double* value, Reader* reader)
    {
        return ReadAs<double>(endian, value, reader);
    }
};

// std::size_t encoding format depends on the size of std::size_t for the platform. Simply forward to either
// std::uint32_t or std::uint64_t.
template <typename T>
struct Encoding<T, std::enable_if_t<std::is_same<T, std::size_t>::value &&
                                    IsUnique<std::is_same, std::size_t, std::uint32_t, std::uint64_t>::value>>
    : EncodingIO<std::size_t>
{
    // Check that std::size_t is either 32 or 64-bit.
    static_assert(sizeof(std::size_t) == sizeof(std::uint32_t) || sizeof(std::size_t) == sizeof(std::uint64_t),
                  "std::size_t must be either 32 or 64-bit!");

    using BaseType = std::conditional_t<sizeof(std::size_t) == sizeof(std::uint32_t), std::uint32_t, std::uint64_t>;

    static std::string TypeName()
    {
        return Encoding<BaseType>::TypeName();
    }

    static constexpr std::size_t Size()
    {
        return sizeof(std::size_t);
    }

    static constexpr std::size_t Size(std::size_t value)
    {
        return sizeof(std::size_t);
    }

    static constexpr std::size_t Size(const LengthField /*field*/, std::size_t value)
    {
        return sizeof(std::size_t);
    }

    template <typename Writer>
    static constexpr Status<void> WritePayload(std::size_t value, Writer* writer)
    {
        return Encoding<BaseType>::WritePayload(value, writer);
    }

    template <typename Writer>
    static constexpr Status<void> WritePayload(const LengthField field, const Endianness endian,
                                               const Alignment alignment, std::size_t value, Writer* writer)
    {
        return Encoding<BaseType>::WritePayload(field, endian, alignment, value, writer);
    }

    template <typename Reader>
    static constexpr Status<void> ReadPayload(std::size_t* value, Reader* reader)
    {
        BaseType base_value = 0;
        auto status = Encoding<BaseType>::ReadPayload(&base_value, reader);
        if (!status)
            return status;

        *value = base_value;
        return {};
    }

    template <typename Reader>
    static constexpr Status<void> ReadPayload(const LengthField field, const Endianness endian,
                                              const Alignment alignment, std::size_t* value, Reader* reader)
    {
        BaseType base_value = 0;
        auto status = Encoding<BaseType>::ReadPayload(field, endian, alignment, &base_value, reader);
        if (!status)
            return status;

        *value = base_value;
        return {};
    }
};

// Work around GCC bug that somtimes fails to match Endoding<int> to Encoding<std::int32_t>.
template <typename T>
struct Encoding<T, std::enable_if_t<std::is_same<T, int>::value && sizeof(int) == sizeof(std::int32_t)>>
    : Encoding<std::int32_t>
{};

}  // namespace serialization
}  // namespace para

#endif  // PARA_SERIALIZATION_BASE_ENCODING_H_
