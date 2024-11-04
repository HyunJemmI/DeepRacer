// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef ARA_CRYPTO_CRYP_CRYOBJ_PRIVATE_KEY_H
#define ARA_CRYPTO_CRYP_CRYOBJ_PRIVATE_KEY_H

#include <memory>

#include "ara/crypto/common/base_id_types.h"
#include "ara/crypto/cryp/cryobj/public_key.h"
#include "ara/crypto/cryp/cryobj/restricted_use_object.h"

namespace ara
{
namespace crypto
{
namespace cryp
{

/// @brief Generalized Asymmetric Private Key interface.
/// @uptrace{SWS_CRYPT_22500}
class PrivateKey : public RestrictedUseObject
{
  public:
    /// @brief Unique smart pointer of the interface.
    /// @uptrace{SWS_CRYPT_22501}
    using Uptrc = std::unique_ptr<const PrivateKey>;

    /// @brief Get the public key correspondent to this private key.
    /// @return unique smart pointer to the public key correspondent to this private key
    /// @uptrace{SWS_CRYPT_22511}
    virtual ara::core::Result<PublicKey::Uptrc> GetPublicKey() const noexcept = 0;

    /// @brief Static mapping of this interface to specific value of CryptoObjectType enumeration.
    /// @uptrace{SWS_CRYPT_22503}
    const CryptoObjectType kObjectType = CryptoObjectType::kPrivateKey;
};

}  // namespace cryp
}  // namespace crypto
}  // namespace ara

#endif  // ARA_CRYPTO_CRYP_CRYOBJ_PRIVATE_KEY_H