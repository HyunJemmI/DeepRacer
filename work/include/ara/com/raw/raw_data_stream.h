// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef ARA_COM_RAW_RAW_DATA_STREAM_H_
#define ARA_COM_RAW_RAW_DATA_STREAM_H_

#include <chrono>

#include "ara/com/sample_ptr.h"
#include "ara/core/instance_specifier.h"

namespace ara
{
namespace com
{
namespace raw
{

/// @uptrace{SWS_CM_11300}
struct ReadDataResult
{
    /// @uptrace{SWS_CM_11301}
    ara::com::SamplePtr<std::uint8_t> data;
    /// @uptrace{SWS_CM_11302}
    std::size_t numberOfBytes;
};

/// @uptrace{SWS_CM_10481}
class RawDataStreamClient final
{
  public:
    /// @uptrace{SWS_CM_10482,SWS_CM_10476,SWS_CM_99004}
    explicit RawDataStreamClient(const ara::core::InstanceSpecifier& instance);
    /// @uptrace{SWS_CM_10483}
    ~RawDataStreamClient() noexcept;
    /// @uptrace{SWS_CM_11303}
    RawDataStreamClient(const RawDataStreamClient&) = delete;
    /// @uptrace{SWS_CM_11304}
    RawDataStreamClient& operator=(const RawDataStreamClient&) = delete;
    /// @uptrace{SWS_CM_11305}
    RawDataStreamClient(RawDataStreamClient&& other) noexcept;
    /// @uptrace{SWS_CM_11306}
    RawDataStreamClient& operator=(RawDataStreamClient&& other) noexcept;

    /// @uptrace{SWS_CM_10484,SWS_CM_10477}
    /// @brief: Sets up a socket connection for the RawDataStream defined by the instance, and establish connection to
    /// the TCP server.
    ara::core::Result<void> Connect() noexcept;
    /// @uptrace{SWS_CM_11307,SWS_CM_99006}
    ara::core::Result<void> Connect(std::chrono::milliseconds timeout) noexcept;
    /// @uptrace{SWS_CM_10485,SWS_CM_10478}
    ara::core::Result<void> Shutdown() noexcept;
    /// @uptrace{SWS_CM_10308,SWS_CM_99006}
    ara::core::Result<void> Shutdown(std::chrono::milliseconds timeout) noexcept;
    /// @uptrace{SWS_CM_10486,SWS_CM_10479}
    ara::core::Result<ReadDataResult> ReadData(std::size_t maxLength) noexcept;
    /// @uptrace{SWS_CM_11309,SWS_CM_99006}
    ara::core::Result<ReadDataResult> ReadData(std::size_t maxLength, std::chrono::milliseconds timeout) noexcept;
    /// @uptrace{SWS_CM_10487,SWS_CM_10480}
    ara::core::Result<std::size_t> WriteData(ara::com::SamplePtr<std::uint8_t> data, std::size_t maxLength) noexcept;
    /// @uptrace{SWS_CM_11310,SWS_CM_99006}
    ara::core::Result<std::size_t> WriteData(ara::com::SamplePtr<std::uint8_t> data, std::size_t maxLength,
                                             std::chrono::milliseconds timeout) noexcept;

  private:
    // Forward declaration of the implementation class
    class Impl;
    // The implementation class
    std::unique_ptr<Impl> mImpl;
};

/// @uptrace{SWS_CM_11311}
class RawDataStreamServer final
{
  public:
    /// @uptrace{SWS_CM_11312,SWS_CM_10476,SWS_CM_99004}
    explicit RawDataStreamServer(const ara::core::InstanceSpecifier& instance);
    /// @uptrace{SWS_CM_11313}
    ~RawDataStreamServer() noexcept;
    /// @uptrace{SWS_CM_11314}
    RawDataStreamServer(const RawDataStreamServer&) = delete;
    /// @uptrace{SWS_CM_11315}
    RawDataStreamServer& operator=(const RawDataStreamServer&) = delete;
    /// @uptrace{SWS_CM_11316}
    RawDataStreamServer(RawDataStreamServer&& other) noexcept;
    /// @uptrace{SWS_CM_11317}
    RawDataStreamServer& operator=(RawDataStreamServer&& other) noexcept;

    /// @uptrace{SWS_CM_11318,SWS_CM_99005}
    ara::core::Result<void> WaitForConnection() noexcept;
    /// @uptrace{SWS_CM_11319,SWS_CM_99006}
    ara::core::Result<void> WaitForConnection(std::chrono::milliseconds timeout) noexcept;
    /// @uptrace{SWS_CM_11320,SWS_CM_10478}
    ara::core::Result<void> Shutdown() noexcept;
    /// @uptrace{SWS_CM_11321,SWS_CM_99006}
    ara::core::Result<void> Shutdown(std::chrono::milliseconds timeout) noexcept;
    /// @uptrace{SWS_CM_11322,SWS_CM_10479}
    ara::core::Result<ReadDataResult> ReadData(std::size_t maxLength) noexcept;
    /// @uptrace{SWS_CM_11323,SWS_CM_99006}
    ara::core::Result<ReadDataResult> ReadData(std::size_t maxLength, std::chrono::milliseconds timeout) noexcept;
    /// @uptrace{SWS_CM_11324,SWS_CM_10480}
    ara::core::Result<std::size_t> WriteData(ara::com::SamplePtr<std::uint8_t> data, std::size_t maxLength) noexcept;
    /// @uptrace{SWS_CM_11325,SWS_CM_99006}
    ara::core::Result<std::size_t> WriteData(ara::com::SamplePtr<std::uint8_t> data, std::size_t maxLength,
                                             std::chrono::milliseconds timeout) noexcept;

  private:
    // Forward declaration of the implementation class
    class Impl;
    // The implementation class
    std::unique_ptr<Impl> mImpl;
};

}  // namespace raw
}  // namespace com
}  // namespace ara

#endif  // ARA_COM_RAW_RAW_DATA_STREAM_H_