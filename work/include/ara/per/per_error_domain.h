// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef ARA_PER_PER_ERROR_DOMAIN_H_
#define ARA_PER_PER_ERROR_DOMAIN_H_

#include "ara/core/core_error_domain.h"

namespace ara
{
namespace per
{

//[SWS_PER_00311]
enum class PerErrc : ara::core::ErrorDomain::CodeType
{
    kStorageNotFound = 1,
    kKeyNotFound = 2,
    kIllegalWriteAccess = 3,
    kPhysicalStorageFailure = 4,
    kIntegrityCorrupted = 5,
    kValidationFailed = 6,
    kEncryptionFailed = 7,
    kDataTypeMismatch = 8,
    kInitValueNotAvailable = 9,
    kResourceBusy = 10,
    kOutOfStorageSpace = 12,
    kFileNotFound = 13,
    kNotInitialized = 14,
    kInvalidPosition = 15,
    kIsEof = 16,
    kInvalidOpenMode = 17,
    kInvalidSize = 18
};

//[SWS_PER_00354]
class PerException : public ara::core::Exception
{
  public:
    //[SWS_PER_00355]
    explicit PerException(ara::core::ErrorCode errorCode) noexcept : ara::core::Exception(std::move(errorCode))
    {}
};

//[SWS_PER_00312]
class PerErrorDomain final : public ara::core::ErrorDomain
{
    constexpr static ara::core::ErrorDomain::IdType kId = 0x8000'0000'0000'0101;

  public:
    using Errc = PerErrc;
    using Exception = PerException;

    //[SWS_PER_00313]
    constexpr PerErrorDomain() noexcept : ErrorDomain(kId)
    {}

    //[SWS_PER_00314]
    const char* Name() const noexcept override
    {
        return "Per";
    }

    //[SWS_PER_00315]
    const char* Message(ara::core::ErrorDomain::CodeType errorCode) const noexcept override
    {
        Errc const code = static_cast<Errc>(errorCode);
        switch (code) {
            case Errc::kStorageNotFound:
                return "Not configured in the AUTOSAR model";
            case Errc::kKeyNotFound:
                return "Key cannot be not found in the Key-Value Storage";
            case Errc::kIllegalWriteAccess:
                return "The storage is configured read-only";
            case Errc::kPhysicalStorageFailure:
                return "Not accessing the physical storage";
            case Errc::kIntegrityCorrupted:
                return "The structural integrity of the storage could not be established";
            case Errc::kValidationFailed:
                return "The validation of redundancy measures failed";
            case Errc::kEncryptionFailed:
                return "The encryption or decryption failed";
            case Errc::kDataTypeMismatch:
                return "The provided data type does not match the stored data type";
            case Errc::kInitValueNotAvailable:
                return "The operation could not be performed because no initial value is available";
            case Errc::kResourceBusy:
                return "The operation could not be performed because the resource is currently busy";
            case Errc::kOutOfStorageSpace:
                return "The allocated storage quota was exceeded";
            case Errc::kFileNotFound:
                return "The requested file cannot be not found in the File Storage";
            case Errc::kNotInitialized:
                return "Called before ara::core::Initialize() or after ara::core::Deinitialize()";
            case Errc::kInvalidPosition:
                return "SetPosition/MovePosition tried to move to a position that is not reachable";
            case Errc::kIsEof:
                return "The application tried to read from the end of the file or from an empty file";
            case Errc::kInvalidOpenMode:
                return "Opening a file failed because the requested combination of OpenModes is invalid";
            case Errc::kInvalidSize:
                return "SetFileSize tried to set a new size that is bigger than the current file size";
            default:
                return "Unknown error";
        }
    }

    //[SWS_PER_00350]
    void ThrowAsException(const ara::core::ErrorCode& errorCode) const noexcept(false) override
    {
        ara::core::ThrowOrTerminate<Exception>(errorCode);
    }
};

namespace internal
{
constexpr PerErrorDomain g_PerErrorDomain;
}

inline constexpr ara::core::ErrorDomain const& GetPerErrorDomain() noexcept
{
    return internal::g_PerErrorDomain;
}

inline constexpr ara::core::ErrorCode MakeErrorCode(PerErrc code, ara::core::ErrorDomain::SupportDataType data) noexcept
{
    return ara::core::ErrorCode(static_cast<ara::core::ErrorDomain::CodeType>(code), GetPerErrorDomain(), data);
}

}  // namespace per
}  // namespace ara

#endif  // ARA_PER_PER_ERROR_DOMAIN_H_