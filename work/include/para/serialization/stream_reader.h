// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef PARA_SERIALIZATION_STREAM_READER_H_
#define PARA_SERIALIZATION_STREAM_READER_H_

#include <cstdint>
#include <istream>

#include "para/serialization/base/status.h"

namespace para
{
namespace serialization
{
// Reader template type that wraps STL input streams.
// Implements the basic reader interface on top of an STL input stream type.
template <typename IStream>
class StreamReader
{
  public:
    using CharType = typename IStream::char_type;

    template <typename... Args>
    StreamReader(Args&&... args) : mStream{std::forward<Args>(args)...}
    {}

    StreamReader(const StreamReader&) = default;
    StreamReader& operator=(const StreamReader&) = default;

    Status<void> Ensure(std::size_t /*size*/)
    {
        return {};
    }

    Status<void> Read(std::uint8_t* byte)
    {
        mStream.read(reinterpret_cast<CharType*>(byte), sizeof(std::uint8_t));
        mIndex += 1U;
        return ReturnStatus();
    }

    Status<void> Read(void* begin, void* end)
    {
        CharType* begin_char = static_cast<CharType*>(begin);
        CharType* end_char = static_cast<CharType*>(end);

        const std::size_t length_bytes = std::distance(begin_char, end_char);
        mStream.read(begin_char, length_bytes);
        mIndex += length_bytes;
        return ReturnStatus();
    }

    Status<void> Read(void* ptr, std::size_t byteLength)
    {
        CharType* ptrChar = static_cast<CharType*>(ptr);

        mStream.read(ptrChar, byteLength);
        mIndex += byteLength;
        return ReturnStatus();
    }

    Status<void> Skip(std::size_t padding_bytes)
    {
        mStream.seekg(padding_bytes, std::ios_base::cur);
        mIndex += padding_bytes;
        return ReturnStatus();
    }

    const IStream& stream() const
    {
        return mStream;
    }

    IStream& stream()
    {
        return mStream;
    }

    IStream&& take()
    {
        return std::move(mStream);
    }

    std::size_t size() const
    {
        return mIndex;
    }

  private:
    Status<void> ReturnStatus()
    {
        if (mStream.bad() || mStream.eof())
            return ErrorStatus::StreamError;
        else
            return {};
    }

    IStream mStream;
    std::size_t mIndex{0};
};

}  // namespace serialization
}  // namespace para

#endif  // PARA_SERIALIZATION_STREAM_READER_H_
