// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef ARA_CRYPTO_CYRP_AUTH_CIPHER_CTX_H
#define ARA_CRYPTO_CYRP_AUTH_CIPHER_CTX_H

#include <cstdint>
#include <memory>

#include "ara/core/result.h"
#include "ara/crypto/common/base_id_types.h"
#include "ara/crypto/common/mem_region.h"
#include "ara/crypto/cryp/block_service.h"
#include "ara/crypto/cryp/cryobj/secret_seed.h"
#include "ara/crypto/cryp/cryobj/signature.h"
#include "ara/crypto/cryp/cryobj/symmetric_key.h"
#include "ara/crypto/cryp/crypto_context.h"

namespace ara
{
namespace crypto
{
namespace cryp
{

/// @brief Generalized Authenticated Cipher Context interface.
///        Methods of the derived interface BufferedDigest are used for authentication of associated public data.
///        Methods of the derived interface StreamCipherCtx are used for encryption/decryption and authentication
///        of confidential part of message. The data processing must be executed in following order:
///        Call one of the Start() methods. Process all associated public data via calls of Update() methods.
///        Process the confidential part of the message via calls of ProcessBlocks(), ProcessBytes() (and optionally
///        FinishBytes()) methods. Call the Finish() method due to finalize the authentication code calculation (and get
///        it optionally). Copy of the calculated MAC may be extracted (by GetDigest()) or compared internally (by
///        Compare()). Receiver side should not use decrypted data before finishing of the whole decryption and
///        authentication process! I.e. decrypted data can be used only after successful MAC verification!
/// @uptrace{SWS_CRYPT_20100}
class AuthCipherCtx : public CryptoContext
{
  public:
    /// @brief Unique smart pointer of the interface.
    /// @uptrace{SWS_CRYPT_20101}
    using Uptr = std::unique_ptr<AuthCipherCtx>;

    /// @brief Check the calculated digest against an expected "signature" object.
    ///        Entire digest value is kept in the context up to next call Start(), therefore it can be verified again or
    ///        extracted. This method can be implemented as "inline" after standartization of function
    ///        ara::core::memcmp().
    /// @param expected the signature object containing an expected digest value
    /// @return true if value and meta-information of the provided "signature" object is identical to calculated digest
    ///         and current configuration of the context respectively; but false otherwise
    /// @uptrace{SWS_CRYPT_20319}
    virtual ara::core::Result<bool> Check(const Signature& expected) const noexcept = 0;

    /// @brief Get BlockService instance.
    /// @return BlockService
    /// @uptrace{SWS_CRYPT_20102}
    virtual BlockService::Uptr GetBlockService() const noexcept = 0;

    /// @brief Retrieve the calculated digest. The entire digest value is kept in the context until the next call of
    /// Start().
    ///        Therefore, the digest can be re-checked or extracted at any time. If the offset is larger than the
    ///        digest, an empty buffer shall be returned. This method can be implemented as "inline" after
    ///        standardization of function ara::core::memcpy().
    /// @param offset position of the first byte of digest that should be placed to the output buffer
    /// @return an output buffer storing the requested digest fragment or the full digest
    /// @uptrace{SWS_CRYPT_20316}
    /// @todo Alloc impl
    template <typename Alloc = <implementation - defined>>
    ara::core::Result<ByteVector<Alloc>> GetDigest(std::size_t offset = 0) const noexcept;

    /// @brief Get the kind of transformation configured for this context: kEncrypt or kDecrypt.
    /// @return CryptoTransform
    /// @uptrace{SWS_CRYPT_21715}
    virtual ara::core::Result<CryptoTransform> GetTransformation() const noexcept = 0;

    /// @brief Get maximal supported size of associated public data.
    /// @return maximal supported size of associated public data in bytes
    /// @uptrace{SWS_CRYPT_20103}
    virtual std::uint64_t GetMaxAssociatedDataSize() const noexcept = 0;

    /// @brief Process confidential data The input buffer will be overwritten by the processed message.
    ///        This function is the final call, i.e. all associated data must have been already provided.
    ///        Hence, the function will check the authentication tag and only return the processed data, if the tag is
    ///        valid.
    /// @param in the input buffer containing the full message
    /// @param expectedTag pointer to read only mem region
    /// @return --
    /// @uptrace{SWS_CRYPT_23634}
    virtual ara::core::Result<ara::core::Vector<ara::core::Byte>> ProcessConfidentialData(
        ReadOnlyMemRegion in, ReadOnlyMemRegion expectedTag = nullptr) noexcept = 0;

    /// @brief Process confidential data The input buffer will be overwritten by the processed message After
    ///        this method is called no additional associated data may be updated.
    /// @param inOut the input buffer containing the full message
    /// @param expectedTag pointer to read only mem region
    /// @return --
    /// @uptrace{SWS_CRYPT_23635}
    virtual ara::core::Result<void> ProcessConfidentialData(ReadWriteMemRegion inOut,
                                                            ReadOnlyMemRegion expectedTag = nullptr) noexcept = 0;

    /// @brief Clear the crypto context.
    /// @return --
    /// @uptrace{SWS_CRYPT_20414}
    virtual ara::core::Result<void> Reset() noexcept = 0;

    /// @brief Set (deploy) a key to the authenticated cipher symmetric algorithm context.
    /// @param key the source key object
    /// @param transform --
    /// @return --
    /// @uptrace{SWS_CRYPT_23911}
    virtual ara::core::Result<void> SetKey(const SymmetricKey& key,
                                           CryptoTransform transform = CryptoTransform::kEncrypt) noexcept = 0;

    /// @brief Initialize the context for a new data processing or generation (depending from the primitive).
    ///        If IV size is greater than maximally supported by the algorithm then an implementation may use the
    ///        leading bytes only from the sequence.
    /// @param iv an optional Initialization Vector (IV) or "nonce" value
    /// @return --
    /// @uptrace{SWS_CRYPT_24714}
    virtual ara::core::Result<void> Start(ReadOnlyMemRegion iv = ReadOnlyMemRegion()) noexcept = 0;

    /// @brief Initialize the context for a new data processing or generation (depending from the primitive).
    ///        If IV size is greater than maximally supported by the algorithm then an implementation may use the
    ///        leading bytes only from the sequence.
    /// @param iv the Initialization Vector (IV) or "nonce" object
    /// @return --
    /// @uptrace{SWS_CRYPT_24715}
    virtual ara::core::Result<void> Start(const SecretSeed& iv) noexcept = 0;

    /// @brief Update the digest calculation by the specified RestrictedUseObject.
    ///        This method is dedicated for cases then the RestrictedUseObject is a part of the "message".
    /// @param in a part of input message that should be processed
    /// @return --
    /// @uptrace{SWS_CRYPT_20312}
    virtual ara::core::Result<void> UpdateAssociatedData(const RestrictedUseObject& in) noexcept = 0;

    /// @brief Update the digest calculation by a new chunk of associated data.
    /// @param in a part of the input message that should be processed
    /// @return --
    /// @uptrace{SWS_CRYPT_20313}
    virtual ara::core::Result<void> UpdateAssociatedData(ReadOnlyMemRegion in) noexcept = 0;

    /// @brief Update the digest calculation by the specified Byte.
    ///        This method is convenient for processing of constant tags.
    /// @param in a byte value that is a part of input message
    /// @return --
    /// @uptrace{SWS_CRYPT_20314}
    virtual ara::core::Result<void> UpdateAssociatedData(std::uint8_t in) noexcept = 0;
};

}  // namespace cryp
}  // namespace crypto
}  // namespace ara

#endif  // ARA_CRYPTO_CYRP_AUTH_CIPHER_CTX_H
