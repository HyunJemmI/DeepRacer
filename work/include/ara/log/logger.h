// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef ARA_LOG_LOGGER_H_
#define ARA_LOG_LOGGER_H_

#include <atomic>

#include "ara/core/string_view.h"
#include "ara/log/log_stream.h"

namespace ara
{
namespace log
{

class Logger final
{
  public:
    Logger() = default;
    Logger(ara::core::StringView ctxId, ara::core::StringView ctxDescription, LogLevel ctxDefLogLevel);
    virtual ~Logger();

    /// @uptrace{SWS_LOG_00064}
    LogStream LogFatal() const noexcept;
    /// @uptrace{SWS_LOG_00065}
    LogStream LogError() const noexcept;
    /// @uptrace{SWS_LOG_00066}
    LogStream LogWarn() const noexcept;
    /// @uptrace{SWS_LOG_00067}
    LogStream LogInfo() const noexcept;
    /// @uptrace{SWS_LOG_00068}
    LogStream LogDebug() const noexcept;
    /// @uptrace{SWS_LOG_00069}
    LogStream LogVerbose() const noexcept;
    /// @uptrace{SWS_LOG_00070}
    bool IsEnabled(LogLevel logLevel) const noexcept;
    /// @uptrace{SWS_LOG_00131}
    LogStream WithLevel(LogLevel logLevel) const noexcept;

    DltContext* GetContext();

  private:
    DltContext mContext;
    std::atomic<bool> mInitiated{false};
};

/// Vendor Specific
Logger& DefaultLogger();

/// @uptrace{SWS_LOG_00021}
Logger& CreateLogger(ara::core::StringView ctxId, ara::core::StringView ctxDescription,
                     LogLevel ctxDefLogLevel = LogLevel::kWarn) noexcept;

/// @uptrace{SWS_LOG_00022}
constexpr LogHex8 HexFormat(uint8_t value) noexcept
{
    return LogHex8{value};
}

/// @uptrace{SWS_LOG_00023}
constexpr LogHex8 HexFormat(int8_t value) noexcept
{
    return LogHex8{static_cast<uint8_t>(value)};
}

/// @uptrace{SWS_LOG_00024}
constexpr LogHex16 HexFormat(uint16_t value) noexcept
{
    return LogHex16{value};
}

/// @uptrace{SWS_LOG_00025}
constexpr LogHex16 HexFormat(int16_t value) noexcept
{
    return LogHex16{static_cast<uint16_t>(value)};
}

/// @uptrace{SWS_LOG_00026}
constexpr LogHex32 HexFormat(uint32_t value) noexcept
{
    return LogHex32{value};
}

/// @uptrace{SWS_LOG_00027}
constexpr LogHex32 HexFormat(int32_t value) noexcept
{
    return LogHex32{static_cast<uint32_t>(value)};
}

/// @uptrace{SWS_LOG_00028}
constexpr LogHex64 HexFormat(uint64_t value) noexcept
{
    return LogHex64{value};
}

/// @uptrace{SWS_LOG_00029}
constexpr LogHex64 HexFormat(int64_t value) noexcept
{
    return LogHex64{static_cast<uint64_t>(value)};
}

/// @uptrace{SWS_LOG_00030}
constexpr LogBin8 BinFormat(uint8_t value) noexcept
{
    return LogBin8{value};
}

/// @uptrace{SWS_LOG_00031}
constexpr LogBin8 BinFormat(int8_t value) noexcept
{
    return LogBin8{static_cast<uint8_t>(value)};
}

/// @uptrace{SWS_LOG_00032}
constexpr LogBin16 BinFormat(uint16_t value) noexcept
{
    return LogBin16{value};
}

/// @uptrace{SWS_LOG_00033}
constexpr LogBin16 BinFormat(int16_t value) noexcept
{
    return LogBin16{static_cast<uint16_t>(value)};
}

/// @uptrace{SWS_LOG_00034}
constexpr LogBin32 BinFormat(uint32_t value) noexcept
{
    return LogBin32{value};
}

/// @uptrace{SWS_LOG_00035}
constexpr LogBin32 BinFormat(int32_t value) noexcept
{
    return LogBin32{static_cast<uint32_t>(value)};
}

/// @uptrace{SWS_LOG_00036}
constexpr LogBin64 BinFormat(uint64_t value) noexcept
{
    return LogBin64{value};
}

/// @uptrace{SWS_LOG_00037}
constexpr LogBin64 BinFormat(int64_t value) noexcept
{
    return LogBin64{static_cast<uint64_t>(value)};
}

/// @uptrace{SWS_LOG_00101}
ClientState remoteClientState() noexcept;

/// @uptrace{SWS_LOG_00201}
// template <typename T>
// Argument<T> Arg (T&& arg, const char* name=nullptr, const char* unit=nullptr) noexcept;

}  // namespace log
}  // namespace ara

#endif  // ARA_LOG_LOGGER_H_
