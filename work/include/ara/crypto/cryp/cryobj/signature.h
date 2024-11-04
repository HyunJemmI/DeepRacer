// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef ARA_CRYPTO_CRYP_CRYOBJ_SIGNATURE_H
#define ARA_CRYPTO_CRYP_CRYOBJ_SIGNATURE_H

#include <memory>

#include "ara/crypto/cryp/cryobj/crypto_object.h"
#include "ara/crypto/cryp/cryobj/crypto_primitive_id.h"

namespace ara
{
namespace crypto
{
namespace cryp
{

/// @brief Signature container interface This interface is applicable for keeping
///        the Digital Signature, Hash Digest, (Hash-based) Message Authentication Code (MAC/HMAC).
///        In case of a keyed signature (Digital Signature or MAC/HMAC) a COUID of the signature verification key can be
///        obtained by a call of CryptoObject::HasDependence()!
/// @uptrace{SWS_CRYPT_23300}
class Signature : public CryptoObject
{
  public:
    /// @brief Unique smart pointer of the interface.
    /// @uptrace{SWS_CRYPT_23301}
    using Uptrc = std::unique_ptr<const Signature>;

    /// @brief Get an ID of hash algorithm used for this signature object production.
    /// @return ID of used hash algorithm only (without signature algorithm specification)
    /// @uptrace{SWS_CRYPT_23311}
    virtual CryptoPrimitiveId::AlgId GetHashAlgId() const noexcept = 0;

    /// @brief Get the hash size required by current signature algorithm.
    /// @return required hash size in bytes
    /// @uptrace{SWS_CRYPT_23312}
    virtual std::size_t GetRequiredHashSize() const noexcept = 0;

    /// @brief Signature object initialized.
    /// @uptrace{SWS_CRYPT_23302}
    const CryptoObjectType kObjectType = CryptoObjectType::kSignature;
};

}  // namespace cryp
}  // namespace crypto
}  // namespace ara

#endif  // ARA_CRYPTO_CRYP_CRYOBJ_SIGNATURE_H