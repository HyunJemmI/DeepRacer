// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef ARA_COM_E2E_ERROR_DOMAIN_H_
#define ARA_COM_E2E_ERROR_DOMAIN_H_

#include "ara/core/error_code.h"
#include "ara/core/exception.h"

namespace ara
{
namespace com
{
namespace e2e
{

/// @uptrace{SWS_CM_10474}
enum class E2EErrc : ara::core::ErrorDomain::CodeType
{
    kRepeated = 1,
    kWrongSequence = 2,
    kError = 3,
    kNotAvailable = 4,
    kNoNewData = 5,
};

/// @uptrace{SWS_CM_12501}
class E2EException : public ara::core::Exception
{
  public:
    /// @uptrace{SWS_CM_12502}
    explicit E2EException(ara::core::ErrorCode errorCode) noexcept : ara::core::Exception(errorCode)
    {}
};

/// @uptrace{SWS_CM_12503}
class E2EErrorDomain final : public ara::core::ErrorDomain
{
    /// @uptrace{SWS_CM_99026}
    constexpr static ara::core::ErrorDomain::IdType kId = 0x8000000000001268ULL;

  public:
    /// @uptrace{SWS_CM_12504}
    using Errc = E2EErrc;
    /// @uptrace{SWS_CM_12505}
    using Exception = E2EException;

    /// @uptrace{SWS_CM_12506}
    constexpr E2EErrorDomain() noexcept : ara::core::ErrorDomain(kId)
    {}

    /// @uptrace{SWS_CM_12507}
    char const* Name() const noexcept override
    {
        return "E2E";
    }

    /// @uptrace{SWS_CM_12508}
    char const* Message(ara::core::ErrorDomain::CodeType errorCode) const noexcept override
    {
        Errc const kCode = static_cast<Errc>(errorCode);
        switch (kCode) {
            case Errc::kRepeated:
                return "repeated";
            case Errc::kWrongSequence:
                return "wrong sequence";
            case Errc::kError:
                return "error";
            case Errc::kNotAvailable:
                return "not available";
            case Errc::kNoNewData:
                return "no new data";
            default:
                return "unknown error";
        }
    }

    /// @uptrace{SWS_CM_12509}
    void ThrowAsException(ara::core::ErrorCode const& errorCode) const noexcept(false) override
    {
        core::ThrowOrTerminate<ara::core::Exception>(errorCode);
    }
};

namespace internal
{
constexpr E2EErrorDomain mE2EErrorDomain;
}  // namespace internal

/// @uptrace{SWS_CM_12510}
inline constexpr ara::core::ErrorDomain const& GetComErrorDomain() noexcept
{
    return internal::mE2EErrorDomain;
}

/// @uptrace{SWS_CM_12511}
inline constexpr core::ErrorCode MakeErrorCode(E2EErrc code, ara::core::ErrorDomain::SupportDataType data) noexcept
{
    return ara::core::ErrorCode(static_cast<ara::core::ErrorDomain::CodeType>(code), GetComErrorDomain(), data);
}

}  // namespace e2e
}  // namespace com
}  // namespace ara

#endif  // ARA_COM_E2E_ERROR_DOMAIN_H_