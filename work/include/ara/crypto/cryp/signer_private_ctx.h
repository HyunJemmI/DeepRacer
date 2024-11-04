// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef ARA_CRYPTO_CRYP_SIGNER_PRIVATE_CTX_H
#define ARA_CRYPTO_CRYP_SIGNER_PRIVATE_CTX_H

#include <memory>

#include "ara/core/result.h"
#include "ara/crypto/common/mem_region.h"
#include "ara/crypto/cryp/cryobj/private_key.h"
#include "ara/crypto/cryp/cryobj/signature.h"
#include "ara/crypto/cryp/crypto_context.h"
#include "ara/crypto/cryp/hash_function_ctx.h"
#include "ara/crypto/cryp/signature_service.h"

namespace ara
{
namespace crypto
{
namespace cryp
{

/// @brief Signature Private key Context interface.
/// @uptrace{SWS_CRYPT_23500}
class SignerPrivateCtx : public CryptoContext
{
  public:
    /// @brief Unique smart pointer of the interface.
    /// @uptrace{SWS_CRYPT_23501}
    using Uptr = std::unique_ptr<SignerPrivateCtx>;

    /// @brief Get SignatureService instance.
    /// @return --
    /// @uptrace{SWS_CRYPT_23510}
    virtual SignatureService::Uptr GetSignatureService() const noexcept = 0;

    /// @brief Clear the crypto context.
    /// @return --
    /// @uptrace{SWS_CRYPT_23516}
    virtual ara::core::Result<void> Reset() noexcept = 0;

    /// @brief Set (deploy) a key to the signer private algorithm context.
    /// @param key the source key object
    /// @return --
    /// @uptrace{SWS_CRYPT_23515}
    virtual ara::core::Result<void> SetKey(const PrivateKey& key) noexcept = 0;

    /// @brief Sign a provided digest value stored in the hash-function context.
    ///        This method must put the hash-function algorithm ID and a COUID of the used key-pair to the resulting
    ///        signature object! The user supplied context may be used for such algorithms as: Ed25519ctx, Ed25519ph,
    ///        Ed448ph. If the target algorithm doesn’t support the context argument then the empty (default) value must
    ///        be supplied!
    /// @param hashFn a finalized hash-function context that contains a digest value ready for sign
    /// @param context an optional user supplied "context" (its support depends from concrete algorithm)
    /// @return unique smart pointer to serialized signature
    /// @uptrace{SWS_CRYPT_23511}
    virtual ara::core::Result<Signature::Uptrc> SignPreHashed(
        const HashFunctionCtx& hashFn, ReadOnlyMemRegion context = ReadOnlyMemRegion()) const noexcept = 0;

    /// @brief Sign a directly provided hash or message value.
    ///        This method can be used for implementation of the "multiple passes" signature algorithms that process a
    ///        message directly, i.e. without "pre-hashing" (like Ed25519ctx). But also this method is suitable for
    ///        implementation of the traditional signature schemes with pre-hashing (like Ed25519ph, Ed448ph, ECDSA). If
    ///        the target algorithm doesn’t support the context argument then the empty (default) value must be
    ///        supplied!
    /// @param value the (pre-)hashed or direct message value that should be signed
    /// @param context an optional user supplied "context" (its support depends from concrete algorithm)
    /// @return actual size of the signature value stored to the output buffer
    /// @uptrace{SWS_CRYPT_23512}
    virtual ara::core::Result<ara::core::Vector<ara::core::Byte>> Sign(
        ReadOnlyMemRegion value, ReadOnlyMemRegion context = ReadOnlyMemRegion()) const noexcept = 0;

    /// @brief Sign a directly provided digest value and create the Signature object.
    ///        This method must put the hash-function algorithm ID and a COUID of the used key-pair to the resulting
    ///        signature object! The user supplied context may be used for such algorithms as: Ed25519ctx, Ed25519ph,
    ///        Ed448ph. If the target algorithm doesn’t support the context argument then the empty (default) value must
    ///        be supplied!
    /// @param hashAlgId hash function algorithm ID
    /// @param hashValue hash function value (resulting digest without any truncations)
    /// @param context an optional user supplied "context" (its support depends from concrete algorithm)
    /// @return unique smart pointer to serialized signature
    /// @uptrace{SWS_CRYPT_23513}
    virtual ara::core::Result<Signature::Uptrc> SignPreHashed(
        AlgId hashAlgId, ReadOnlyMemRegion hashValue,
        ReadOnlyMemRegion context = ReadOnlyMemRegion()) const noexcept = 0;

    /// @brief Sign a directly provided hash or message value.
    ///        This method can be used for implementation of the "multiple passes" signature algorithms that process a
    ///        message directly, i.e. without "pre-hashing" (like Ed25519ctx). But also this method is suitable for
    ///        implementation of the traditional signature schemes with pre-hashing (like Ed25519ph, Ed448ph, ECDSA).
    ///        This method sets the size of the output container according to actually saved value!
    ///        If the target algorithm doesn’t support the context argument then the empty (default) value must be
    ///        supplied!
    /// @param value the (pre-)hashed or direct message value that should be signed
    /// @param context an optional user supplied "context" (its support depends from concrete algorithm)
    /// @return --
    /// @uptrace{SWS_CRYPT_23514}
    template <typename Alloc = <implementation - defined>>
    ara::core::Result<ByteVector<Alloc>> Sign(ReadOnlyMemRegion value,
                                              ReadOnlyMemRegion context = ReadOnlyMemRegion()) const noexcept;
};

}  // namespace cryp
}  // namespace crypto
}  // namespace ara

#endif  // ARA_CRYPTO_CRYP_SIGNER_PRIVATE_CTX_H