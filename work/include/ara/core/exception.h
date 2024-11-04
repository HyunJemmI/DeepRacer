// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef ARA_CORE_EXCEPTIONS_H
#define ARA_CORE_EXCEPTIONS_H

#include <exception>
#include <ostream>
#include <system_error>
#include <utility>

#include "ara/core/error_code.h"

namespace ara
{
namespace core
{
/// @uptrace{SWS_CORE_00601}
class Exception : public std::exception
{
    const ErrorCode mErrorCode;

  public:
    /// @uptrace{SWS_CORE_00611}
    explicit Exception(ErrorCode err) noexcept : mErrorCode(std::move(err))
    {}

    /// @uptrace{SWS_CORE_00613}
    const ErrorCode& Error() const noexcept
    {
        return mErrorCode;
    }

    /// @uptrace{SWS_CORE_00612}
    const char* what() const noexcept override
    {
        return std::exception::what();
    }

  protected:
    /// @uptrace{SWS_CORE_00614}
    Exception& operator=(const Exception& other) = default;
};

}  // namespace core
}  // namespace ara

#endif  // ARA_CORE_EXCEPTIONS_H
