// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef PARA_SERIALIZATION_BASE_SERIALIZER_IMPL_H_
#define PARA_SERIALIZATION_BASE_SERIALIZER_IMPL_H_

#include <memory>

#include "para/serialization/base/encoding.h"
#include "para/serialization/base/status.h"

namespace para
{
namespace serialization
{
// Serializer and Deserializer template types provide the basic interface for writing and reading C++ types to a writer
// or reader class. Implementation of Write method common to all Serializer specializations.
struct SerializerCommon
{
    template <typename T, typename Writer>
    static constexpr Status<void> Write(const T& value, Writer* writer)
    {
        // Determine how much space to prepare the writer for.
        const std::size_t size_bytes = Encoding<T>::Size(value);
        // Prepare the writer for the serialized data.
        auto status = writer->Prepare(size_bytes);
        if (!status)
            return status;
        // Serialize the data to the writer.
        return Encoding<T>::Write(value, writer);
    }

    template <typename T, typename Writer>
    static constexpr Status<void> Write(const LengthField field, const Endianness endian, const Alignment alignment,
                                        const T& value, Writer* writer)
    {
        // Determine how much space to prepare the writer for.
        const std::size_t size_bytes = Encoding<T>::Size(value);
        // Prepare the writer for the serialized data.
        auto status = writer->Prepare(size_bytes);
        if (!status)
            return status;
        // Serialize the data to the writer.
        return Encoding<T>::Write(field, endian, alignment, value, writer);
    }

    template <typename T>
    static std::string GetTypeName()
    {
        return Encoding<T>::TypeName();
    }

    template <typename T>
    static constexpr std::size_t GetDataSize(const T& value)
    {
        return Encoding<T>::Size(value);
    }

    template <typename T>
    static constexpr std::size_t GetDataSize(const LengthField field, const T& value)
    {
        return Encoding<T>::Size(field, value);
    }
};

// Serializer with internal instance of Writer.
template <typename Writer>
class SerializerImpl
{
  public:
    template <typename... Args>
    constexpr SerializerImpl(Args&&... args) : mWriter{std::forward<Args>(args)...}
    {}

    constexpr SerializerImpl(SerializerImpl&&) = default;
    constexpr SerializerImpl& operator=(SerializerImpl&&) = default;

    // Returns the encoded size of |value| in bytes. This may be an over estimate but must never be an under estimate.
    template <typename T>
    constexpr std::size_t GetSize(const T& value)
    {
        return Encoding<T>::Size(value);
    }

    // Serializes |value| to the Writer.
    template <typename T>
    constexpr Status<void> Write(const T& value)
    {
        return SerializerCommon::Write(value, &mWriter);
    }

    template <typename T>
    constexpr Status<void> Write(const LengthField field, const Endianness endian, const Alignment alignment,
                                 const T& value)
    {
        return SerializerCommon::Write(field, endian, alignment, value, &mWriter);
    }

    constexpr const Writer& writer() const
    {
        return mWriter;
    }

    constexpr Writer& writer()
    {
        return mWriter;
    }

    constexpr Writer&& take()
    {
        return std::move(mWriter);
    }

  private:
    Writer mWriter;

    SerializerImpl(const SerializerImpl&) = delete;
    SerializerImpl& operator=(const SerializerImpl&) = delete;
};

// Serializer that wraps a pointer to Writer.
template <typename Writer>
class SerializerImpl<Writer*>
{
  public:
    constexpr SerializerImpl() : mWriter{nullptr}
    {}

    constexpr SerializerImpl(Writer* writer) : mWriter{writer}
    {}

    constexpr SerializerImpl(const SerializerImpl&) = default;
    constexpr SerializerImpl& operator=(const SerializerImpl&) = default;

    // Returns the encoded size of |value| in bytes. This may be an over estimate but must never be an under estimate.
    template <typename T>
    constexpr std::size_t GetSize(const T& value)
    {
        return Encoding<T>::Size(value);
    }

    // Serializes |value| to the Writer.
    template <typename T>
    constexpr Status<void> Write(const T& value)
    {
        return SerializerCommon::Write(value, mWriter);
    }

    template <typename T>
    constexpr Status<void> Write(const LengthField field, const Endianness endian, const Alignment alignment,
                                 const T& value)
    {
        return SerializerCommon::Write(field, endian, alignment, value, mWriter);
    }

    constexpr const Writer& writer() const
    {
        return *mWriter;
    }

    constexpr Writer& writer()
    {
        return *mWriter;
    }

  private:
    Writer* mWriter;
};

// Serializer that wraps a unique pointer to Writer.
template <typename Writer>
class SerializerImpl<std::unique_ptr<Writer>>
{
  public:
    constexpr SerializerImpl() = default;

    constexpr SerializerImpl(std::unique_ptr<Writer> writer) : mWriter{std::move(writer)}
    {}

    constexpr SerializerImpl(SerializerImpl&&) = default;
    constexpr SerializerImpl& operator=(SerializerImpl&&) = default;

    // Returns the encoded size of |value| in bytes. This may be an over estimate but must never be an under estimate.
    template <typename T>
    constexpr std::size_t GetSize(const T& value)
    {
        return Encoding<T>::Size(value);
    }

    // Serializes |value| to the Writer.
    template <typename T>
    constexpr Status<void> Write(const T& value)
    {
        return SerializerCommon::Write(value, mWriter.get());
    }

    template <typename T>
    constexpr Status<void> Write(const LengthField field, const Endianness endian, const Alignment alignment,
                                 const T& value)
    {
        return SerializerCommon::Write(field, endian, alignment, value, mWriter.get());
    }

    constexpr const Writer& writer() const
    {
        return *mWriter;
    }

    constexpr Writer& writer()
    {
        return *mWriter;
    }

  private:
    std::unique_ptr<Writer> mWriter;

    SerializerImpl(const SerializerImpl&) = delete;
    SerializerImpl& operator=(const SerializerImpl&) = delete;
};

// Deserializer that wraps an internal instance of Reader.
template <typename Reader>
class DeserializerImpl
{
  public:
    template <typename... Args>
    constexpr DeserializerImpl(Args&&... args) : mReader{std::forward<Args>(args)...}
    {}

    constexpr DeserializerImpl(DeserializerImpl&&) = default;
    constexpr DeserializerImpl& operator=(DeserializerImpl&&) = default;

    // Deserializes the data from the reader.
    template <typename T>
    constexpr Status<void> Read(T* value)
    {
        return Encoding<T>::Read(value, &mReader);
    }

    template <typename T>
    constexpr Status<void> Read(const LengthField field, const Endianness endian, const Alignment alignment, T* value)
    {
        return Encoding<T>::Read(field, endian, alignment, value, &mReader);
    }

    constexpr const Reader& reader() const
    {
        return mReader;
    }

    constexpr Reader& reader()
    {
        return mReader;
    }

    constexpr Reader&& take()
    {
        return std::move(mReader);
    }

  private:
    Reader mReader;

    DeserializerImpl(const DeserializerImpl&) = delete;
    DeserializerImpl& operator=(const DeserializerImpl&) = delete;
};

// Deserializer that wraps a pointer to Reader.
template <typename Reader>
class DeserializerImpl<Reader*>
{
  public:
    constexpr DeserializerImpl() : mReader{nullptr}
    {}

    constexpr DeserializerImpl(Reader* reader) : mReader{reader}
    {}

    constexpr DeserializerImpl(const DeserializerImpl&) = default;
    constexpr DeserializerImpl& operator=(const DeserializerImpl&) = default;

    // Deserializes the data from the reader.
    template <typename T>
    constexpr Status<void> Read(T* value)
    {
        return Encoding<T>::Read(value, mReader);
    }

    template <typename T>
    constexpr Status<void> Read(const LengthField field, const Endianness endian, const Alignment alignment, T* value)
    {
        return Encoding<T>::Read(field, endian, alignment, value, mReader);
    }

    constexpr const Reader& reader() const
    {
        return *mReader;
    }

    constexpr Reader& reader()
    {
        return *mReader;
    }

  private:
    Reader* mReader;
};

// Deserializer that wraps a unique pointer to reader.
template <typename Reader>
class DeserializerImpl<std::unique_ptr<Reader>>
{
  public:
    constexpr DeserializerImpl() = default;

    constexpr DeserializerImpl(std::unique_ptr<Reader> reader) : mReader{std::move(reader)}
    {}

    constexpr DeserializerImpl(DeserializerImpl&&) = default;
    constexpr DeserializerImpl& operator=(DeserializerImpl&&) = default;

    // Deserializes the data from the reader.
    template <typename T>
    constexpr Status<void> Read(T* value)
    {
        return Encoding<T>::Read(value, mReader.get());
    }

    template <typename T>
    constexpr Status<void> Read(const LengthField field, const Endianness endian, const Alignment alignment, T* value)
    {
        return Encoding<T>::Read(field, endian, alignment, value, mReader.get());
    }

    constexpr const Reader& reader() const
    {
        return *mReader;
    }

    constexpr Reader& reader()
    {
        return *mReader;
    }

  private:
    std::unique_ptr<Reader> mReader;

    DeserializerImpl(const DeserializerImpl&) = delete;
    DeserializerImpl& operator=(const DeserializerImpl&) = delete;
};

}  // namespace serialization
}  // namespace para

#endif  // PARA_SERIALIZATION_BASE_SERIALIZER_IMPL_H_
