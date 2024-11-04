// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef ARA_CRYPTO_COMMON_SECURITY_ERROR_DOMAIN_H
#define ARA_CRYPTO_COMMON_SECURITY_ERROR_DOMAIN_H

#include "ara/core/error_code.h"
#include "ara/core/error_domain.h"
#include "ara/core/exception.h"

namespace ara
{
namespace crypto
{

/// @brief Enumeration of all Security Error Code values that may be reported by ara::crypto.
/// @uptrace{SWS_CRYPT_10099}
enum class SecurityErrc : ara::core::ErrorDomain::CodeType
{
    kErrorClass = 0x1000000U,                    // Reserved (a multiplier of error class IDs)
    kErrorSubClass = 0x10000U,                   // Reserved (a multiplier of error sub-class IDs)
    kErrorSubSubClass = 0x100U,                  // Reserved (a multiplier of error sub-sub-class IDs)
    kResourceFault = 1 * kErrorClass,            // ResourceException: Generic resource fault!
    kBusyResource = kResourceFault + 1,          // ResourceException: Specified resource is busy!
    kInsufficientResource = kResourceFault + 2,  // ResourceException: Insufficient capacity of specified resource!
    kUnreservedResource = kResourceFault + 3,    // ResourceException: Specified resource was not reserved!
    kModifiedResource = kResourceFault + 4,      // ResourceException: Specified resource has been modified!
    kLogicFault = 2 * kErrorClass,               // LogicException: Generic logic fault!
    kInvalidArgument =
        kLogicFault + 1 * kErrorSubClass,       // InvalidArgumentException: An invalid argument value is provided!
    kUnknownIdentifier = kInvalidArgument + 1,  // InvalidArgumentException: Unknown identifier is provided!
    kInsufficientCapacity =
        kInvalidArgument + 2,                  // InvalidArgumentException: Insufficient capacity of the output buffer!
    kInvalidInputSize = kInvalidArgument + 3,  // InvalidArgumentException: Invalid size of an input buffer!
    kIncompatibleArguments =
        kInvalidArgument + 4,  // InvalidArgumentException: Provided values of arguments are incompatible!
    kInOutBuffersIntersect = kInvalidArgument + 5,  // InvalidArgumentException: Input and output buffers are intersect!
    kBelowBoundary = kInvalidArgument + 6,  // InvalidArgumentException: Provided value is below the lower boundary!
    kAboveBoundary = kInvalidArgument + 7,  // InvalidArgumentException: Provided value is above the upper boundary!
    kAuthTagNotValid =
        kInvalidArgument + 8,  // AuthTagNotValidException: Provided authentication-tag cannot be verified!
    kUnsupported =
        kInvalidArgument +
        1 * kErrorSubSubClass,  // UnsupportedException: Unsupported request (due to limitations of the implementation)!
    kInvalidUsageOrder =
        kLogicFault + 2 * kErrorSubClass,  // InvalidUsageOrderException: Invalid usage order of the interface!
    kUninitializedContext =
        kInvalidUsageOrder + 1,  // InvalidUsageOrderException: Context of the interface was not initialized!
    kProcessingNotStarted = kInvalidUsageOrder + 2,  // InvalidUsageOrderException: Data processing was not started yet!
    kProcessingNotFinished =
        kInvalidUsageOrder + 3,              // InvalidUsageOrderException: Data processing was not finished yet!
    kRuntimeFault = 3 * kErrorClass,         // RuntimeException: Generic runtime fault!
    kUnsupportedFormat = kRuntimeFault + 1,  // RuntimeException: Unsupported serialization format for this object type!
    kBruteForceRisk =
        kRuntimeFault + 2,  // RuntimeException: Operation is prohibitted due to a risk of a brute force attack!
    kContentRestrictions =
        kRuntimeFault + 3,  // RuntimeException: The operation violates content restrictions of the target container!
    kBadObjectReference = kRuntimeFault + 4,  // RuntimeException: Incorrect reference between objects!
    kContentDuplication =
        kRuntimeFault + 6,  // RuntimeException: Provided content already exists in the target storage!
    kUnexpectedValue =
        kRuntimeFault + 1 * kErrorSubClass,  // UnexpectedValueException: Unexpected value of an argument is provided!
    kIncompatibleObject = kUnexpectedValue + 1,  // UnexpectedValueException: The provided object is incompatible with
                                                 // requested operation or its configuration!
    kIncompleteArgState = kUnexpectedValue + 2,  // UnexpectedValueException: Incomplete state of an argument!
    kEmptyContainer = kUnexpectedValue + 3,      // UnexpectedValueException: Specified container is empty!
    kMissingArgument = kUnexpectedValue + 4,     // kMissingArgumentException: Expected argument, but none provided!
    kBadObjectType =
        kUnexpectedValue + 1 * kErrorSubSubClass,  // BadObjectTypeException: Provided object has unexpected type!
    kUsageViolation =
        kRuntimeFault + 2 * kErrorSubClass,  // UsageViolationException: Violation of allowed usage for the object!
    kAccessViolation = kRuntimeFault + 3 * kErrorSubClass  // AccessViolationException: Access rights violation!
};

/// @brief Exception type thrown for CRYPTO errors.
/// @uptrace{SWS_CRYPT_19905}
class SecurityException : public ara::core::Exception
{
  public:
    /// @brief Construct a new SecurityException from an ErrorCode.
    /// @param err the ErrorCode
    /// @uptrace{SWS_CRYPT_19906}
    explicit SecurityException(ara::core::ErrorCode err) noexcept : ara::core::Exception(err)
    {}
};

/// @brief Security Error Domain class that provides interfaces as defined by ara::core::ErrorDomain
///        such as a name of the Security Error Domain or messages for each error code.
///        This class represents an error domain responsible for all errors that may be reported by
///        public APIs in ara::crypto namespace
/// @uptrace{SWS_CRYPT_19900}
class SecurityErrorDomain final : public ara::core::ErrorDomain
{
    /// @uptrace{SWS_CRYPT_19900}
    constexpr static ara::core::ErrorDomain::IdType kId = 0x8000000000000801UL;

  public:
    /// @brief Security error
    /// @uptrace{SWS_CRYPT_19903}
    using Errc = SecurityErrc;
    /// @brief Alias for the exception base class.
    /// @uptrace{SWS_CRYPT_19904}
    using Exception = SecurityException;

    /// @brief Ctor of the SecurityErrorDomain.
    /// @uptrace{SWS_CRYPT_19902}
    constexpr SecurityErrorDomain() noexcept : ara::core::ErrorDomain(kId)
    {}

    /// @brief returns Text "Security"
    /// @return "Security" text
    /// @uptrace{SWS_CRYPT_19950}
    const char* Name() const noexcept override
    {
        return "Security";
    }

    /// @brief Translate an error code value into a text message.
    /// @param errorCode an error code identifier from the SecurityErrc enumeration
    /// @return message text of error code
    /// @uptrace{SWS_CRYPT_19953}
    const char* Message(ara::core::ErrorDomain::CodeType errorCode) const noexcept override
    {
        const Errc kCode = static_cast<Errc>(errorCode);
        switch (kCode) {
            case Errc::kErrorClass:
                return "error class";
            case Errc::kErrorSubClass:
                return "error sub class";
            case Errc::kErrorSubSubClass:
                return "error sub sub class";
            case Errc::kResourceFault:
                return "resource fault";
            case Errc::kBusyResource:
                return "busy resource";
            case Errc::kInsufficientResource:
                return "insufficient resource";
            case Errc::kUnreservedResource:
                return "unreserved resource";
            case Errc::kModifiedResource:
                return "modified resource";
            case Errc::kLogicFault:
                return "logic fault";
            case Errc::kInvalidArgument:
                return "invalid argument";
            case Errc::kUnknownIdentifier:
                return "unknown identifier";
            case Errc::kInsufficientCapacity:
                return "insufficient capacity";
            case Errc::kInvalidInputSize:
                return "invalid input size";
            case Errc::kIncompatibleArguments:
                return "incompatible arguments";
            case Errc::kInOutBuffersIntersect:
                return "inout buffers intersect";
            case Errc::kBelowBoundary:
                return "below boundary";
            case Errc::kAboveBoundary:
                return "above boundary";
            case Errc::kAuthTagNotValid:
                return "auth tag not valid";
            case Errc::kUnsupported:
                return "unsupported";
            case Errc::kInvalidUsageOrder:
                return "invalid usage order";
            case Errc::kUninitializedContext:
                return "uninitialized context";
            case Errc::kProcessingNotStarted:
                return "processing not started";
            case Errc::kProcessingNotFinished:
                return "processing not finished";
            case Errc::kRuntimeFault:
                return "runtime fault";
            case Errc::kUnsupportedFormat:
                return "unsupported format";
            case Errc::kBruteForceRisk:
                return "brute force risk";
            case Errc::kContentRestrictions:
                return "content restrictions";
            case Errc::kBadObjectReference:
                return "bad object reference";
            case Errc::kContentDuplication:
                return "content duplication";
            case Errc::kUnexpectedValue:
                return "unexpected value";
            case Errc::kIncompatibleObject:
                return "incompatible object";
            case Errc::kIncompleteArgState:
                return "incomplete arg state";
            case Errc::kEmptyContainer:
                return "empty container";
            case Errc::kMissingArgument:
                return "missing argument";
            case Errc::kBadObjectType:
                return "bad object type";
            case Errc::kUsageViolation:
                return "usage violation";
            case Errc::kAccessViolation:
                return "access violation";
            default:
                return "unknown error";
        }
    }

    /// @brief throws exception of error code
    /// @param errorCode an error code identifier from the SecurityErrc enumeration
    /// @uptrace{SWS_CRYPT_19954}
    void ThrowAsException(const ara::core::ErrorCode& errorCode) const noexcept(false) override
    {
        core::ThrowOrTerminate<ara::core::Exception>(errorCode);
    }
};

namespace internal
{
constexpr SecurityErrorDomain m_securityErrorDomain;
}  // namespace internal

inline constexpr const ara::core::ErrorDomain& GetSecurityErrorDomain() noexcept
{
    return internal::m_securityErrorDomain;
}

/// @brief Makes Error Code instances from the Security Error Domain.
///        The returned ErrorCode instance always references to SecurityErrorDomain.
/// @uptrace{SWS_CRYPT_19951}
inline constexpr core::ErrorCode MakeErrorCode(SecurityErrc code, ara::core::ErrorDomain::SupportDataType data) noexcept
{
    return ara::core::ErrorCode(static_cast<ara::core::ErrorDomain::CodeType>(code), GetExecErrorDomain(), data);
}

}  // namespace crypto
}  // namespace ara

#endif  // ARA_CRYPTO_COMMON_SECURITY_ERROR_DOMAIN_H