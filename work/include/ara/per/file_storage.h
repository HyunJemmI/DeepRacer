// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef ARA_PER_FILE_STORAGE_H_
#define ARA_PER_FILE_STORAGE_H_

#include "ara/core/instance_specifier.h"
#include "ara/per/read_accessor.h"
#include "ara/per/read_write_accessor.h"
#include "ara/per/shared_handle.h"
#include "ara/per/unique_handle.h"

namespace ara
{
namespace per
{

// @uptrace{SWS_PER_00147}
enum class OpenMode : std::uint32_t
{
    kAtTheBeginning = 1 << 0,
    kAtTheEnd = 1 << 1,
    kTruncate = 1 << 2,
    kAppend = 1 << 3
};

// @uptrace{SWS_PER_00144}
inline constexpr OpenMode operator|(OpenMode left, OpenMode right)
{
    std::uint32_t result = (static_cast<std::uint32_t>(left) | (static_cast<std::uint32_t>(right)));
    return static_cast<OpenMode>(result);
}

// @uptrace{SWS_PER_00434}
inline OpenMode& operator|=(OpenMode& left, const OpenMode& right)
{
    left = left | right;
    return left;
}

// @uptrace{SWS_PER_00435}
enum class FileCreationState : std::uint32_t
{
    kCreatedDuringInstallion = 1,
    kCreatedDuringUpdate = 2,
    kCreatedDuringReset = 3,
    kCreatedDuringRecovery = 4,
    kCreatedByApplication = 5
};

// @uptrace{SWS_PER_00436}
enum class FileModificationState : std::uint32_t
{
    kModifiedDuringUpdate = 2,
    kModifiedDuringReset = 3,
    kModifiedDuringRecovery = 4,
    kModifiedByApplication = 5
};

// @uptrace{SWS_PER_00437}
struct FileInfo
{
    // @uptrace{SWS_PER_00441}
    std::uint64_t creationTime;
    // @uptrace{SWS_PER_00442}
    std::uint64_t modificationTime;
    // @uptrace{SWS_PER_00443}
    std::uint64_t accessTime;
    // @uptrace{SWS_PER_00444}
    FileCreationState fileCreationState;
    // @uptrace{SWS_PER_00445}
    FileModificationState fileModificationState;
};

// @uptrace{SWS_PER_00340}
class FileStorage final
{
  public:
    explicit FileStorage(const ara::core::InstanceSpecifier& fs);

    // @uptrace{SWS_PER_00326}
    FileStorage(FileStorage&& fs) noexcept;

    // @uptrace{SWS_PER_00328}
    FileStorage(const FileStorage&) = delete;

    // @uptrace{SWS_PER_00327}
    FileStorage& operator=(FileStorage&&) & noexcept;

    // @uptrace{SWS_PER_00329}
    FileStorage& operator=(const FileStorage&) = delete;

    // @uptrace{SWS_PER_00330}
    virtual ~FileStorage() noexcept;

    ara::core::Result<void> Init(const std::string& fsInstanceName);

    // @uptrace{SWS_PER_00110}
    ara::core::Result<ara::core::Vector<ara::core::String>> GetAllFileNames() const noexcept;

    // @uptrace{SWS_PER_00407}
    ara::core::Result<std::uint64_t> GetCurrentFileSize(ara::core::StringView fileName) const noexcept;

    // @uptrace{SWS_PER_00111}
    ara::core::Result<void> DeleteFile(ara::core::StringView fileName) noexcept;

    // @uptrace{SWS_PER_00112}
    ara::core::Result<bool> FileExists(ara::core::StringView fileName) const noexcept;

    // @uptrace{SWS_PER_00337}
    ara::core::Result<void> RecoverFile(ara::core::StringView fileName) noexcept;

    // @uptrace{SWS_PER_00338}
    ara::core::Result<void> ResetFile(ara::core::StringView fileName) noexcept;

    // @uptrace{SWS_PER_00407}
    ara::core::Result<FileInfo> GetFileInfo(ara::core::StringView fileName) const noexcept;

    // @uptrace{SWS_PER_00375}
    ara::core::Result<UniqueHandle<ReadWriteAccessor>> OpenFileReadWrite(ara::core::StringView fileName) noexcept;

    // @uptrace{SWS_PER_00113}
    ara::core::Result<UniqueHandle<ReadWriteAccessor>> OpenFileReadWrite(ara::core::StringView fileName,
                                                                         OpenMode mode) noexcept;

    // @uptrace{SWS_PER_00429}
    ara::core::Result<UniqueHandle<ReadWriteAccessor>> OpenFileReadWrite(
        ara::core::StringView fileName, OpenMode mode, ara::core::Span<ara::core::Byte> buffer) noexcept;

    // @uptrace{SWS_PER_00376}
    ara::core::Result<UniqueHandle<ReadAccessor>> OpenFileReadOnly(ara::core::StringView fileName) noexcept;

    // @uptrace{SWS_PER_00114}
    ara::core::Result<UniqueHandle<ReadAccessor>> OpenFileReadOnly(ara::core::StringView fileName,
                                                                   OpenMode mode) noexcept;

    // @uptrace{SWS_PER_00430}
    ara::core::Result<UniqueHandle<ReadAccessor>> OpenFileReadOnly(ara::core::StringView fileName, OpenMode mode,
                                                                   ara::core::Span<ara::core::Byte> buffer) noexcept;

    // @uptrace{SWS_PER_00377}
    ara::core::Result<UniqueHandle<ReadWriteAccessor>> OpenFileWriteOnly(ara::core::StringView fileName) noexcept;

    // @uptrace{SWS_PER_00115}
    ara::core::Result<UniqueHandle<ReadWriteAccessor>> OpenFileWriteOnly(ara::core::StringView fileName,
                                                                         OpenMode mode) noexcept;

    // @uptrace{SWS_PER_00431}
    ara::core::Result<UniqueHandle<ReadWriteAccessor>> OpenFileWriteOnly(
        ara::core::StringView fileName, OpenMode mode, ara::core::Span<ara::core::Byte> buffer) noexcept;

    bool GetStorageStatus();
    ara::core::Result<void> ResetAllFiles();

  private:
    // Forward declaration of the implementation class
    class Impl;
    // The implementation class
    std::unique_ptr<Impl> mImpl;
};

// @uptrace{SWS_PER_00116}
ara::core::Result<SharedHandle<FileStorage>> OpenFileStorage(const ara::core::InstanceSpecifier& fs) noexcept;

// @uptrace{SWS_PER_00335}
ara::core::Result<void> RecoverAllFiles(const ara::core::InstanceSpecifier& fs) noexcept;

// @uptrace{SWS_PER_00336}
ara::core::Result<void> ResetAllFiles(const ara::core::InstanceSpecifier& fs) noexcept;

// @uptrace{SWS_PER_00406}
ara::core::Result<std::uint64_t> GetCurrentFileStorageSize(const ara::core::InstanceSpecifier& fs) noexcept;

}  // namespace per
}  // namespace ara

#endif  // ARA_PER_FILE_STORAGE_H_