// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef ARA_PER_UPDATE_H_
#define ARA_PER_UPDATE_H_

#include "ara/core/instance_specifier.h"

namespace ara
{
namespace per
{
//[SWS_PER_00356]
void RegisterApplicationDataUpdateCallback(
    std::function<void(const ara::core::InstanceSpecifier& storage, ara::core::String version)>
        appDataUpdateCallback) noexcept;

//[SWS_PER_00357]
ara::core::Result<void> UpdatePersistency() noexcept;

//[SWS_PER_00358]
ara::core::Result<void> ResetPersistency() noexcept;

}  // namespace per
}  // namespace ara

#endif  // ARA_PER_UPDATE_H_