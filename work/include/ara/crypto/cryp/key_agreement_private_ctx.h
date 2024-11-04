// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef ARA_CRYPTO_CRYP_KEY_AGREEMENT_PRIVATE_CTX_H
#define ARA_CRYPTO_CRYP_KEY_AGREEMENT_PRIVATE_CTX_H

#include <memory>

#include "ara/core/result.h"
#include "ara/crypto/common/mem_region.h"
#include "ara/crypto/cryp/cryobj/private_key.h"
#include "ara/crypto/cryp/cryobj/public_key.h"
#include "ara/crypto/cryp/cryobj/secret_seed.h"
#include "ara/crypto/cryp/cryobj/symmetric_key.h"
#include "ara/crypto/cryp/crypto_context.h"
#include "ara/crypto/cryp/extension_service.h"
#include "ara/crypto/cryp/key_derivation_function_ctx.h"

namespace ara
{
namespace crypto
{
namespace cryp
{

/// @brief Key Agreement Private key Context interface (Diffie Hellman or conceptually similar).
/// @uptrace{SWS_CRYPT_21300}
class KeyAgreementPrivateCtx : public CryptoContext
{
  public:
    /// @brief Unique smart pointer of this interface.
    /// @uptrace{SWS_CRYPT_21301}
    using Uptr = std::unique_ptr<KeyAgreementPrivateCtx>;

    /// @brief Produce a common symmetric key via execution of the key-agreement algorithm between this private key and
    /// a public key of another side.
    ///        Produced SymmetricKey object has following attributes: session, non-exportable.
    ///        This method can be used for direct production of the target key, without creation of the intermediate
    ///        SecretSeed object.
    /// @param otherSideKey the public key of the other side of the Key-Agreement
    /// @param kdf the Context of a Key Derivation Function, which should be used for the target key production
    /// @param targetAlgId identifier of the target symmetric algorithm (also defines a target key-length)
    /// @param allowedUsage the allowed usage scope of the target key
    /// @param salt an optional salt value (if used, it should be unique for each instance of the target key)
    /// @param ctxLabel an optional application specific "context label" (it can identify purpose of the target key
    /// and/or communication parties)
    /// @return a unique pointer to SecretSeed object, which contains the key material produced by the Key-Agreement
    /// algorithm
    /// @uptrace{SWS_CRYPT_21312}
    virtual ara::core::Result<SymmetricKey::Uptrc> AgreeKey(
        const PublicKey& otherSideKey, KeyDerivationFunctionCtx& kdf, AlgId targetAlgId, AllowedUsageFlags allowedUsage,
        ReadOnlyMemRegion salt = ReadOnlyMemRegion(),
        ReadOnlyMemRegion ctxLabel = ReadOnlyMemRegion()) const noexcept = 0;

    /// @brief Produce a common secret seed via execution of the key-agreement algorithm between this private key and a
    /// public key of another side.
    ///        Produced SecretSeed object has following attributes: session, non-exportable, AlgID (this Key-Agreement
    ///        Algorithm ID).
    /// @param otherSideKey the public key of the other side of the Key-Agreement
    /// @param allowedUsage the allowed usage scope of the target seed
    /// @return unique pointer to SecretSeed object, which contains the key material produced by the Key-Agreement
    /// algorithm
    /// @uptrace{SWS_CRYPT_21311}
    virtual ara::core::Result<SecretSeed::Uptrc> AgreeSeed(
        const PublicKey& otherSideKey, SecretSeed::Usage allowedUsage = kAllowKdfMaterialAnyUsage) const noexcept = 0;

    /// @brief Get ExtensionService instance.
    /// @return --
    /// @uptrace{SWS_CRYPT_21302}
    virtual ExtensionService::Uptr GetExtensionService() const noexcept = 0;

    /// @brief Clear the crypto context.
    /// @return --
    /// @uptrace{SWS_CRYPT_21314}
    virtual ara::core::Result<void> Reset() noexcept = 0;

    /// @brief Set (deploy) a key to the key agreement private algorithm context.
    /// @param key the source key object
    /// @return --
    /// @uptrace{SWS_CRYPT_21313}
    virtual ara::core::Result<void> SetKey(const PrivateKey& key) noexcept = 0;
};

}  // namespace cryp
}  // namespace crypto
}  // namespace ara

#endif  // ARA_CRYPTO_CRYP_KEY_AGREEMENT_PRIVATE_CTX_H