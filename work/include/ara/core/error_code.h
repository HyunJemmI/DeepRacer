// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef ARA_CORE_ERROR_CODE_H_
#define ARA_CORE_ERROR_CODE_H_

#include <exception>
#include <ostream>
#include <type_traits>
#include <utility>

#include "ara/core/error_domain.h"
#include "ara/core/string_view.h"

namespace ara
{
namespace core
{

/// @uptrace{SWS_CORE_00501}
/// @uptrace{SWS_CORE_10300}
class ErrorCode
{
    friend std::ostream& operator<<(std::ostream& out, const ErrorCode& e)
    {
        return (out << e.mDomain->Name() << ":" << e.mValue << ":" << e.mSupportData);
    }

    using CodeType = ErrorDomain::CodeType;
    using SupportDataType = ErrorDomain::SupportDataType;

  public:
    /// @uptrace{SWS_CORE_00512}
    template <typename EnumT, typename = typename std::enable_if<std::is_enum<EnumT>::value>::type>
    constexpr ErrorCode(EnumT e, SupportDataType data = 0) noexcept : ErrorCode(MakeErrorCode(e, data))
    {}

    /// @uptrace{SWS_CORE_00513}
    constexpr ErrorCode(CodeType value, const ErrorDomain& domain, SupportDataType data = 0) noexcept
        : mValue(value), mSupportData(data), mDomain(&domain)
    {}

    /// @uptrace{SWS_CORE_00514}
    constexpr CodeType Value() const noexcept
    {
        return mValue;
    }

    /// @uptrace{SWS_CORE_00516}
    constexpr SupportDataType SupportData() const noexcept
    {
        return mSupportData;
    }

    /// @uptrace{SWS_CORE_00515}
    constexpr const ErrorDomain& Domain() const noexcept
    {
        return *mDomain;
    }

    /// @uptrace{SWS_CORE_00518}
    StringView Message() const noexcept
    {
        return Domain().Message(Value());
    }

    /// @uptrace{SWS_CORE_00519}
    [[noreturn]] void ThrowAsException() const noexcept(false)
    {
        Domain().ThrowAsException(*this);
        // Never reached, but apparently needed to avoid warnings from certain compilers (such as 5.4.0).
        std::terminate();
    }

  private:
    CodeType mValue;
    SupportDataType mSupportData;
    const ErrorDomain* mDomain;
};

/// @uptrace{SWS_CORE_00571}
constexpr inline bool operator==(const ErrorCode& lhs, const ErrorCode& rhs)
{
    return lhs.Domain() == rhs.Domain() && lhs.Value() == rhs.Value();
}

/// @uptrace{SWS_CORE_00572}
constexpr inline bool operator!=(const ErrorCode& lhs, const ErrorCode& rhs)
{
    return lhs.Domain() != rhs.Domain() || lhs.Value() != rhs.Value();
}

// Helper function for other Functional Clusters' error domains.
template <typename ExceptionType>
[[noreturn]] void ThrowOrTerminate(ErrorCode errorCode)
{
#ifndef ARA_NO_EXCEPTIONS
    throw ExceptionType(std::move(errorCode));
#else
    (void)errorCode;
    std::terminate();
#endif
}

}  // namespace core
}  // namespace ara

#endif  // ARA_CORE_ERROR_CODE_H_
