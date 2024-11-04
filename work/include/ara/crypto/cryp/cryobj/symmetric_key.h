// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef ARA_CRYPTO_CRYP_CRYOBJ_SYMMETRIC_KEY_H
#define ARA_CRYPTO_CRYP_CRYOBJ_SYMMETRIC_KEY_H

#include <memory>

#include "ara/crypto/cryp/cryobj/restricted_use_object.h"

namespace ara
{
namespace crypto
{
namespace cryp
{

/// @brief Symmetric Key interface.
/// @uptrace{SWS_CRYPT_23800}
class SymmetricKey : public RestrictedUseObject
{
  public:
    /// @brief Unique smart pointer of the interface.
    /// @uptrace{SWS_CRYPT_23801}
    using Uptrc = std::unique_ptr<const SymmetricKey>;

    /// @brief const object type
    /// @uptrace{SWS_CRYPT_23802}
    const CryptoObjectType kObjectType = CryptoObjectType::kSymmetricKey;
};

}  // namespace cryp
}  // namespace crypto
}  // namespace ara

#endif  // ARA_CRYPTO_CRYP_CRYOBJ_SYMMETRIC_KEY_H