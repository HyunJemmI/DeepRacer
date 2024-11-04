// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef ARA_PHM_HEALTH_CHANNEL_H_
#define ARA_PHM_HEALTH_CHANNEL_H_

#include <cstdint>
#include <type_traits>

#include "ara/core/instance_specifier.h"
#include "ara/phm/internal/health_channel_impl.h"
#include "ara/phm/phm_error_domain.h"

namespace ara
{
namespace phm
{

/// @brief HealthChannel Class.
/// @param EnumT  An enum type that contains health status Identifier
/// @uptrace{SWS_PHM_01132}
template <typename EnumT>
class HealthChannel
{
  public:
    /// @brief Creation of a HealthChannel.
    /// @param instance  instance specifier of the health channel.
    /// @uptrace{SWS_PHM_00457}
    explicit HealthChannel(const ara::core::InstanceSpecifier& instance)
        : mImpl(std::make_unique<HealthChannelImpl>(instance))
    {}

    /// @brief Destructor of a HealthChannel.
    /// @uptrace{SWS_PHM_01211}
    ~HealthChannel() noexcept = default;
    /// @brief The copy constructor for HealthChannel shall not be used.
    /// @uptrace{SWS_PHM_01222}
    HealthChannel(const HealthChannel& channel) = delete;

    /// @brief The copy assignment operator for HealthChannel shall not be used.
    /// @uptrace{SWS_PHM_01223}
    HealthChannel& operator=(const HealthChannel& channel) = delete;

    /// @brief Move constructor for HealthChannel.
    /// @param channel  The HealthChannel object to be moved.
    /// @uptrace{SWS_PHM_01224}
    HealthChannel(HealthChannel&& channel) noexcept = default;
    /// @brief Move assignment operator for HealthChannel.
    /// @param channel  The HealthChannel object to be moved.
    /// @return The moved HealthChannel object.
    /// @uptrace{SWS_PHM_01225}
    HealthChannel& operator=(HealthChannel&& channel) noexcept = default;

    /// @brief Reports a Health Status.
    /// @param healthStatusId  The identifier representing the Health Status.
    /// @return A Result, being either empty or containing an implementation specific error code
    /// @uptrace{SWS_PHM_01128}
    ara::core::Result<void> ReportHealthStatus(EnumT healthStatusId)
    {
        if (mImpl == nullptr) {
            // return error - health channel is stopped
            return ara::core::Result<void>(ara::phm::PhmErrc::kHealthStatusNotAvailable);
        }
        if (!std::is_enum<EnumT>::value) {
            // return error - health status id is not enum
            return ara::core::Result<void>(ara::phm::PhmErrc::kCheckpointNotAvailable);
        }
        // send health status - health status id to phm
        return mImpl->ReportHealthStatus(static_cast<std::uint32_t>(healthStatusId));
    }

  private:
    using IntegerType = typename std::underlying_type<EnumT>::type;

    // The implementation class
    std::unique_ptr<HealthChannelImpl> mImpl;
};

}  // namespace phm
}  // namespace ara

#endif  // ARA_PHM_HEALTH_CHANNEL_H_