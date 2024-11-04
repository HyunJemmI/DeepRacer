// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef ARA_LOG_COMMON_H_
#define ARA_LOG_COMMON_H_

#include <cstdint>

namespace ara
{
namespace log
{

/// @uptrace{SWS_LOG_00018}
enum class LogLevel : std::uint8_t
{
    kOff = 0x00,     /// No logging
    kFatal = 0x01,   /// Fatal error, not recoverable
    kError = 0x02,   /// Error with impact to correct functionality
    kWarn = 0x03,    /// Warning if correct behavior cannot be ensured
    kInfo = 0x04,    /// Informational, providing high level understanding
    kDebug = 0x05,   /// Detailed information for programmers
    kVerbose = 0x06  /// Extra-verbose debug messages (highest grade of information)
};

/// @uptrace{SWS_LOG_00019}
enum class LogMode : std::uint8_t
{
    kRemote = 0x01,  /// Sent remotely
    kFile = 0x02,    /// Save to file
    kConsole = 0x04  /// Forward to console
};

}  // namespace log
}  // namespace ara

#endif  // ARA_LOG_COMMON_H_

#if 0
namespace internal
{

/**
 * @brief LogReturn. Flags to define return values.
 * @note This is required to hide include/implementation of dlt on exported headers
 *
 * Maps one-by-one to the values from DltReturnValue.
 * @sa <dls/dlt_types.h>
 */
enum class LogReturnValue : int8_t
{
    kReturnLoggingDisabled = -7,
    kReturnUserBufferFull = -6,
    kReturnWrongParameter = -5,
    kReturnBufferFull = -4,
    kReturnPipeFull = -3,
    kReturnPipeError = -2,
    kReturnError = -1,
    kReturnOk = 0,
    kReturnTrue = 1
};

}  // namespace internal

inline std::ostream& operator<<(std::ostream& os, const LogLevel& level)
{
    switch (level) {
    case LogLevel::kOff:
        os << "Off";
        break;
    case LogLevel::kFatal:
        os << "Fatal";
        break;
    case LogLevel::kError:
        os << "Error";
        break;
    case LogLevel::kWarn:
        os << "Warn";
        break;
    case LogLevel::kInfo:
        os << "Info";
        break;
    case LogLevel::kDebug:
        os << "Debug";
        break;
    case LogLevel::kVerbose:
        os << "Verbose";
        break;
    }

    return os;
}

inline LogMode operator|(LogMode lhs, LogMode rhs)
{
    return (static_cast<LogMode>(static_cast<typename std::underlying_type<LogMode>::type>(lhs)
        | static_cast<typename std::underlying_type<LogMode>::type>(rhs)));
}

inline typename std::underlying_type<LogMode>::type operator&(LogMode lhs, LogMode rhs)
{
    return (static_cast<typename std::underlying_type<LogMode>::type>(lhs)
        & static_cast<typename std::underlying_type<LogMode>::type>(rhs));
}

/// @}
} /* namespace log */
} /* namespace ara */

#endif  // ARA_LOG_COMMON_H_
