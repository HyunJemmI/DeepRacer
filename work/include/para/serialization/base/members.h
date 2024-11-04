// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef PARA_SERIALIZATION_BASE_MEMBERS_H_
#define PARA_SERIALIZATION_BASE_MEMBERS_H_

#include "para/serialization/base/encoding.h"
#include "para/serialization/base/encoding_helper.h"
#include "para/serialization/base/utility.h"
#include "para/serialization/types/member_pointer.h"

namespace para
{
namespace serialization
{
// struct/class T encoding format:
// +----------+----//-----+
// | UINT32:L | N MEMBERS |
// +----------+----//-----+
// Members must be valid encodings of type T.
template <typename T>
struct Encoding<T, EnableIfHasMemberList<T>> : EncodingIO<T>
{
    static std::string TypeName()
    {
        std::stringstream ss;
        ss << "struct{";
        ss << TypeNameImpl(Index<Count>{});
        ss << "}";

        return ss.str();
    }

    static constexpr std::size_t SizePayload(const T& value)
    {
        return Size(value, Index<Count>{});
    }

    static constexpr std::size_t SizePayload(const LengthField field, const T& value)
    {
        return Size(field, value, Index<Count>{});
    }

    static constexpr std::size_t Size(const T& value)
    {
        return 4U + SizePayload(value);
    }

    static constexpr std::size_t Size(const LengthField field, const T& value)
    {
        return EncodingHelper::GetLengthSize(field) + SizePayload(field, value);
    }

    template <typename Writer>
    static constexpr Status<void> WritePayload(const T& value, Writer* writer)
    {
        SizeType length = SizePayload(value);
        auto status = EncodingHelper::WriteLength(length, writer);
        if (!status)
            return status;
        else
            return WriteMembers(value, writer, Index<Count>{});
    }

    template <typename Writer>
    static constexpr Status<void> WritePayload(const LengthField field, const Endianness endian,
                                               const Alignment alignment, const T& value, Writer* writer)
    {
        SizeType length = SizePayload(field, value);
        auto status = EncodingHelper::WriteLength(field, endian, length, writer);
        if (!status)
            return status;
        else
            return WriteMembers(field, endian, alignment, value, writer, Index<Count>{});
    }

    template <typename Reader>
    static constexpr Status<void> ReadPayload(T* value, Reader* reader)
    {
        SizeType length = 0;
        auto status = EncodingHelper::ReadLength(&length, reader);
        if (!status)
            return status;
        return ReadMembers(value, reader, Index<Count>{});
    }

    template <typename Reader>
    static constexpr Status<void> ReadPayload(const LengthField field, const Endianness endian,
                                              const Alignment alignment, T* value, Reader* reader)
    {
        SizeType length = 0;
        auto status = EncodingHelper::ReadLength(field, endian, &length, reader);
        if (!status)
            return status;
        return ReadMembers(field, endian, alignment, value, reader, Index<Count>{});
    }

  private:
    enum : std::size_t
    {
        Count = MemberListTraits<T>::MemberList::Count
    };

    using MemberList = typename MemberListTraits<T>::MemberList;

    template <std::size_t Index>
    using PointerAt = typename MemberList::template At<Index>;

    static std::string TypeNameImpl(Index<0> /*unused*/)
    {
        return "";
    }

    template <std::size_t index>
    static std::string TypeNameImpl(Index<index> /*unused*/)
    {
        std::stringstream ss;

        ss << TypeNameImpl(Index<index - 1>{});
        if (index - 1 > 0) {
            ss << ",";
        }
        ss << PointerAt<index - 1>::TypeName();

        return ss.str();
    }

    static constexpr std::size_t Size(const T& /*value*/, Index<0>)
    {
        return 0;
    }

    static constexpr std::size_t Size(const LengthField /*field*/, const T& /*value*/, Index<0>)
    {
        return 0;
    }

    template <std::size_t index>
    static constexpr std::size_t Size(const T& value, Index<index>)
    {
        return Size(value, Index<index - 1>{}) + PointerAt<index - 1>::Size(value);
    }

    template <std::size_t index>
    static constexpr std::size_t Size(const LengthField field, const T& value, Index<index>)
    {
        return Size(field, value, Index<index - 1>{}) + PointerAt<index - 1>::Size(field, value);
    }

    template <typename Writer>
    static constexpr Status<void> WriteMembers(const T& /*value*/, Writer* /*writer*/, Index<0>)
    {
        return {};
    }

    template <typename Writer>
    static constexpr Status<void> WriteMembers(const LengthField /*field*/, const Endianness /*endian*/,
                                               const Alignment /*alignment*/, const T& /*value*/, Writer* /*writer*/,
                                               Index<0>)
    {
        return {};
    }

    template <std::size_t index, typename Writer>
    static constexpr Status<void> WriteMembers(const T& value, Writer* writer, Index<index>)
    {
        auto status = WriteMembers(value, writer, Index<index - 1>{});
        if (!status)
            return status;
        else
            return PointerAt<index - 1>::Write(value, writer, MemberList{});
    }

    template <std::size_t index, typename Writer>
    static constexpr Status<void> WriteMembers(const LengthField field, const Endianness endian,
                                               const Alignment alignment, const T& value, Writer* writer, Index<index>)
    {
        auto status = WriteMembers(field, endian, alignment, value, writer, Index<index - 1>{});
        if (!status)
            return status;
        else
            return PointerAt<index - 1>::Write(field, endian, alignment, value, writer, MemberList{});
    }

    template <typename Reader>
    static constexpr Status<void> ReadMembers(T* /*value*/, Reader* /*reader*/, Index<0>)
    {
        return {};
    }

    template <typename Reader>
    static constexpr Status<void> ReadMembers(const LengthField /*field*/, const Endianness /*endian*/,
                                              const Alignment /*alignment*/, T* /*value*/, Reader* /*reader*/, Index<0>)
    {
        return {};
    }

    template <std::size_t index, typename Reader>
    static constexpr Status<void> ReadMembers(T* value, Reader* reader, Index<index>)
    {
        auto status = ReadMembers(value, reader, Index<index - 1>{});
        if (!status)
            return status;
        else
            return PointerAt<index - 1>::Read(value, reader, MemberList{});
    }

    template <std::size_t index, typename Reader>
    static constexpr Status<void> ReadMembers(const LengthField field, const Endianness endian,
                                              const Alignment alignment, T* value, Reader* reader, Index<index>)
    {
        auto status = ReadMembers(field, endian, alignment, value, reader, Index<index - 1>{});
        if (!status)
            return status;
        else
            return PointerAt<index - 1>::Read(field, endian, alignment, value, reader, MemberList{});
    }
};

}  // namespace serialization
}  // namespace para

#endif  // PARA_SERIALIZATION_BASE_MEMBERS_H_