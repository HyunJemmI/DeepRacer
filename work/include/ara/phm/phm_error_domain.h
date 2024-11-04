// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef ARA_PHM_PHM_ERROR_DOMAIN_H_
#define ARA_PHM_PHM_ERROR_DOMAIN_H_

#include "ara/core/error_code.h"
#include "ara/core/error_domain.h"
#include "ara/core/exception.h"

namespace ara
{
namespace phm
{
///
enum class PhmErrc : ara::core::ErrorDomain::CodeType
{
    kSupervisionNotAvailable = 1,
    kCheckpointNotAvailable = 2,
    kHealthStatusNotAvailable = 3,
    kWrongInstance = 4,
    kOfferFailed = 11,
    kStopOfferFailed = 12,
    kTemplateTypeError = 21
};

///
class PhmException : public ara::core::Exception
{
  public:
    explicit PhmException(ara::core::ErrorCode err) noexcept : ara::core::Exception(err)
    {}
};

/// @uptrace{SWS_CM_11329}
class PhmErrorDomain final : public ara::core::ErrorDomain
{
    /// @uptrace{SWS_CM_11267}
    /// vendor specific ErrorDomainId
    constexpr static ara::core::ErrorDomain::IdType kId = 0xC000'0000'004F'0087ULL;

  public:
    using Errc = PhmErrc;

    /// @uptrace{SWS_CM_11330}
    constexpr PhmErrorDomain() noexcept : ara::core::ErrorDomain(kId)
    {}

    /// @uptrace{SWS_CM_11331}
    char const* Name() const noexcept override
    {
        return "Phm";
    }

    /// @uptrace{SWS_CM_11332}
    char const* Message(ara::core::ErrorDomain::CodeType errorCode) const noexcept override
    {
        Errc const kCode = static_cast<Errc>(errorCode);
        switch (kCode) {
            case Errc::kSupervisionNotAvailable:
                return "superivsion is not available";
            case Errc::kCheckpointNotAvailable:
                return "checkpoint is not available";
            case Errc::kHealthStatusNotAvailable:
                return "status not available";
            case Errc::kWrongInstance:
                return "wrong instance";
            case Errc::kOfferFailed:
                return "failed offer";
            case Errc::kStopOfferFailed:
                return "failed stop offer";
            case Errc::kTemplateTypeError:
                return "worng template type";
            default:
                return "unknown error";
        }
    }

    /// @uptrace{SWS_CM_11333}
    void ThrowAsException(ara::core::ErrorCode const& errorCode) const noexcept(false) override
    {
        core::ThrowOrTerminate<ara::core::Exception>(errorCode);
    }
};

namespace internal
{
constexpr PhmErrorDomain mPhmErrorDomain;
}  // namespace internal

///
inline constexpr ara::core::ErrorDomain const& GetPhmErrorDomain() noexcept
{
    return internal::mPhmErrorDomain;
}

///
inline constexpr core::ErrorCode MakeErrorCode(PhmErrc code, ara::core::ErrorDomain::SupportDataType data) noexcept
{
    return ara::core::ErrorCode(static_cast<ara::core::ErrorDomain::CodeType>(code), GetPhmErrorDomain(), data);
}

}  // namespace phm
}  // namespace ara

#endif  // ARA_PHM_PHM_ERROR_DOMAIN_H_