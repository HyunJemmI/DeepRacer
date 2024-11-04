// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef ARA_CRYPTO_CRYP_KEY_ENCAPSULATOR_PUBLIC_CTX_H
#define ARA_CRYPTO_CRYP_KEY_ENCAPSULATOR_PUBLIC_CTX_H

#include <memory>

#include "ara/core/result.h"
#include "ara/core/utility.h"
#include "ara/core/vector.h"
#include "ara/crypto/common/mem_region.h"
#include "ara/crypto/cryp/cryobj/public_key.h"
#include "ara/crypto/cryp/cryobj/restricted_use_object.h"
#include "ara/crypto/cryp/crypto_context.h"
#include "ara/crypto/cryp/extension_service.h"

namespace ara
{
namespace crypto
{
namespace cryp
{

/// @brief Asymmetric Key Encapsulation Mechanism (KEM) Public key Context interface.
/// @uptrace{SWS_CRYPT_21800}
class KeyEncapsulatorPublicCtx : public CryptoContext
{
  public:
    /// @brief Unique smart pointer of the interface.
    /// @uptrace{SWS_CRYPT_21801}
    using Uptr = std::unique_ptr<KeyEncapsulatorPublicCtx>;

    /// @brief Get fixed size of the encapsulated data block.
    /// @return size of the encapsulated data block in bytes
    /// @uptrace{SWS_CRYPT_21818}
    virtual std::size_t GetEncapsulatedSize() const noexcept = 0;

    /// @brief Get ExtensionService instance.
    /// @return --
    /// @uptrace{SWS_CRYPT_21802}
    virtual ExtensionService::Uptr GetExtensionService() const noexcept = 0;

    /// @brief Get entropy (bit-length) of the key encryption key (KEK) material.
    ///        For RSA system the returned value corresponds to the length of module N (minus 1).
    ///        For DH-like system the returned value corresponds to the length of module q (minus 1).
    /// @return entropy of the KEK material in bits
    /// @uptrace{SWS_CRYPT_21817}
    virtual std::size_t GetKekEntropy() const noexcept = 0;

    /// @brief Add the content to be encapsulated (payload) according to RFC 5990 ("keying data").
    ///        At the moment only SymmetricKey and SecretSeed objects are supported.
    /// @param keyingData the payload to be protected
    /// @return --
    /// @uptrace{SWS_CRYPT_21810}
    virtual ara::core::Result<void> AddKeyingData(RestrictedUseObject& keyingData) noexcept = 0;

    /// @brief Encapsulate Key Encryption Key (KEK).
    /// @param kdf a context of a key derivation function, which should be used for the target KEK production
    /// @param kekAlgId an algorithm ID of the target KEK
    /// @param salt an optional salt value (if used, it should be unique for each instance of the target key)
    /// @param ctxLabel an optional application specific "context label" (it can identify purpose of the target key
    /// and/or communication parties)
    /// @return unique smart pointer to a symmetric key object derived from a randomly generated material encapsulated
    /// to the output buffer.
    ///         Only first GetEncapsulatedSize() bytes of the output buffer should be updated by this method.
    ///         Produced SymmetricKey object has following attributes: session, non-exportable, Allowed Key Usage:
    ///         kAllowKeyExporting. This method can be used for direct production of the target key, without creation of
    ///         the intermediate SecretSeed object.
    /// @uptrace{SWS_CRYPT_21813}
    virtual ara::core::Result<ara::core::Vector<ara::core::Byte>> Encapsulate(
        KeyDerivationFunctionCtx& kdf, AlgId kekAlgId, ReadOnlyMemRegion salt = ReadOnlyMemRegion(),
        ReadOnlyMemRegion ctxLabel = ReadOnlyMemRegion()) const noexcept = 0;

    /// @brief Clear the crypto context.
    /// @return --
    /// @uptrace{SWS_CRYPT_21816}
    virtual ara::core::Result<void> Reset() noexcept = 0;

    /// @brief Set (deploy) a key to the key encapsulator public algorithm context.
    /// @param key the source key object
    /// @return --
    /// @uptrace{SWS_CRYPT_21815}
    virtual ara::core::Result<void> SetKey(const PublicKey& key) noexcept = 0;
};

}  // namespace cryp
}  // namespace crypto
}  // namespace ara

#endif  // ARA_CRYPTO_CRYP_KEY_ENCAPSULATOR_PUBLIC_CTX_H