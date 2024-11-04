// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef ARA_CRYPTO_CRYP_KEY_DECAPSULATOR_PRIVATE_CTX_H
#define ARA_CRYPTO_CRYP_KEY_DECAPSULATOR_PRIVATE_CTX_H

#include <memory>

#include "ara/core/result.h"
#include "ara/crypto/common/mem_region.h"
#include "ara/crypto/cryp/cryobj/private_key.h"
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

/// @brief Asymmetric Key Encapsulation Mechanism (KEM) Private key Context interface.
/// @uptrace{SWS_CRYPT_21400}
class KeyDecapsulatorPrivateCtx : public CryptoContext
{
  public:
    /// @brief Unique smart pointer of the interface.
    /// @uptrace{SWS_CRYPT_21401}
    using Uptr = std::unique_ptr<KeyDecapsulatorPrivateCtx>;

    /// @brief Decapsulate Key Encryption Key (KEK).
    ///        Produced SymmetricKey object has following attributes: session, non-exportable, Key Usage:
    ///        kAllowKeyImporting. This method can be used for direct production of the target key, without creation of
    ///        the intermediate SecretSeed object.
    /// @param input an input buffer (its size should be equal GetEncapsulatedSize() bytes)
    /// @param kdf a context of a key derivation function, which should be used for the target KEK production
    /// @param kekAlgId an algorithm ID of the target KEK
    /// @param salt an optional salt value (if used, it should be unique for each instance of the target key)
    /// @param ctxLabel an optional application specific "context label" (it can identify purpose of the target key
    /// and/or communication parties)
    /// @return unique smart pointer to a symmetric key object derived from a key material decapsulated from the input
    /// block
    /// @uptrace{SWS_CRYPT_21412}
    virtual ara::core::Result<SymmetricKey::Uptrc> DecapsulateKey(
        ReadOnlyMemRegion input, KeyDerivationFunctionCtx& kdf, AlgId kekAlgId,
        ReadOnlyMemRegion salt = ReadOnlyMemRegion(),
        ReadOnlyMemRegion ctxLabel = ReadOnlyMemRegion()) const noexcept = 0;

    /// @brief Decapsulate key material. Returned Key Material object should be used for derivation of a symmetric key.
    ///        Produced SecretSeed object has following attributes: session, non-exportable, AlgID = this KEM AlgID.
    /// @param input a buffer with the encapsulated seed (its size should be equal GetEncapsulatedSize() bytes)
    /// @param allowedUsage the allowed usage scope of the target seed
    /// @return unique smart pointer to SecretSeed object, which keeps the key material decapsulated from the input
    /// buffer
    /// @uptrace{SWS_CRYPT_21411}
    virtual ara::core::Result<SecretSeed::Uptrc> DecapsulateSeed(
        ReadOnlyMemRegion input, SecretSeed::Usage allowedUsage = kAllowKdfMaterialAnyUsage) const noexcept = 0;

    /// @brief Get fixed size of the encapsulated data block.
    /// @return size of the encapsulated data block in bytes
    /// @uptrace{SWS_CRYPT_21416}
    virtual std::size_t GetEncapsulatedSize() const noexcept = 0;

    /// @brief Get ExtensionService instance.
    /// @return --
    /// @uptrace{SWS_CRYPT_21402}
    virtual ExtensionService::Uptr GetExtensionService() const noexcept = 0;

    /// @brief Get entropy (bit-length) of the key encryption key (KEK) material.
    ///        For RSA system the returned value corresponds to the length of module N (minus 1).
    ///        For DH-like system the returned value corresponds to the length of module q (minus 1).
    /// @return entropy of the KEK material in bits
    /// @uptrace{SWS_CRYPT_21415}
    virtual std::size_t GetKekEntropy() const noexcept = 0;

    /// @brief Clear the crypto context.
    /// @return --
    /// @uptrace{SWS_CRYPT_21414}
    virtual ara::core::Result<void> Reset() noexcept = 0;

    /// @brief Set (deploy) a key to the key decapsulator private algorithm context.
    /// @param key the source key object
    /// @return --
    /// @uptrace{SWS_CRYPT_21413}
    virtual ara::core::Result<void> SetKey(const PrivateKey& key) noexcept = 0;
};

}  // namespace cryp
}  // namespace crypto
}  // namespace ara

#endif  // ARA_CRYPTO_CRYP_KEY_DECAPSULATOR_PRIVATE_CTX_H
