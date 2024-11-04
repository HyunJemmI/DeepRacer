// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef ARA_PER_RECOVERY_H_
#define ARA_PER_RECOVERY_H_

#include "ara/core/instance_specifier.h"
#include "ara/core/vector.h"

namespace ara
{
namespace per
{

//[SWS_PER_00432]
enum class RecoveryReportKind : std::uint32_t
{
    kKeyValueStorageRecoveryFailed = 1,
    kKeyValueStorageRecovered = 2,
    kKeyRecoveryFailed = 3,
    kKeyRecovered = 4,
    kFileStorageRecoveryFailed = 5,
    kFileStorageRecovered = 6,
    kFileRecoveryFailed = 7,
    kFileRecovered = 8
};

//[SWS_PER_00433]
void RegisterRecoveryReportCallback(
    std::function<void(const ara::core::InstanceSpecifier& storage, ara::per::RecoveryReportKind recoveryReportKind,
                       ara::core::Vector<ara::core::String> reportedElements,
                       ara::core::Vector<std::uint8_t> reportedInstances)>
        recoveryReportCallback) noexcept;

}  // namespace per
}  // namespace ara

#endif  // ARA_PER_RECOVERY_H_