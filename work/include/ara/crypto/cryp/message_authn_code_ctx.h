// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef ARA_CRYPTO_CRYP_MESSAGE_AUTHN_CODE_CTX_H
#define ARA_CRYPTO_CRYP_MESSAGE_AUTHN_CODE_CTX_H

#include <memory>

#include "ara/core/result.h"
#include "ara/core/utility.h"
#include "ara/core/vector.h"
#include "ara/crypto/common/base_id_types.h"
#include "ara/crypto/common/mem_region.h"
#include "ara/crypto/cryp/cryobj/restricted_use_object.h"
#include "ara/crypto/cryp/cryobj/secret_seed.h"
#include "ara/crypto/cryp/cryobj/signature.h"
#include "ara/crypto/cryp/cryobj/symmetric_key.h"
#include "ara/crypto/cryp/crypto_context.h"
#include "ara/crypto/cryp/digest_service.h"

namespace ara
{
namespace crypto
{
namespace cryp
{

/// @brief Keyed Message Authentication Code Context interface definition (MAC/HMAC).
/// @uptrace{SWS_CRYPT_22100}
class MessageAuthnCodeCtx : public CryptoContext
{
  public:
    /// @brief Unique smart pointer of the interface.
    /// @uptrace{SWS_CRYPT_22101}
    using Uptr = std::unique_ptr<MessageAuthnCodeCtx>;

    /// @brief Check the calculated digest against an expected "signature" object.
    ///        Entire digest value is kept in the context up to next call Start(), therefore it can be verified again or
    ///        extracted. This method can be implemented as "inline" after standartization of function
    ///        ara::core::memcmp().
    /// @param expected the signature object containing an expected digest value
    /// @return true if value and meta-information of the provided "signature" object is identical to calculated digest
    ///         and current configuration of the context respectively; but false otherwise
    /// @uptrace{SWS_CRYPT_22119}
    virtual ara::core::Result<bool> Check(const Signature& expected) const noexcept = 0;

    /// @brief Finish the digest calculation and optionally produce the "signature" object.
    ///        Only after call of this method the digest can be signed, verified, extracted or compared!
    ///        If the signature object produced by a keyed MAC/HMAC/AE/AEAD algorithm
    ///        then the dependence COUID of the "signature" should be set to COUID of used symmetric key.
    /// @param makeSignatureObject
    /// @return unique smart pointer to created signature object,
    ///         if (makeSignatureObject == true) or nullptr if (makeSignatureObject == false)
    /// @uptrace{SWS_CRYPT_22115}
    virtual ara::core::Result<Signature::Uptrc> Finish(bool makeSignatureObject = false) noexcept = 0;

    /// @brief Get DigestService instance.
    /// @return --
    /// @uptrace{SWS_CRYPT_22102}
    virtual DigestService::Uptr GetDigestService() const noexcept = 0;

    /// @brief Get requested part of calculated digest to existing memory buffer.
    ///        Entire digest value is kept in the context up to next call Start(), therefore any its part can be
    ///        extracted again or verified. If (full_digest_size <= offset) then return_size = 0 bytes; else return_size
    ///        = min(output.size(), (full_digest_size - offset)) bytes. This method can be implemented as "inline" after
    ///        standartization of function ara::core::memcpy().
    /// @param offset position of the first byte of digest that should be placed to the output buffer
    /// @return number of digest bytes really stored to the output buffer (they are always <= output.size() and denoted
    /// below as return_size)
    /// @uptrace{SWS_CRYPT_22116}
    virtual ara::core::Result<ara::core::Vector<ara::core::Byte>> GetDigest(std::size_t offset = 0) const noexcept = 0;

    /// @brief Get requested part of calculated digest to pre-reserved managed container.
    ///        This method sets the size of the output container according to actually saved value.
    ///        Entire digest value is kept in the context up to next call Start(), therefore any its part can be
    ///        extracted again or verified. If (full_digest_size <= offset) then return_size = 0 bytes; else return_size
    ///        = min(output.capacity(), (full_digest_size - offset)) bytes.
    /// @param offset position of first byte of digest that should be placed to the output buffer
    /// @return --
    /// @uptrace{SWS_CRYPT_22117}
    template <typename Alloc = <implementation - defined>>
    ara::core::Result<ByteVector<Alloc>> GetDigest(std::size_t offset = 0) const noexcept;

    /// @brief Clear the crypto context.
    /// @return --
    /// @uptrace{SWS_CRYPT_22120}
    virtual ara::core::Result<void> Reset() noexcept = 0;

    /// @brief Set (deploy) a key to the message authn code algorithm context.
    /// @param key the source key object
    /// @param transform --
    /// @return --
    /// @uptrace{SWS_CRYPT_22118}
    virtual ara::core::Result<void> SetKey(const SymmetricKey& key,
                                           CryptoTransform transform = CryptoTransform::kMacGenerate) noexcept = 0;

    /// @brief Initialize the context for a new data stream processing or generation (depending from the primitive).
    ///        If IV size is greater than maximally supported by the algorithm then an implementation may use the
    ///        leading bytes only from the sequence.
    /// @param iv an optional Initialization Vector (IV) or "nonce" value
    /// @return --
    /// @uptrace{SWS_CRYPT_22110}
    virtual ara::core::Result<void> Start(ReadOnlyMemRegion iv = ReadOnlyMemRegion()) noexcept = 0;

    /// @brief Initialize the context for a new data stream processing or generation (depending from the primitive).
    ///        If IV size is greater than maximally supported by the algorithm then an implementation may use the
    ///        leading bytes only from the sequence.
    /// @param iv the Initialization Vector (IV) or "nonce" object
    /// @return --
    /// @uptrace{SWS_CRYPT_22111}
    virtual ara::core::Result<void> Start(const SecretSeed& iv) noexcept = 0;

    /// @brief Update the digest calculation context by a new part of the message.
    ///        This method is dedicated for cases then the RestrictedUseObject is a part of the "message".
    /// @param in a part of input message that should be processed
    /// @return --
    /// @uptrace{SWS_CRYPT_22112}
    virtual ara::core::Result<void> Update(const RestrictedUseObject& in) noexcept = 0;

    /// @brief Update the digest calculation context by a new part of the message.
    /// @param in a part of the input message that should be processed
    /// @return --
    /// @uptrace{SWS_CRYPT_22113}
    virtual ara::core::Result<void> Update(ReadOnlyMemRegion in) noexcept = 0;

    /// @brief Update the digest calculation context by a new part of the message. This method is convenient for
    /// processing of constant tags.
    /// @param in a byte value that is a part of input message
    /// @return --
    /// @uptrace{SWS_CRYPT_22114}
    virtual ara::core::Result<void> Update(std::uint8_t in) noexcept = 0;
};

}  // namespace cryp
}  // namespace crypto
}  // namespace ara

#endif  // ARA_CRYPTO_CRYP_MESSAGE_AUTHN_CODE_CTX_H