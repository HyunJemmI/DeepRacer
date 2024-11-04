// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef ARA_LOG_STREAM_H_
#define ARA_LOG_STREAM_H_

#include <dlt/dlt_user.h>

#include <chrono>
#include <sstream>

#include "ara/core/instance_specifier.h"
#include "ara/core/span.h"
#include "ara/core/string_view.h"
#include "ara/log/common.h"

namespace ara
{
namespace log
{

/// @uptrace{SWS_LOG_00108}
struct LogHex8
{
    std::uint8_t mValue;
};

/// @uptrace{SWS_LOG_00109}
struct LogHex16
{
    std::uint16_t mValue;
};

/// @uptrace{SWS_LOG_00110}
struct LogHex32
{
    std::uint32_t mValue;
};

/// @uptrace{SWS_LOG_00111}
struct LogHex64
{
    std::uint64_t mValue;
};

/// @uptrace{SWS_LOG_00112}
struct LogBin8
{
    std::uint8_t mValue;
};

/// @uptrace{SWS_LOG_00113}
struct LogBin16
{
    std::uint16_t mValue;
};

/// @uptrace{SWS_LOG_00114}
struct LogBin32
{
    std::uint32_t mValue;
};

/// @uptrace{SWS_LOG_00111}
struct LogBin64
{
    std::uint64_t mValue;
};

/// @uptrace{SWS_LOG_00098}
enum class ClientState : int8_t
{
    kUnknown = -1,
    kNotConnected,
    kConnected
};

class Logger;

class LogStream
{
  public:
    LogStream() = delete;
    LogStream(LogLevel logLevel, Logger* logger) noexcept;
    virtual ~LogStream();
    LogStream(const LogStream&) = delete;
    LogStream& operator=(const LogStream&) = delete;
    LogStream(LogStream&&) noexcept = default;
    LogStream& operator=(LogStream&&) = delete;

    ///@uptrace{SWS_LOG_00039}
    void Flush() noexcept;
    ///@uptrace{SWS_LOG_00040}
    LogStream& operator<<(bool value) noexcept;
    ///@uptrace{SWS_LOG_00041}
    LogStream& operator<<(uint8_t value) noexcept;
    ///@uptrace{SWS_LOG_00042}
    LogStream& operator<<(uint16_t value) noexcept;
    ///@uptrace{SWS_LOG_00043}
    LogStream& operator<<(uint32_t value) noexcept;
    ///@uptrace{SWS_LOG_00044}
    LogStream& operator<<(uint64_t value) noexcept;
    ///@uptrace{SWS_LOG_00045}
    LogStream& operator<<(int8_t value) noexcept;
    ///@uptrace{SWS_LOG_00046}
    LogStream& operator<<(int16_t value) noexcept;
    ///@uptrace{SWS_LOG_00047}
    LogStream& operator<<(int32_t value) noexcept;
    ///@uptrace{SWS_LOG_00048}
    LogStream& operator<<(int64_t value) noexcept;
    ///@uptrace{SWS_LOG_00049}
    LogStream& operator<<(float value) noexcept;
    ///@uptrace{SWS_LOG_00050, 6044fa22c325c77c111bbca7bdc5027ba580748d}
    LogStream& operator<<(double value) noexcept;
    ///@uptrace{SWS_LOG_00053}
    LogStream& operator<<(const LogHex8& value) noexcept;
    ///@uptrace{SWS_LOG_00054}
    LogStream& operator<<(const LogHex16& value) noexcept;
    ///@uptrace{SWS_LOG_00055}
    LogStream& operator<<(const LogHex32& value) noexcept;
    ///@uptrace{SWS_LOG_00056}
    LogStream& operator<<(const LogHex64& value) noexcept;
    ///@uptrace{SWS_LOG_00057}
    LogStream& operator<<(const LogBin8& value) noexcept;
    ///@uptrace{SWS_LOG_00058}
    LogStream& operator<<(const LogBin16& value) noexcept;
    ///@uptrace{SWS_LOG_00059}
    LogStream& operator<<(const LogBin32& value) noexcept;
    ///@uptrace{SWS_LOG_00060}
    LogStream& operator<<(const LogBin64& value) noexcept;
    ///@uptrace{SWS_LOG_00062}
    LogStream& operator<<(const ara::core::StringView value) noexcept;
    ///@uptrace{SWS_LOG_00051}
    LogStream& operator<<(const char* const value) noexcept;
    ///@uptrace{SWS_LOG_00128, 4b15d6150bf77eacbb14bc0e6bfa5a1997265cda}
    LogStream& operator<<(ara::core::Span<const ara::core::Byte> data) noexcept;
    ///@uptrace{SWS_LOG_00129}
    LogStream& WithLocation(ara::core::StringView file, int line) noexcept;

    DltContextData* mDataPtr;
    DltReturnValue mDltRet;
};

///@uptrace{SWS_LOG_00063}
LogStream& operator<<(LogStream& out, LogLevel value) noexcept;
/// @uptrace{SWS_LOG_00124}
LogStream& operator<<(LogStream& out, const ara::core::ErrorCode& ec) noexcept;

/// @uptrace{SWS_LOG_00125}
template <typename Rep, typename Period>
inline LogStream& operator<<(LogStream& out, const std::chrono::duration<Rep, Period>& value) noexcept
{
    std::ostringstream s;

    // 'Period' is always a std::ratio<...>
    auto getUnit = [&s]() -> std::string {
        if (std::is_same<typename Period::type, std::nano>::value)
            return "ns";
        if (std::is_same<typename Period::type, std::micro>::value)
            return "us";
        if (std::is_same<typename Period::type, std::milli>::value)
            return "ms";
        if (std::is_same<typename Period::type, std::ratio<1>>::value)
            return "s";
        if (std::is_same<typename Period::type, std::centi>::value)
            return "cs";
        if (std::is_same<typename Period::type, std::deci>::value) {
            return "ds";
        }
        s << "(" << Period::num << "/" << Period::den << ")s";
        std::string unit = s.str();
        s.str("");
        return unit;
    };

    s << std::fixed;
    s << value.count() << getUnit();
    // TODO::    out << s.str();

    return out;
}

///@uptrace{SWS_LOG_00126}
LogStream& operator<<(LogStream& out, const ara::core::InstanceSpecifier& value) noexcept;

///@uptrace{SWS_LOG_00203}
// template <typename T>
// LogStream& operator<< (const Argument<T> &arg) noexcept;

///@uptrace{SWS_LOG_00204}
// template <typename MsgId, typename... Params>
// void Log(const MsgId &id, const Params &... args) noexcept;

}  // namespace log
}  // namespace ara

#endif  // ARA_LOG_STREAM_H_
