// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef ARA_CORE_CORE_ERROR_DOMAIN_H_
#define ARA_CORE_CORE_ERROR_DOMAIN_H_

#include <cerrno>

#include "ara/core/error_code.h"
#include "ara/core/error_domain.h"
#include "ara/core/exception.h"

namespace ara
{
namespace core
{

/// @brief An enumeration with errors that can occur within this Functional Cluster
/// @uptrace{SWS_CORE_05200}
/// @uptrace{SWS_CORE_10900}
/// @uptrace{SWS_CORE_10901}
/// @uptrace{SWS_CORE_10902}
/// @uptrace{SWS_CORE_10903}
/// @uptrace{SWS_CORE_10999}
enum class CoreErrc : ErrorDomain::CodeType
{
    kInvalidArgument = 22,
    kInvalidMetaModelShortname = 137,
    kInvalidMetaModelPath = 138
};

/// @brief Exception type thrown for CORE errors.
/// @uptrace{SWS_CORE_05211}
/// @uptrace{SWS_CORE_10910}
/// @uptrace{SWS_CORE_10911}
/// @uptrace{SWS_CORE_10999}
class CoreException : public Exception
{
  public:
    /// @uptrace{SWS_CORE_05212}
    explicit CoreException(ErrorCode err) noexcept : Exception(err)
    {}
};

/// @brief An error domain for ara::core errors.
/// @uptrace{SWS_CORE_00011}
/// @uptrace{SWS_CORE_05221}
/// @uptrace{SWS_CORE_10400}
/// @uptrace{SWS_CORE_10930}
/// @uptrace{SWS_CORE_10931}
/// @uptrace{SWS_CORE_10932}
/// @uptrace{SWS_CORE_10950}
/// @uptrace{SWS_CORE_10999}
class CoreErrorDomain final : public ErrorDomain
{
    constexpr static ErrorDomain::IdType kId = 0x8000000000000014;

  public:
    /// @brief Alias for the error code value enumeration
    /// @uptrace{SWS_CORE_05231}
    /// @uptrace{SWS_CORE_10933}
    using Errc = CoreErrc;

    /// @brief Alias for the exception base class
    /// @uptrace{SWS_CORE_05232}
    /// @uptrace{SWS_CORE_10934}
    using Exception = CoreException;

    /// @brief Default constructor
    /// @uptrace{SWS_CORE_05241}
    /// @uptrace{SWS_CORE_00014}
    constexpr CoreErrorDomain() noexcept : ErrorDomain(kId)
    {}

    /// @brief Return the "shortname" ApApplicationErrorDomain.SN of this error domain.
    /// @returns "Core"
    /// @uptrace{SWS_CORE_05242}
    /// @uptrace{SWS_CORE_00014}
    /// @uptrace{SWS_CORE_10951}
    const char* Name() const noexcept override
    {
        return "Core";
    }

    /// @brief Translate an error code value into a text message.
    /// @param errorCode  the error code value
    /// @returns the text message, never nullptr
    /// @uptrace{SWS_CORE_05243}
    const char* Message(ErrorDomain::CodeType errorCode) const noexcept override
    {
        const Errc code = static_cast<Errc>(errorCode);
        switch (code) {
            case Errc::kInvalidArgument:
                return "invalid argument";
            case Errc::kInvalidMetaModelShortname:
                return "invalid meta model shortname";
            case Errc::kInvalidMetaModelPath:
                return "invalid meta model path";
            default:
                return "unknown error";
        }
    }

    /// @brief Throw the exception type corresponding to the given ErrorCode.
    /// @param errorCode  the ErrorCode instance
    /// @uptrace{SWS_CORE_05244}
    void ThrowAsException(const ErrorCode& errorCode) const noexcept(false) override
    {
        /// @uptrace{SWS_CORE_10953}
        ThrowOrTerminate<Exception>(errorCode);
    }
};

namespace internal
{
constexpr CoreErrorDomain g_coreErrorDomain;
}

/// @brief Return a reference to the global CoreErrorDomain.
/// @returns the CoreErrorDomain
/// @uptrace{SWS_CORE_05280}
/// @uptrace{SWS_CORE_10980}
/// @uptrace{SWS_CORE_10981}
/// @uptrace{SWS_CORE_10982}
/// @uptrace{SWS_CORE_10999}
inline constexpr const ErrorDomain& GetCoreErrorDomain() noexcept
{
    return internal::g_coreErrorDomain;
}

/// @brief Create a new ErrorCode within CoreErrorDomain.
/// @param code  the CoreErrorDomain-specific error code value
/// @param data  optional vendor-specific error data
/// @returns a new ErrorCode instance
/// @uptrace{SWS_CORE_05290}
/// @uptrace{SWS_CORE_10990}
/// @uptrace{SWS_CORE_10991}
/// @uptrace{SWS_CORE_10999}
inline constexpr ErrorCode MakeErrorCode(CoreErrc code, ErrorDomain::SupportDataType data) noexcept
{
    return ErrorCode(static_cast<ErrorDomain::CodeType>(code), GetCoreErrorDomain(), data);
}

}  // namespace core
}  // namespace ara

#endif  // ARA_CORE_CORE_ERROR_DOMAIN_H_
