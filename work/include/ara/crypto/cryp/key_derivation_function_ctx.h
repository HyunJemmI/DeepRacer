// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef ARA_CRYPTO_CRYP_KEY_DERIVATION_FUNCTION_CTX_H
#define ARA_CRYPTO_CRYP_KEY_DERIVATION_FUNCTION_CTX_H

#include <memory>

#include "ara/core/result.h"
#include "ara/crypto/common/mem_region.h"
#include "ara/crypto/cryp/cryobj/restricted_use_object.h"
#include "ara/crypto/cryp/cryobj/secret_seed.h"
#include "ara/crypto/cryp/cryobj/symmetric_key.h"
#include "ara/crypto/cryp/crypto_context.h"

namespace ara
{
namespace crypto
{
namespace cryp
{

/// @brief Key Derivation Function interface.
/// @uptrace{SWS_CRYPT_21500}
class KeyDerivationFunctionCtx : public CryptoContext
{
  public:
    /// @brief Unique smart pointer of the interface.
    /// @uptrace{SWS_CRYPT_21501}
    using Uptr = std::unique_ptr<KeyDerivationFunctionCtx>;

    /// @brief Add an application filler value stored in a (non-secret) ReadOnlyMemRegion.
    ///        If (GetFillerSize() == 0), then this method call will be ignored.
    ///        Add a secret application filler value stored in a SecretSeed object.
    ///        If (GetFillerSize() == 0), then this method call will be ignored.
    ///        Add a salt value stored in a (non-secret) ReadOnlyMemRegion.
    /// @param salt a salt value (if used, it should be unique for each instance of the target key)
    /// @return --
    /// @uptrace{SWS_CRYPT_21510}
    virtual ara::core::Result<void> AddSalt(ReadOnlyMemRegion salt) noexcept = 0;

    /// @brief Add a secret salt value stored in a SecretSeed object.
    /// @param salt a salt value (if used, it should be unique for each instance of the target key)
    /// @return --
    /// @uptrace{SWS_CRYPT_21513}
    virtual ara::core::Result<void> AddSecretSalt(const SecretSeed& salt) noexcept = 0;

    /// @brief Configure the number of iterations that will be applied by default.
    ///        Implementation can restrict minimal and/or maximal value of the iterations number.
    /// @param iterations the required number of iterations of the base function (0 means implementation default number)
    /// @return actual number of the iterations configured in the context now (after this method call)
    /// @uptrace{SWS_CRYPT_21514}
    virtual std::uint32_t ConfigIterations(std::uint32_t iterations = 0) noexcept = 0;

    /// @brief Derive a symmetric key from the provided key material and provided context configuration.
    /// @param isSession the "session" (or "temporary") attribute for the target key (if true)
    /// @param isExportable the exportability attribute for the target key (if true)
    /// @return unique smart pointer to the created instance of derived symmetric key
    /// @uptrace{SWS_CRYPT_21515}
    virtual ara::core::Result<SymmetricKey::Uptrc> DeriveKey(bool isSession = true,
                                                             bool isExportable = false) const noexcept = 0;

    /// @brief Derive a "slave" key material (secret seed) from the provided "master" key material and provided context
    /// configuration.
    /// @param isSession the "session" (or "temporary") attribute for the target key (if true)
    /// @param isExportable the exportability attribute for the target key (if true)
    /// @return unique smart pointer to the created SecretSeed object
    /// @uptrace{SWS_CRYPT_21516}
    virtual ara::core::Result<SecretSeed::Uptrc> DeriveSeed(bool isSession = true,
                                                            bool isExportable = false) const noexcept = 0;

    /// @brief Clear the crypto context.
    /// @return --
    /// @uptrace{SWS_CRYPT_21524}
    virtual ara::core::Result<void> Reset() noexcept = 0;

    /// @brief Get ExtensionService instance.
    /// @return --
    /// @uptrace{SWS_CRYPT_21517}
    virtual ExtensionService::Uptr GetExtensionService() const noexcept = 0;

    /// @brief Get the fixed size of an application specific "filler" required by this context instance.
    ///        If this instance of the key derivation context does not support filler values, 0 shall be returned.
    ///        Get the fixed size of the target key ID required by diversification algorithm.
    ///        Returned value is constant for each instance of the interface, i.e. independent from configuration by
    /// @return size of the application specific filler in bytes Returned value is constant for this instance of the key
    /// derivation context,
    ///         i.e. independent from configuration by the Init() call. size of the key ID in bytes the Init() call.
    /// @uptrace{SWS_CRYPT_21518}
    virtual std::size_t GetKeyIdSize() const noexcept = 0;

    /// @brief Get the symmetric algorithm ID of target (slave) key.
    ///        If the context was not configured yet by a call of the Init() method then kAlgIdUndefined should be.
    /// @return the symmetric algorithm ID of the target key, configured by the last call of the Init() method returned.
    /// @uptrace{SWS_CRYPT_21520}
    virtual AlgId GetTargetAlgId() const noexcept = 0;

    /// @brief Get allowed key usage of target (slave) key.
    ///        The returned value depends on the source key-material allowed usage flags and the argument allowedUsage
    ///        of last call of the Init() method. If the context has not yet been configured by a call of the Init()
    ///        method, the allowed usage flags of the source key-material shall be returned. If the context has not yet
    ///        been configured by a call of the Init() method and no source key-material has been set either,
    ///        kAllowKdfMaterialAnyUsage shall be returned.
    /// @return allowed key usage bit-flags of target keys
    /// @uptrace{SWS_CRYPT_21521}
    virtual AllowedUsageFlags GetTargetAllowedUsage() const noexcept = 0;

    /// @brief Get the bit-length of target (diversified) keys. Returned value is configured by the context factory
    /// method, i.e. independent from configuration by.
    /// @return the length of target (diversified) key in bits the Init() calls
    /// @uptrace{SWS_CRYPT_21522}
    virtual std::size_t GetTargetKeyBitLength() const noexcept = 0;

    /// @brief Initialize this context by setting at least the target key ID.
    ///        The byte sequence provided via argument ctxLabel can include a few fields with different meaning
    ///        separated by single 0x00 byte. If (targetAlgId == kAlgIdAny) then a diversified key can be loaded to any
    ///        symmetric context that supports the same key length (if the "allowed usage" flags are also satisfied)!
    /// @param targetKeyId ID of the target key
    /// @param targetAlgId the identifier of the target symmetric crypto algorithm
    /// @param allowedUsage bit-flags that define a list of allowed transformations’ types in which the target key may
    /// be used
    /// @param ctxLabel an optional application specific "context label" (this can identify the purpose of the target
    /// key and/or communication parties)
    /// @return --
    /// @uptrace{SWS_CRYPT_21523}
    virtual ara::core::Result<void> Init(ReadOnlyMemRegion targetKeyId, AlgId targetAlgId = kAlgIdAny,
                                         AllowedUsageFlags allowedUsage = kAllowKdfMaterialAnyUsage,
                                         ReadOnlyMemRegion ctxLabel = ReadOnlyMemRegion()) noexcept = 0;

    /// @brief Set (deploy) key-material to the key derivation algorithm context.
    /// @param sourceKM the source key-material
    /// @return --
    /// @uptrace{SWS_CRYPT_21525}
    virtual ara::core::Result<void> SetSourceKeyMaterial(const RestrictedUseObject& sourceKM) noexcept = 0;
};

}  // namespace cryp
}  // namespace crypto
}  // namespace ara

#endif  // ARA_CRYPTO_CRYP_KEY_DERIVATION_FUNCTION_CTX_H
