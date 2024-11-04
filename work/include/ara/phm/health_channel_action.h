
// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef ARA_PHM_HEALTH_CHANNEL_ACTION_H_
#define ARA_PHM_HEALTH_CHANNEL_ACTION_H_

#include "ara/core/instance_specifier.h"
#include "ara/phm/internal/health_channel_action_impl.h"
#include "ara/phm/phm_error_domain.h"

namespace ara
{
namespace phm
{

/// @uptrace{SWS_PHM_01139}
template <typename EnumT>
class HealthChannelAction
{
  public:
    /// @uptrace{SWS_PHM_01231}
    explicit HealthChannelAction(const ara::core::InstanceSpecifier& instance)
        : mImpl(std::make_unique<HealthChannelActionImpl>(instance))
    {
        auto handler = [this](std::uint32_t Id) -> void {
            this->RecoveryHandler(static_cast<EnumT>(Id));
        };
        mImpl->SetActionHandler(handler);
    }

    /// @uptrace{SWS_PHM_01232}
    ~HealthChannelAction() noexcept = default;

    /// @uptrace{SWS_PHM_01233}
    HealthChannelAction(HealthChannelAction&& hca) noexcept
    {
        // move ctor
        mImpl = std::move(hca.mImpl);
        auto handler = [this](std::uint32_t Id) -> void {
            this->RecoveryHandler(static_cast<EnumT>(Id));
        };
        mImpl->SetActionHandler(handler);
        hca.mImpl = nullptr;
    }

    /// @uptrace{SWS_PHM_01235}
    HealthChannelAction& operator=(HealthChannelAction&& hca) noexcept
    {
        // move operator
        mImpl = std::move(hca.mImpl);
        auto handler = [this](std::uint32_t Id) -> void {
            this->RecoveryHandler(static_cast<EnumT>(Id));
        };
        mImpl->SetActionHandler(handler);
        hca.mImpl = nullptr;
        return *this;
    }

    /// @uptrace{SWS_PHM_01234}
    HealthChannelAction(const HealthChannelAction& hca) = delete;
    /// @uptrace{SWS_PHM_01236}
    HealthChannelAction& operator=(const HealthChannelAction& hca) = delete;

    /// @uptrace{SWS_PHM_01238}
    ara::core::Result<void> Offer()
    {
        return mImpl->Offer();
    }

    /// @uptrace{SWS_PHM_01239}
    void StopOffer()
    {
        // disconnect with phm
        mImpl->StopOffer();
    }

    /// @uptrace{SWS_PHM_01237}
    virtual void RecoveryHandler(EnumT healthStatusId) = 0;

  private:
    //    using IntegerType = typename std::underlying_type<EnumT>::type;

    // The implementation class
    std::unique_ptr<HealthChannelActionImpl> mImpl;
};

}  // namespace phm
}  // namespace ara

#endif  // ARA_PHM_HEALTH_CHANNEL_ACTION_H_