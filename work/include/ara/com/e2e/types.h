// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef ARA_COM_E2E_TYPES_H_
#define ARA_COM_E2E_TYPES_H_

#include <iostream>

namespace ara
{
namespace com
{
namespace e2e
{

using DataID = std::uint64_t;
using MessageCounter = std::uint64_t;

/// @uptrace{SWS_CM_90421}
enum class ProfileCheckStatus : uint8_t
{
    kOk,
    kRepeated,
    kWrongSequence,
    kError,
    kNotAvailable,
    kNoNewData,
    kCheckDisabled,
    kNone
};

/// @uptrace{SWS_CM_90422}
enum class SMState : uint8_t
{
    kValid,
    kNoData,
    kInit,
    kInvalid,
    kStateMDisabled
};

/// @uptrace{PRS_E2E_00322}
struct Result
{
    ProfileCheckStatus mE2EStatus;
    SMState mE2EState;
};

/// @uptrace{PRS_E2E_00673}
enum class ErrorCode : uint8_t
{
    E2E_E_OK = 0,
    E2E_E_INPUTERR_WRONG = 1,
    E2E_E_INPUTERR_NULL = 2,
};

/// @uptrace{PRS_E2E_00739}
enum class MessageType : uint8_t
{
    STD_MESSAGETYPE_REQUEST = 0,
    STD_MESSAGETYPE_RESPONSE = 1,
    NONE = 2,
};

enum class MessageResult : uint8_t
{
    STD_MESSAGERESULT_OK = 0,
    STD_MESSAGERESULT_ERROR = 1,
    NONE = 2,
};

}  // namespace e2e
}  // namespace com
}  // namespace ara

#endif  // ARA_COM_E2E_ERROR_DOMAIN_H_