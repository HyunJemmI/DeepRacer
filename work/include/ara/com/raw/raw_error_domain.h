// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef ARA_COM_RAW_ERROR_DOMAIN_H_
#define ARA_COM_RAW_ERROR_DOMAIN_H_

#include "ara/core/error_code.h"
#include "ara/core/error_domain.h"
#include "ara/core/exception.h"

namespace ara
{
namespace com
{
namespace raw
{
/// @uptrace{SWS_CM_12367}
enum class RawErrc : ara::core::ErrorDomain::CodeType
{
    kStreamNotConnected = 1,
    kCommunicationTimeout = 2,
    kConnectionRefused = 3,
    kAddressNotAvailable = 4,
    kStreamAlreadyConnected = 5,
    kConnectionClosedByPeer = 6,
    kPeerUnreachable = 7,
    kConnectionAborted = 8,
    kInterruptedBySignal = 9
};

/// @uptrace{SWS_CM_11291}
class RawException : public ara::core::Exception
{
  public:
    /// @uptrace{SWS_CM_11292}
    explicit RawException(ara::core::ErrorCode err) noexcept : ara::core::Exception(err)
    {}
};

/// @uptrace{SWS_CM_11293}
class RawErrorDomain final : public ara::core::ErrorDomain
{
    /// @uptrace{SWS_CM_99025}
    constexpr static ara::core::ErrorDomain::IdType kDomainId = 0x8000000000001269ULL;

  public:
    using Errc = RawErrc;

    /// @uptrace{UNKNOWN}
    constexpr RawErrorDomain() noexcept : ara::core::ErrorDomain(kDomainId)
    {}

    /// @uptrace{SWS_CM_11295}
    char const* Name() const noexcept override
    {
        return "Raw";
    }

    /// @uptrace{SWS_CM_11296}
    char const* Message(ara::core::ErrorDomain::CodeType errorCode) const noexcept override
    {
        Errc const kCode = static_cast<Errc>(errorCode);
        switch (kCode) {
            case Errc::kStreamNotConnected:
                return "stream not connected";
            case Errc::kCommunicationTimeout:
                return "communication timeout";
            case Errc::kConnectionRefused:
                return "connection refused";
            case Errc::kAddressNotAvailable:
                return "address not available";
            case Errc::kStreamAlreadyConnected:
                return "stream already connected";
            case Errc::kConnectionClosedByPeer:
                return "connection closed by peer";
            case Errc::kPeerUnreachable:
                return "peer unreachable";
            case Errc::kConnectionAborted:
                return "connection aborted";
            case Errc::kInterruptedBySignal:
                return "interrupted by signal";
            default:
                return "unknown error";
        }
    }

    /// @uptrace{SWS_CM_11297}
    void ThrowAsException(ara::core::ErrorCode const& errorCode) const noexcept(false) override
    {
        core::ThrowOrTerminate<ara::core::Exception>(errorCode);
    }
};

namespace internal
{
constexpr RawErrorDomain mRawErrorDomain;
}  // namespace internal

/// @uptrace{SWS_CM_11298}
inline constexpr ara::core::ErrorDomain const& GetRawErrorDomain() noexcept
{
    return internal::mRawErrorDomain;
}

/// @uptrace{SWS_CM_11299}
inline constexpr ara::core::ErrorCode MakeErrorCode(RawErrc code, ara::core::ErrorDomain::SupportDataType data) noexcept
{
    return ara::core::ErrorCode(static_cast<ara::core::ErrorDomain::CodeType>(code), GetRawErrorDomain(), data);
}

}  // namespace raw
}  // namespace com
}  // namespace ara

#endif  // ARA_COM_RAW_ERROR_DOMAIN_H_