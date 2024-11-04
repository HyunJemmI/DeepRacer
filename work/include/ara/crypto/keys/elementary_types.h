// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef ARA_CRYPTO_KEYS_ELEMENTARY_TYPES_H
#define ARA_CRYPTO_KEYS_ELEMENTARY_TYPES_H

#include <cstdint>

#include "ara/core/result.h"
#include "ara/crypto/keys/keyslot.h"

namespace ara
{
namespace crypto
{
namespace keys
{

/// @brief Definition of a transaction identifier type. The zero value should be reserved for especial cases.
/// @uptrace{SWS_CRYPT_30010}
using TransactionId = std::uint64_t;

/// @brief Definition of a "transaction scope" type. The "transaction scope" defines a list of key slots that are target
/// for update in a transaction.
/// @uptrace{SWS_CRYPT_30011}
using TransactionScope = ara::core::Vector<KeySlot>;

}  // namespace keys
}  // namespace crypto
}  // namespace ara

#endif  // ARA_CRYPTO_KEYS_ELEMENTARY_TYPES_H
