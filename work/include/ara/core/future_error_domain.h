// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef ARA_CORE_FUTURE_ERROR_DOMAIN_H_
#define ARA_CORE_FUTURE_ERROR_DOMAIN_H_

#include <cstdint>

#include "ara/core/error_code.h"
#include "ara/core/error_domain.h"
#include "ara/core/exception.h"

namespace ara
{
namespace core
{

/// @brief Specifies the types of internal errors that can occur upon calling Future::get or Future::GetResult.
/// @uptrace{SWS_CORE_00400}
/// @uptrace{SWS_CORE_10900}
/// @uptrace{SWS_CORE_10902}
/// @uptrace{SWS_CORE_10903}
/// @uptrace{SWS_CORE_10999}
enum class future_errc : std::int32_t
{
    broken_promise = 101,
    future_already_retrieved = 102,
    promise_already_satisfied = 103,
    no_state = 104
};

/// @brief Exception type thrown by Future and Promise classes.
/// @uptrace{SWS_CORE_00411}
/// @uptrace{SWS_CORE_10910}
/// @uptrace{SWS_CORE_10911}
/// @uptrace{SWS_CORE_10999}
class FutureException : public Exception
{
  public:
    /// @brief Construct a new FutureException from an ErrorCode.
    /// @param err  the ErrorCode
    /// @uptrace{SWS_CORE_00412}
    explicit FutureException(ErrorCode err) noexcept : Exception(std::move(err))
    {}
};

/// @brief Error domain for errors originating from classes Future and Promise.
/// @uptrace{SWS_CORE_00011}
/// @uptrace{SWS_CORE_10400}
/// @uptrace{SWS_CORE_10930}
/// @uptrace{SWS_CORE_10931}
/// @uptrace{SWS_CORE_10932}
/// @uptrace{SWS_CORE_10950}
/// @uptrace{SWS_CORE_10999}
class FutureErrorDomain final : public ErrorDomain
{
    /// @uptrace{SWS_CORE_00421}
    constexpr static ErrorDomain::IdType kId = 0x8000000000000013;

  public:
    /// @brief Alias for the error code value enumeration
    /// @uptrace{SWS_CORE_00431}
    /// @uptrace{SWS_CORE_10933}
    using Errc = future_errc;
    /// @brief Alias for the exception base class
    /// @uptrace{SWS_CORE_00432}
    /// @uptrace{SWS_CORE_10934}
    using Exception = FutureException;

    /// @brief Default constructor
    /// @uptrace{SWS_CORE_00441}
    /// @uptrace{SWS_CORE_00013}
    constexpr FutureErrorDomain() noexcept : ErrorDomain(kId)
    {}

    /// @brief Return the "shortname" ApApplicationErrorDomain.SN of this error domain.
    /// @returns "Future"
    /// @uptrace{SWS_CORE_00442}
    /// @uptrace{SWS_CORE_00013}
    /// @uptrace{SWS_CORE_10951}
    const char* Name() const noexcept override
    {
        return "Future";
    }

    /// @brief Translate an error code value into a text message.
    /// @param errorCode  the error code value
    /// @returns the text message, never nullptr
    /// @uptrace{SWS_CORE_00443}
    const char* Message(ErrorDomain::CodeType errorCode) const noexcept override
    {
        const Errc code = static_cast<Errc>(errorCode);
        switch (code) {
            case Errc::broken_promise:
                return "broken promise";
            case Errc::future_already_retrieved:
                return "future already retrieved";
            case Errc::promise_already_satisfied:
                return "promise already satisfied";
            case Errc::no_state:
                return "no state associated with this future";
            default:
                return "unknown future error";
        }
    }

    /// @brief Throw the exception type corresponding to the given ErrorCode.
    /// @param errorCode  the ErrorCode instance
    /// @uptrace{SWS_CORE_00444}
    void ThrowAsException(const ErrorCode& errorCode) const noexcept(false) override
    {
        /// @uptrace{SWS_CORE_10953}
        ThrowOrTerminate<Exception>(errorCode);
    }
};

namespace internal
{
constexpr FutureErrorDomain g_futureErrorDomain;
}

/// @brief Obtain the reference to the single global FutureErrorDomain instance.
/// @returns reference to the FutureErrorDomain instance
/// @uptrace{SWS_CORE_00480}
/// @uptrace{SWS_CORE_10980}
/// @uptrace{SWS_CORE_10981}
/// @uptrace{SWS_CORE_10982}
/// @uptrace{SWS_CORE_10999}
inline constexpr const ErrorDomain& GetFutureErrorDomain() noexcept
{
    return internal::g_futureErrorDomain;
}

/// @brief Create a new ErrorCode for FutureErrorDomain with the given support data type and message.
/// @param code  an enumeration value from future_errc
/// @param data  a vendor-defined supplementary value
/// @returns the new ErrorCode instance
/// @uptrace{SWS_CORE_00490}
/// @uptrace{SWS_CORE_10990}
/// @uptrace{SWS_CORE_10991}
/// @uptrace{SWS_CORE_10999}
inline constexpr ErrorCode MakeErrorCode(future_errc code, ErrorDomain::SupportDataType data) noexcept
{
    return ErrorCode(static_cast<ErrorDomain::CodeType>(code), GetFutureErrorDomain(), data);
}

}  // namespace core
}  // namespace ara

#endif  // ARA_CORE_FUTURE_ERROR_DOMAIN_H_
