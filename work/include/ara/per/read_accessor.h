// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef ARA_PER_READ_ACCESSOR_H_
#define ARA_PER_READ_ACCESSOR_H_

#include "ara/core/result.h"
#include "ara/core/span.h"
#include "ara/core/string.h"
#include "ara/core/vector.h"

namespace ara
{
namespace per
{
// @uptrace{SWS_PER_00146}
enum class Origin : std::uint32_t
{
    kBeginning = 0,
    kCurrent = 1,
    kEnd = 2
};

class FileStorage;  // Forward declaration

class ReadAccessor
{
  public:
    ReadAccessor(ara::core::Span<ara::core::Byte> buffer, const std::string& filename);

    explicit ReadAccessor(const std::string& filename);

    // @uptrace{SWS_PER_00146}
    ReadAccessor(ReadAccessor&& ra) noexcept;

    // @uptrace{SWS_PER_00415}
    ReadAccessor(const ReadAccessor&) = delete;

    // @uptrace{SWS_PER_00414}
    ReadAccessor& operator=(ReadAccessor&& ra) & noexcept;

    // @uptrace{SWS_PER_00416}
    ReadAccessor& operator=(const ReadAccessor&) = delete;

    // @uptrace{SWS_PER_00417}
    virtual ~ReadAccessor() noexcept;

    // @uptrace{SWS_PER_00167}
    ara::core::Result<char> PeekChar() const noexcept;

    // @uptrace{SWS_PER_00418}
    ara::core::Result<ara::core::Byte> PeekByte() const noexcept;

    // @uptrace{SWS_PER_00168}
    ara::core::Result<char> GetChar() noexcept;

    // @uptrace{SWS_PER_00419}
    ara::core::Result<ara::core::Byte> GetByte() noexcept;

    // @uptrace{SWS_PER_00420}
    ara::core::Result<ara::core::String> ReadText() noexcept;

    // @uptrace{SWS_PER_00165}
    ara::core::Result<ara::core::String> ReadText(std::uint64_t n) noexcept;

    // @uptrace{SWS_PER_00421}
    ara::core::Result<ara::core::Vector<ara::core::Byte>> ReadBinary() noexcept;

    // @uptrace{SWS_PER_00422}
    ara::core::Result<ara::core::Vector<ara::core::Byte>> ReadBinary(std::uint64_t n) noexcept;

    // @uptrace{SWS_PER_00119}
    ara::core::Result<ara::core::String> ReadLine(char delimiter = '\n') noexcept;

    // @uptrace{SWS_PER_00424}
    std::uint64_t GetSize() const noexcept;

    // @uptrace{SWS_PER_00162}
    uint64_t GetPosition() const noexcept;

    // @uptrace{SWS_PER_00163}
    ara::core::Result<void> SetPosition(std::uint64_t position) noexcept;

    // @uptrace{SWS_PER_00164}
    ara::core::Result<uint64_t> MovePosition(Origin origin, std::int64_t offset) noexcept;

    // @uptrace{SWS_PER_00107}
    bool IsEof() const noexcept;

  protected:
    // Forward declaration of the implementation class
    class Impl;
    // Constructor for inherited class
    explicit ReadAccessor(std::shared_ptr<Impl>);
    // The implementation class
    std::shared_ptr<Impl> mImpl;

    // Allow access to private scope
    friend class FileStorage;
};

}  // namespace per
}  // namespace ara

#endif  // ARA_PER_READ_ACCESSOR_H_