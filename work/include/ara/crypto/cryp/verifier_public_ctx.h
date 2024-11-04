// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef ARA_CRYPTO_CRYP_VERIFIER_PUBLIC_CTX_H
#define ARA_CRYPTO_CRYP_VERIFIER_PUBLIC_CTX_H

#include <memory>

#include "ara/core/result.h"
#include "ara/crypto/common/base_id_types.h"
#include "ara/crypto/common/mem_region.h"
#include "ara/crypto/cryp/cryobj/public_key.h"
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

/// @brief Signature Verification Public key Context interface.
/// @uptrace{SWS_CRYPT_24100}
class VerifierPublicCtx : public CryptoContext
{
  public:
    /// @brief Unique smart pointer of the interface.
    /// @uptrace{SWS_CRYPT_24101}
    using Uptr = std::unique_ptr<VerifierPublicCtx>;

    /// @brief Extension service member class.
    /// @return --
    /// @uptrace{SWS_CRYPT_24102}
    virtual SignatureService::Uptr GetSignatureService() const noexcept = 0;

    /// @brief Clear the crypto context.
    /// @return --
    /// @uptrace{SWS_CRYPT_24116}
    virtual ara::core::Result<void> Reset() noexcept = 0;

    /// @brief Set (deploy) a key to the verifier public algorithm context.
    /// @param key the source key object
    /// @return --
    /// @uptrace{SWS_CRYPT_24115}
    virtual ara::core::Result<void> SetKey(const PublicKey& key) noexcept = 0;

    /// @brief Verify signature by a digest value stored in the hash-function context.
    ///        This is a pass-through interface to SWS_CRYPT_24113 for developer convenience,
    ///        i.e. it adds additional input checks amd then calls the verify() interface from SWS_CRYPT_24113.
    /// @param hashFn hash function to be used for hashing
    /// @param signature the signature object for verification
    /// @param context an optional user supplied "context" (its support depends from concrete algorithm)
    /// @return true if the signature was verified successfully and false otherwise
    /// @uptrace{SWS_CRYPT_24111}
    ara::core::Result<bool> VerifyPrehashed(const HashFunctionCtx& hashFn, const Signature& signature,
                                            ReadOnlyMemRegion context = ReadOnlyMemRegion()) const noexcept;

    /// @brief Verify signature BLOB by a directly provided hash or message value.
    ///        This method can be used for implementation of the "multiple passes" signature algorithms that process a
    ///        message directly, i.e. without "pre-hashing" (like Ed25519ctx). But also this method is suitable for
    ///        implementation of the traditional signature schemes with pre-hashing (like Ed25519ph, Ed448ph, ECDSA). If
    ///        the target algorithm doesn’t support the context argument then the empty (default) value must be
    ///        supplied! The user supplied context may be used for such algorithms as: Ed25519ctx, Ed25519ph, Ed448ph.
    /// @param value the (pre-)hashed or direct message value that should be verified
    /// @param signature the signature BLOB for the verification
    ///        (the BLOB contains a plain sequence of the digital signature components located in fixed/maximum length
    ///        fields defined by the algorithm specification, and each component is presented by a raw bytes sequence
    ///        padded by zeroes to full length of the field; e.g. in case of (EC)DSA-256 (i.e. length of the q module is
    ///        256 bits) the signature BLOB must have two fixed-size fields: 32 + 32 bytes, for R and S components
    ///        respectively, i.e. total BLOB size is 64 bytes)
    /// @param context an optional user supplied "context" (its support depends from concrete algorithm)
    /// @return true if the signature was verified successfully and false otherwise
    /// @uptrace{SWS_CRYPT_24112}
    virtual ara::core::Result<bool> Verify(ReadOnlyMemRegion value, ReadOnlyMemRegion signature,
                                           ReadOnlyMemRegion context = ReadOnlyMemRegion()) const noexcept = 0;

    /// @brief Verify signature by a digest value stored in the hash-function context.
    ///        This is a pass-through interface to SWS_CRYPT_24112 for developer convenience,
    ///        i.e. it adds additional input checks amd then calls the default verify() interface.
    /// @param hashAlgId hash function algorithm ID
    /// @param hashValue hash function value (resulting digest without any truncations)
    /// @param signature the signature object for the verification
    /// @param context an optional user supplied "context" (its support depends from concrete algorithm)
    /// @return true if the signature was verified successfully and false otherwise
    /// @uptrace{SWS_CRYPT_24113}
    ara::core::Result<bool> VerifyPrehashed(CryptoAlgId hashAlgId, ReadOnlyMemRegion hashValue,
                                            const Signature& signature,
                                            ReadOnlyMemRegion context = ReadOnlyMemRegion()) const noexcept;

    /// @brief Verify signature by a digest value stored in the hash-function context.
    ///        This is a pass-through interface to SWS_CRYPT_24112 for developer convenience,
    ///        i.e. it adds additional input checks amd then calls the default verify() interface.
    /// @param hashFn hash function to be used for hashing
    /// @param signature the data BLOB to be verified
    /// @param context an optional user supplied "context" (its support depends from concrete algorithm)
    /// @return true if the signature was verified successfully and false otherwise
    /// @uptrace{SWS_CRYPT_24114}
    virtual ara::core::Result<bool> VerifyPrehashed(const HashFunctionCtx& hashFn, ReadOnlyMemRegion signature,
                                                    ReadOnlyMemRegion context = ReadOnlyMemRegion()) const noexcept;
};

}  // namespace cryp
}  // namespace crypto
}  // namespace ara

#endif  // ARA_CRYPTO_CRYP_VERIFIER_PUBLIC_CTX_H