// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef ARA_PHM_INTERNAL_SUPERVISED_ENTITY_IMPL_H_
#define ARA_PHM_INTERNAL_SUPERVISED_ENTITY_IMPL_H_

#include <ara/core/result.h>

#include "ara/core/instance_specifier.h"

namespace ara
{
namespace phm
{

enum class LocalSupervisionStatus : std::uint32_t;
enum class GlobalSupervisionStatus : std::uint32_t;

class SupervisedEntityImpl final
{
  public:
    SupervisedEntityImpl(const ara::core::InstanceSpecifier& instanceSpecifier);
    virtual ~SupervisedEntityImpl();
    ara::core::Result<void> ReportCheckpoint(std::uint32_t checkpoint);
    ara::core::Result<ara::phm::LocalSupervisionStatus> GetLocalSupervisionStatus();
    ara::core::Result<ara::phm::GlobalSupervisionStatus> GetGlobalSupervisionStatus();

  private:
    // Forward declaration of the implementation class members
    class Member;
    // The implementation class members
    std::unique_ptr<Member> m;
};

}  // namespace phm
}  // namespace ara

#endif  // ARA_PHM_INTERNAL_SUPERVISED_ENTITY_IMPL_H_