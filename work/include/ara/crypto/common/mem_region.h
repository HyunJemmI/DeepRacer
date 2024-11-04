// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef ARA_CRYPTO_COMMON_MEM_REGION_H
#define ARA_CRYPTO_COMMON_MEM_REGION_H

#include <cstdint>

#include "ara/core/span.h"

namespace ara
{
namespace crypto
{

/// @brief Read-Only Memory Region (intended for [in] arguments)
/// @uptrace{SWS_CRYPT_10033}
using ReadOnlyMemRegion = ara::core::Span<const std::uint8_t>;

/// @brief Read-Write Memory Region (intended for [in/out] arguments)
/// @uptrace{SWS_CRYPT_10031}
using ReadWriteMemRegion = ara::core::Span<std::uint8_t>;

}  // namespace crypto
}  // namespace ara

#endif  // ARA_CRYPTO_COMMON_MEM_REGION_H