// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef ARA_PHM_INTERNAL_HEALTH_CHANNEL_ACTION_IMPL_H_
#define ARA_PHM_INTERNAL_HEALTH_CHANNEL_ACTION_IMPL_H_

#include "ara/core/instance_specifier.h"

namespace ara
{
namespace phm
{

class HealthChannelActionImpl final
{
  public:
    HealthChannelActionImpl(const ara::core::InstanceSpecifier& instanceSpecifier);
    virtual ~HealthChannelActionImpl();
    ara::core::Result<void> Offer();
    void StopOffer();
    void SetActionHandler(std::function<void(std::uint32_t)> handler);

  private:
    // Forward declaration of the implementation class members
    class Member;
    // The implementation class members
    std::unique_ptr<Member> m;
};

}  // namespace phm
}  // namespace ara

#endif  // ARA_PHM_INTERNAL_HEALTH_CHANNEL_ACTION_IMPL_H_