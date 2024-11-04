// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef ARA_PER_READ_WRITE_ACCESSOR_H_
#define ARA_PER_READ_WRITE_ACCESSOR_H_

#include "ara/core/span.h"
#include "ara/per/read_accessor.h"

namespace ara
{
namespace per
{

class FileStorage;  // Forward declaration

//[SWS_PER_00343]
// @uptrace{SWS_PER_00343}
class ReadWriteAccessor final : public ReadAccessor
{
  public:
    explicit ReadWriteAccessor(const std::string& filename);
    ~ReadWriteAccessor() override;

    // @uptrace{SWS_PER_00122}
    ara::core::Result<void> SyncToFile() noexcept;

    // @uptrace{SWS_PER_00428}
    ara::core::Result<void> SetFileSize(std::uint64_t size) noexcept;

    // @uptrace{SWS_PER_00166}
    ara::core::Result<void> WriteText(ara::core::StringView s) noexcept;

    // @uptrace{SWS_PER_00423}
    ara::core::Result<void> WriteBinary(ara::core::Span<const ara::core::Byte> b) noexcept;

    // @uptrace{SWS_PER_00125}
    ReadWriteAccessor& operator<<(ara::core::StringView s) noexcept;

  private:
    // Forward declaration of the implementation class
    class Impl;
    // The implementation class
    std::shared_ptr<Impl> mImpl;

    // Allow access to private scope
    friend class FileStorage;
};

}  // namespace per
}  // namespace ara

#endif  // ARA_PER_READ_WRITE_ACCESSOR_H_