// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef ARA_CORE_ERROR_DOMAIN_H_
#define ARA_CORE_ERROR_DOMAIN_H_

#include <cstdint>
#include <exception>

namespace ara
{
namespace core
{

#if defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnon-virtual-dtor"
#endif

// forward declaration
class ErrorCode;

/// @uptrace{SWS_CORE_00110}
/// @uptrace{SWS_CORE_10400}
class ErrorDomain
{
  public:
    /// @uptrace{SWS_CORE_00121}
    using IdType = std::uint64_t;
    /// @uptrace{SWS_CORE_00122}
    using CodeType = std::int32_t;
    /// @uptrace{SWS_CORE_00123}
    using SupportDataType = std::int32_t;

    /// @uptrace{SWS_CORE_00131}
    ErrorDomain(const ErrorDomain&) = delete;
    /// @uptrace{SWS_CORE_00132}
    ErrorDomain(ErrorDomain&&) = delete;
    /// @uptrace{SWS_CORE_00133}
    ErrorDomain& operator=(const ErrorDomain&) = delete;
    /// @uptrace{SWS_CORE_00134}
    ErrorDomain& operator=(ErrorDomain&&) = delete;

    /// @uptrace{SWS_CORE_00152}
    virtual const char* Name() const noexcept = 0;

    /// @uptrace{SWS_CORE_00153}
    virtual const char* Message(CodeType errorCode) const noexcept = 0;

    /**
     * @brief Throws the given errorCode as Exception
     * @param errorCode error code to be thrown
     * @remark if ARA_NO_EXCEPTIONS is defined, this function call will terminate.
     * @uptrace{SWS_CORE_00154}
     */
    [[noreturn]] virtual void ThrowAsException(const ErrorCode& errorCode) const noexcept(false) = 0;

    /// @uptrace{SWS_CORE_00151}
    /// @uptrace{SWS_CORE_10952}
    constexpr IdType Id() const noexcept
    {
        return mId;
    }

    /// @uptrace{SWS_CORE_00137}
    constexpr bool operator==(const ErrorDomain& other) const noexcept
    {
        return mId == other.mId;
    }

    /// @uptrace{SWS_CORE_00138}
    constexpr bool operator!=(const ErrorDomain& other) const noexcept
    {
        return mId != other.mId;
    }

  protected:
    /// @uptrace{SWS_CORE_00135}
    constexpr explicit ErrorDomain(IdType id) noexcept : mId(id)
    {}

    /// @brief Destructor
    /// @uptrace{SWS_CORE_00136}
    ~ErrorDomain() = default;

  private:
    const IdType mId;
};

#if defined(__GNUC__)
#pragma GCC diagnostic pop
#endif

}  // namespace core
}  // namespace ara

#endif  // ARA_CORE_ERROR_DOMAIN_H_
