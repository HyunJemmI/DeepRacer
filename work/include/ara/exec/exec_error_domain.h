// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef ARA_EXEC_ERROR_DOMAIN_H_
#define ARA_EXEC_ERROR_DOMAIN_H_

#include "ara/core/error_code.h"
#include "ara/core/error_domain.h"
#include "ara/core/exception.h"

namespace ara
{
namespace exec
{
/// @uptrace{SWS_EM_02281}
enum class ExecErrc : ara::core::ErrorDomain::CodeType
{
    kGeneralError = 1,
    kInvalidArguments = 2,
    kCommunicationError = 3,
    kMetaModelError = 4,
    kCancelled = 5,
    kFailed = 6,
    kFailedUnexpectedTerminationOnExit = 7,
    kFailedUnexpectedTerminationOnEnter = 8,
    kInvalidTransition = 9,
    kAlreadyInState = 10,
    kInTransitionToSameState = 11,
    kNoTimeStamp = 12,
    kNoMatchProcess = 13
};

/// @uptrace{SWS_EM_02282}
class ExecException : public ara::core::Exception
{
  public:
    explicit ExecException(ara::core::ErrorCode err) noexcept : ara::core::Exception(err)
    {}
};

/// @uptrace{SWS_EM_02284}
class ExecErrorDomain final : public ara::core::ErrorDomain
{
    /// @uptrace{SWS_EM_02284}
    constexpr static ara::core::ErrorDomain::IdType kId = 0x8000000000000202ULL;

  public:
    using Errc = ExecErrc;

    /// @uptrace{SWS_EM_02286}
    constexpr ExecErrorDomain() noexcept : ara::core::ErrorDomain(kId)
    {}

    /// @uptrace{SWS_EM_02287}
    char const* Name() const noexcept override
    {
        return "Exec";
    }

    /// @uptrace{SWS_EM_02288}
    char const* Message(ara::core::ErrorDomain::CodeType errorCode) const noexcept override
    {
        const Errc kCode = static_cast<Errc>(errorCode);
        switch (kCode) {
            case Errc::kGeneralError:
                return "general error";
            case Errc::kInvalidArguments:
                return "invalid arguments";
            case Errc::kCommunicationError:
                return "communication error";
            case Errc::kMetaModelError:
                return "meta model error";
            case Errc::kCancelled:
                return "cancelled";
            case Errc::kFailed:
                return "failed";
            case Errc::kFailedUnexpectedTerminationOnExit:
                return "failed unexpected termination on exit";
            case Errc::kFailedUnexpectedTerminationOnEnter:
                return "failed unexpected termination on enter";
            case Errc::kInvalidTransition:
                return "invalid transition";
            case Errc::kAlreadyInState:
                return "already in state";
            case Errc::kInTransitionToSameState:
                return "in transition to same state";
            case Errc::kNoTimeStamp:
                return "no time stamp";
            case Errc::kNoMatchProcess:
                return "no match process";
            default:
                return "unknown error";
        }
    }

    /// @uptrace{SWS_EM_02289}
    void ThrowAsException(ara::core::ErrorCode const& errorCode) const noexcept(false) override
    {
        core::ThrowOrTerminate<ara::core::Exception>(errorCode);
    }
};

namespace internal
{
constexpr ExecErrorDomain mComErrorDomain;
}  // namespace internal

/// @uptrace{SWS_EM_02290}
inline constexpr ara::core::ErrorDomain const& GetExecErrorDomain() noexcept
{
    return internal::mComErrorDomain;
}

/// @uptrace{SWS_EM_02291}
inline constexpr core::ErrorCode MakeErrorCode(ExecErrc code, ara::core::ErrorDomain::SupportDataType data) noexcept
{
    return ara::core::ErrorCode(static_cast<ara::core::ErrorDomain::CodeType>(code), GetExecErrorDomain(), data);
}

}  // namespace exec
}  // namespace ara

#endif  // ARA_EXEC_ERROR_DOMAIN_H_