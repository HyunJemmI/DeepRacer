// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef ARA_COM_COM_ERROR_DOMAIN_H_
#define ARA_COM_COM_ERROR_DOMAIN_H_

#include "ara/core/error_code.h"
#include "ara/core/error_domain.h"
#include "ara/core/exception.h"

namespace ara
{
namespace com
{
/// @uptrace{SWS_CM_10432}
enum class ComErrc : ara::core::ErrorDomain::CodeType
{
    /// Service is not available.
    kServiceNotAvailable = 1,
    /// Application holds more SamplePtrs than commited in Subscribe().
    kMaxSamplesReached = 2,
    /// Local failure has been detected by the network binding.
    kNetworkBindingFailure = 3,
    /// Request was refused by Grant enforcement layer.
    kGrantEnforcementError = 4,
    /// TLS handshake fail.
    kPeerIsUnreachable = 5,
    /// Field Value is not valid,.
    kFieldValueIsNotValid = 6,
    /// SetHandler has not been registered.
    kSetHandlerNotSet = 7,
    /// Failure has been detected by unset operation.
    kUnsetFailure = 8,
    /// Not Sufficient memory resources can be allocated.
    kSampleAllocationFailure = 9,
    /// The allocation was illegally done via custom allocator (i.e., not via shared memory allocation).
    kIllegalUseOfAllocate = 10,
    /// Service not offered.
    kServiceNotOffered = 11,
    /// Communication link is broken.
    kCommunicationLinkError = 12,
    /// @deprecated
    /// No clients connected.
    kNoClients = 13,
    /// Communication Stack Error, e.g. network stack, network binding, or communication framework reports an error
    kCommunicationStackError = 14,
    /// ResolveInstanceIDs() failed to resolve InstanceID from InstanceSpecifier, i.e. is not mapped correctly.
    kInstanceIDCouldNotBeResolved = 15,
    /// Provided maxSampleCount not realizable.
    kMaxSampleCountNotRealizable = 16,
    /// Wrong processing mode passed to constructor method call.
    kWrongMethodCallProcessingMode = 17,
    /// The FileHandle returned from FindService is corrupt/service not available.
    kErroneousFileHandle = 18,
    /// Command could not be executed in provided Execution Context.
    kCouldNotExecute = 19,
    /// Given InstanceIdentifier string is corrupted or non-compliant.
    kInvalidInstanceIdentifierString = 20,
};

/// @uptrace{SWS_CM_11327}
class ComException : public ara::core::Exception
{
  public:
    explicit ComException(ara::core::ErrorCode err) noexcept : ara::core::Exception(err)
    {}
};

/// @uptrace{SWS_CM_11329}
class ComErrorDomain final : public ara::core::ErrorDomain
{
    /// @uptrace{SWS_CM_11267}
    constexpr static ara::core::ErrorDomain::IdType kId = 0x8000000000001267ULL;

  public:
    using Errc = ComErrc;

    /// @uptrace{SWS_CM_11330}
    constexpr ComErrorDomain() noexcept : ara::core::ErrorDomain(kId)
    {}

    /// @uptrace{SWS_CM_11331}
    char const* Name() const noexcept override
    {
        return "Com";
    }

    /// @uptrace{SWS_CM_11332}
    char const* Message(ara::core::ErrorDomain::CodeType errorCode) const noexcept override
    {
        Errc const kCode = static_cast<Errc>(errorCode);
        switch (kCode) {
            case Errc::kServiceNotAvailable:
                return "service not available";
            case Errc::kMaxSamplesReached:
                return "max samples reached";
            case Errc::kNetworkBindingFailure:
                return "network binding failure";
            case Errc::kGrantEnforcementError:
                return "grant enforcement error";
            case Errc::kPeerIsUnreachable:
                return "peer is unreachable";
            case Errc::kFieldValueIsNotValid:
                return "field value is not valid";
            case Errc::kSetHandlerNotSet:
                return "set handler not set";
            case Errc::kUnsetFailure:
                return "unset failure";
            case Errc::kSampleAllocationFailure:
                return "sample allocation failure";
            case Errc::kIllegalUseOfAllocate:
                return "illegal use of allocate";
            case Errc::kServiceNotOffered:
                return "service not offered";
            case Errc::kCommunicationLinkError:
                return "communication link error";
            case Errc::kNoClients:
                return "no clients";
            case Errc::kCommunicationStackError:
                return "communication stack error";
            case Errc::kInstanceIDCouldNotBeResolved:
                return "instance id could not be resolved";
            case Errc::kMaxSampleCountNotRealizable:
                return "max sample count not realizable";
            case Errc::kWrongMethodCallProcessingMode:
                return "wrong method call processing mode";
            case Errc::kErroneousFileHandle:
                return "erroneous file handle";
            case Errc::kCouldNotExecute:
                return "could not execute";
            case Errc::kInvalidInstanceIdentifierString:
                return "invalid instance identifier string";
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
constexpr ComErrorDomain mComErrorDomain;
}  // namespace internal

/// @uptrace{SWS_CM_11334}
inline constexpr ara::core::ErrorDomain const& GetComErrorDomain() noexcept
{
    return internal::mComErrorDomain;
}

/// @uptrace{SWS_CM_11335}
inline constexpr core::ErrorCode MakeErrorCode(ComErrc code, ara::core::ErrorDomain::SupportDataType data) noexcept
{
    return ara::core::ErrorCode(static_cast<ara::core::ErrorDomain::CodeType>(code), GetComErrorDomain(), data);
}

}  // namespace com
}  // namespace ara

#endif  // ARA_COM_COM_ERROR_DOMAIN_H_