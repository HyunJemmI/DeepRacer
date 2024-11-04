// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef ARA_PHM_INTERNAL_RECOVERY_ACTION_IMPL_H_
#define ARA_PHM_INTERNAL_RECOVERY_ACTION_IMPL_H_

#include "ara/core/instance_specifier.h"
#include "ara/exec/execution_error_event.h"
#include "ara/phm/supervised_entity.h"

namespace ara
{
namespace phm
{

class RecoveryActionImpl final
{
  public:
    explicit RecoveryActionImpl(const ara::core::InstanceSpecifier& instanceSpecifier);
    virtual ~RecoveryActionImpl();
    ara::core::Result<void> Offer();
    void StopOffer();
    ara::core::Result<GlobalSupervisionStatus> GetGlobalSupervisionStatus();
    void SetActionHandler(std::function<void(const std::string&, const std::uint32_t&, std::uint32_t)> handler);

  private:
    // Forward declaration of the implementation class members
    class Member;
    // The implementation class members
    std::unique_ptr<Member> m;
};

}  // namespace phm
}  // namespace ara

#endif  // ARA_PHM_INTERNAL_RECOVERY_ACTION_IMPL_H_