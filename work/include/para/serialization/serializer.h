// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef PARA_SERIALIZATION_SERIALIZER_H_
#define PARA_SERIALIZATION_SERIALIZER_H_

#include <cstring>
#include <sstream>

#include "para/serialization/base/array.h"
#include "para/serialization/base/encoding.h"
#include "para/serialization/base/enum.h"
#include "para/serialization/base/map.h"
#include "para/serialization/base/members.h"
#include "para/serialization/base/optional.h"
#include "para/serialization/base/serializer_impl.h"
#include "para/serialization/base/stream.h"
#include "para/serialization/base/string.h"
#include "para/serialization/base/structure.h"
#include "para/serialization/base/variant.h"
#include "para/serialization/base/vector.h"
#include "para/serialization/stream_reader.h"
#include "para/serialization/stream_writer.h"

namespace para
{
namespace serializer
{

using LengthField = para::serialization::LengthField;
using Endianness = para::serialization::Endianness;
using Alignment = para::serialization::Alignment;

class Serializer
{
  public:
    using Writer = para::serialization::StreamWriter<para::serialization::ostringstream>;

    /// @brief Write data from parameter. This function supports only littel-endian.
    template <typename D>
    para::serialization::Status<void> write(const D& data)
    {
        return mSerializer.Write(data);
    }

    /// @brief Write data from parameter with special configuration
    template <typename D>
    para::serialization::Status<void> write(const LengthField lenfield, const Endianness endian,
                                            const Alignment alignment, const D& data)
    {
        return mSerializer.Write(lenfield, endian, alignment, data);
    }

    template <typename D>
    static std::string getTypeName()
    {
        return para::serialization::SerializerCommon::GetTypeName<D>();
    }

    template <typename D>
    static constexpr size_t getDataSize(const D& data)
    {
        return para::serialization::SerializerCommon::GetDataSize(data);
    }

    template <typename D>
    static constexpr size_t getDataSize(const LengthField lengthField, const D& data)
    {
        return para::serialization::SerializerCommon::GetDataSize(lengthField, data);
    }

    inline size_t size() const
    {
        return mSerializer.writer().size();
    }

    /// @brief Return stream on std::string
    std::string take()
    {
        std::string str{mSerializer.writer().stream().strbuffer(), mSerializer.writer().size()};
        return str;
    }

    /// @brief Return stream on byte vector
    std::vector<std::uint8_t> ensure()
    {
        std::vector<std::uint8_t> stream(mSerializer.writer().size());
        std::memcpy(stream.data(), mSerializer.writer().stream().strbuffer(), mSerializer.writer().size());
        return stream;
    }

  private:
    para::serialization::SerializerImpl<Writer> mSerializer;
};

class Deserializer
{
  public:
    using Reader = para::serialization::StreamReader<para::serialization::imemstream>;

    /// @brief ctor of Deserializer with std::string stream
    Deserializer(const std::string& stream)
        : mBuffer(std::move(AllocateBuffer(stream))),
          mBufferPtr(reinterpret_cast<const char*>(mBuffer.get())),
          mBufferSize(stream.length()),
          mDeserializer(mBufferPtr, mBufferSize)
    {}

    /// @brief ctor of Deserializer with byte vector stream
    Deserializer(const std::vector<std::uint8_t>& stream)
        : mBuffer(std::move(AllocateBuffer(stream))),
          mBufferPtr(reinterpret_cast<const char*>(mBuffer.get())),
          mBufferSize(stream.size()),
          mDeserializer(mBufferPtr, mBufferSize)
    {}

    Deserializer(const std::vector<std::uint8_t>* stream)
        : mBuffer(std::move(AllocateBuffer(*stream))),
          mBufferPtr(reinterpret_cast<const char*>(mBuffer.get())),
          mBufferSize(stream->size()),
          mDeserializer(mBufferPtr, mBufferSize)
    {}

    /// @brief ctor of Deserializer with std::string stream (data move)
    Deserializer(std::string&& stream)
        : mStrStreamBuf(std::move(stream)),
          mBuffer(nullptr),
          mBufferPtr(reinterpret_cast<const char*>(mStrStreamBuf.data())),
          mBufferSize(mStrStreamBuf.length()),
          mDeserializer(mBufferPtr, mBufferSize)
    {}

    /// @brief ctor of Deserializer with byte vector stream (data move)
    Deserializer(std::vector<std::uint8_t>&& stream)
        : mVecStreamBuf(std::move(stream)),
          mBuffer(nullptr),
          mBufferPtr(reinterpret_cast<const char*>(mVecStreamBuf.data())),
          mBufferSize(mVecStreamBuf.size()),
          mDeserializer(mBufferPtr, mBufferSize)
    {}

    Deserializer(Deserializer&& value)
        : mVecStreamBuf(std::move(value.mVecStreamBuf)),
          mStrStreamBuf(std::move(value.mStrStreamBuf)),
          mBuffer(value.mBuffer == nullptr ? std::move(value.mBuffer) : nullptr),
          mBufferPtr(value.mBufferPtr),
          mBufferSize(value.mBufferSize),
          mDeserializer(mBufferPtr, mBufferSize)
    {}

    Deserializer& operator=(Deserializer&& rhs)
    {
        mVecStreamBuf = std::move(rhs.mVecStreamBuf);
        mStrStreamBuf = std::move(rhs.mStrStreamBuf);
        mBuffer = std::move(rhs.mBuffer);
        mBufferPtr = rhs.mBufferPtr;
        mBufferSize = rhs.mBufferSize;
        mDeserializer.reader().stream().reset(mBufferPtr, mBufferSize);
        return *this;
    }

    std::unique_ptr<uint8_t[]> AllocateBuffer(const std::string& stream)
    {
        auto ptr = std::make_unique<uint8_t[]>(stream.length());
        std::memcpy(ptr.get(), stream.data(), stream.length());
        return ptr;
    }

    std::unique_ptr<uint8_t[]> AllocateBuffer(const std::vector<std::uint8_t>& stream)
    {
        auto ptr = std::make_unique<uint8_t[]>(stream.size());
        std::memcpy(ptr.get(), stream.data(), stream.size());
        return ptr;
    }

    /// @brief Get Current Stream Position
    inline std::size_t getPosition()
    {
        return mDeserializer.reader().size();
    }

    /// @brief Read data to parameter. This function supports only littel-endian.
    template <typename D>
    para::serialization::Status<void> read(D& data)
    {
        return mDeserializer.Read(&data);
    }

    /// @brief Read data to parameter with special configuration
    template <typename D>
    para::serialization::Status<void> read(const LengthField lenfield, const Endianness endian,
                                           const Alignment alignment, D& data)
    {
        return mDeserializer.Read(lenfield, endian, alignment, &data);
    }

  private:
    std::vector<uint8_t> mVecStreamBuf;
    std::string mStrStreamBuf;
    std::unique_ptr<std::uint8_t[]> mBuffer;
    const char* mBufferPtr;
    size_t mBufferSize;
    para::serialization::DeserializerImpl<Reader> mDeserializer;
};

}  // namespace serializer
}  // namespace para

#endif  // PARA_SERIALIZATION_SERIALIZER_H_
