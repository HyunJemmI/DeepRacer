// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef PARA_SERIALIZATION_BASE_STATUS_H_
#define PARA_SERIALIZATION_BASE_STATUS_H_

#include <string>

#include "para/serialization/types/result.h"

// Status<T> is the return type used by the serialization engine to return either success and a value or an error
// indicating the nature of the failure. This type is based on the more general Result<ErrorEnum, T>.
namespace para
{
namespace serialization
{
enum class ErrorStatus
{
    None,                    // 0
    UnexpectedEncodingType,  // 1
    UnexpectedHandleType,    // 2
    UnexpectedVariantType,   // 3
    InvalidContainerLength,  // 4
    InvalidMemberCount,      // 5
    InvalidStringLength,     // 6
    InvalidTableHash,        // 7
    InvalidHandleReference,  // 8
    InvalidHandleValue,      // 9
    InvalidInterfaceMethod,  // 10
    DuplicateTableEntry,     // 11
    ReadLimitReached,        // 12
    WriteLimitReached,       // 13
    StreamError,             // 14
    ProtocolError,           // 15
    IOError,                 // 16
    SystemError,             // 17
    DebugError,              // 18
};

template <typename T>
struct Status : Result<ErrorStatus, T>
{
    using Result<ErrorStatus, T>::Result;

    const char* GetErrorMessage() const
    {
        switch (this->error()) {
            case ErrorStatus::None:
                return "No Error";
            case ErrorStatus::UnexpectedEncodingType:
                return "Unexpected Encoding Type";
            case ErrorStatus::UnexpectedHandleType:
                return "Unexpected Handle Type";
            case ErrorStatus::UnexpectedVariantType:
                return "Unexpected Variant Type";
            case ErrorStatus::InvalidContainerLength:
                return "Invalid Container Length";
            case ErrorStatus::InvalidMemberCount:
                return "Invalid Member Count";
            case ErrorStatus::InvalidStringLength:
                return "Invalid String Length";
            case ErrorStatus::InvalidTableHash:
                return "Invalid Table Hash";
            case ErrorStatus::InvalidHandleReference:
                return "Invalid Handle Reference";
            case ErrorStatus::InvalidHandleValue:
                return "Invalid Handle Value";
            case ErrorStatus::InvalidInterfaceMethod:
                return "Invalid Interface Method";
            case ErrorStatus::DuplicateTableEntry:
                return "Duplicate Table Hash";
            case ErrorStatus::ReadLimitReached:
                return "Read Limit Reached";
            case ErrorStatus::WriteLimitReached:
                return "Write Limit Reached";
            case ErrorStatus::StreamError:
                return "Stream Error";
            case ErrorStatus::ProtocolError:
                return "Protocol Error";
            case ErrorStatus::IOError:
                return "IO Error";
            case ErrorStatus::SystemError:
                return "System Error";
            case ErrorStatus::DebugError:
                return "Debug Error";
            default:
                return "Unknown Error";
        }
    }
};

}  // namespace serialization
}  // namespace para

#endif  // PARA_SERIALIZATION_BASE_STATUS_H_
