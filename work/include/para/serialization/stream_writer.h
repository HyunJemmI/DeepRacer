// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef PARA_SERIALIZATION_STREAM_WRITER_H_
#define PARA_SERIALIZATION_STREAM_WRITER_H_

#include <cstdint>
#include <ostream>

#include "para/serialization/base/status.h"

namespace para
{
namespace serialization
{

// Writer template type that wraps STL output streams.
template <typename OStream>
class StreamWriter
{
  public:
    using CharType = typename OStream::char_type;

    using TBuffer = std::array<std::uint8_t, 65536>;
    using TBufItr = TBuffer::iterator;
    using diff_t = std::iterator_traits<TBufItr>::difference_type;

    template <typename... Args>
    StreamWriter(Args&&... args)
        : mStream{std::forward<Args>(args)...},
          mBuf{},
          mBeginIt{std::begin(mBuf)},
          mCurOffset{0},
          mBufferSize{mBuf.size()}
    {}

    StreamWriter(const StreamWriter&) = default;
    StreamWriter& operator=(const StreamWriter&) = default;

    Status<void> Prepare(std::size_t /*size*/)
    {
        return {};
    }

    void writeBufferToStream()
    {
        mStream.rdbuf()->sputn(reinterpret_cast<const char*>(std::addressof(*mBeginIt)),
                               static_cast<std::streamsize>(mCurOffset));
        mCurOffset = 0;
    }

    Status<void> Write(std::uint8_t byte)
    {
        const auto newOffset = mCurOffset + 1;
        if (newOffset <= mBufferSize) {
            mBuf.at(mCurOffset) = byte;
            mCurOffset = newOffset;
        } else {
            writeBufferToStream();
            mStream.rdbuf()->sputc(byte);
        }

        return ReturnStatus();
    }

    Status<void> Write(const void* begin, const void* end)
    {
        const CharType* begin_char = static_cast<const CharType*>(begin);
        const CharType* end_char = static_cast<const CharType*>(end);

        const std::size_t length_bytes = std::distance(begin_char, end_char);

        const auto newOffset = mCurOffset + length_bytes;
        if (newOffset <= mBufferSize) {
            std::copy_n(begin_char, length_bytes, mBeginIt + static_cast<diff_t>(mCurOffset));
            mCurOffset = newOffset;
        } else {
            writeBufferToStream();
            mStream.rdbuf()->sputn(begin_char, length_bytes);
        }

        mIndex += length_bytes;

        return ReturnStatus();
    }

    Status<void> Write(const void* ptr, std::size_t byteLength)
    {
        const CharType* begin_char = static_cast<const CharType*>(ptr);

        const auto newOffset = mCurOffset + byteLength;
        if (newOffset <= mBufferSize) {
            std::copy_n(begin_char, byteLength, mBeginIt + static_cast<diff_t>(mCurOffset));
            mCurOffset = newOffset;
        } else {
            writeBufferToStream();
            mStream.rdbuf()->sputn(begin_char, byteLength);
        }

        mIndex += byteLength;

        return ReturnStatus();
    }

    Status<void> Skip(std::size_t padding_bytes, std::uint8_t padding_value = 0x00)
    {
        for (std::size_t i = 0; i < padding_bytes; i++) {
            const auto newOffset = mCurOffset + 1;
            if (newOffset <= mBufferSize) {
                mBuf.at(mCurOffset) = padding_value;
                //*(mBeginIt + static_cast<diff_t>(mCurOffset)) = padding_value;
                mCurOffset = newOffset;
            } else {
                writeBufferToStream();
                mStream.rdbuf()->sputc(padding_value);
            }

            auto status = ReturnStatus();
            if (!status)
                return status;
        }
        mIndex += padding_bytes;
        return {};
    }

    const OStream& stream() const
    {
        writeBufferToStream();
        return mStream;
    }

    OStream& stream()
    {
        writeBufferToStream();
        return mStream;
    }

    OStream&& take()
    {
        writeBufferToStream();
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

    OStream mStream;
    std::size_t mIndex{0};

    TBuffer mBuf;
    TBufItr mBeginIt;
    size_t mCurOffset;
    size_t mBufferSize;
};

}  // namespace serialization
}  // namespace para

#endif  // PARA_SERIALIZATION_STREAM_WRITER_H_
