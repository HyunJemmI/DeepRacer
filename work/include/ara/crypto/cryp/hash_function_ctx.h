// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef ARA_CRYPTO_CRYP_HASH_FUNCTION_CTX_H
#define ARA_CRYPTO_CRYP_HASH_FUNCTION_CTX_H

#include <memory>

#include "ara/core/result.h"
#include "ara/core/utility.h"
#include "ara/core/vector.h"
#include "ara/crypto/common/base_id_types.h"
#include "ara/crypto/common/mem_region.h"
#include "ara/crypto/cryp/crypto_context.h"
#include "ara/crypto/cryp/digest_service.h"

namespace ara
{
namespace crypto
{
namespace cryp
{

/// @brief Hash function interface.
/// @uptrace{SWS_CRYPT_21100}
class HashFunctionCtx : public CryptoContext
{
  public:
    /// @brief Unique smart pointer of the interface.
    /// @uptrace{SWS_CRYPT_21101}
    using Uptr = std::unique_ptr<HashFunctionCtx>;

    /// @brief Finish the digest calculation and optionally produce the "signature" object.
    ///        Only after call of this method the digest can be signed, verified, extracted or compared.
    /// @return unique smart pointer to created signature object, if (makeSignatureObject == true) or an empty Signature
    /// object if (makeSignatureObject == false)
    /// @uptrace{SWS_CRYPT_21115}
    virtual ara::core::Result<ara::core::Vector<ara::core::Byte>> Finish() noexcept = 0;

    /// @brief Get DigestService instance.
    /// @return --
    /// @uptrace{SWS_CRYPT_21102}
    virtual DigestService::Uptr GetDigestService() const noexcept = 0;

    /// @brief Get requested part of calculated digest. Entire digest value is kept in the context up to next call
    /// Start(),
    ///        therefore any its part can be extracted again or verified.
    ///        If (full_digest_size <= offset) then return_size = 0 bytes; else return_size = min(output.size(),
    ///        (full_digest_size - offset)) bytes. This method can be implemented as "inline" after standartization of
    ///        function ara::core::memcpy().
    /// @param offset
    /// @return number of digest bytes really stored to the output buffer (they are always <= output.size() and denoted
    /// below as return_size)
    /// @uptrace{SWS_CRYPT_21116}
    virtual ara::core::Result<ara::core::Vector<ara::core::Byte>> GetDigest(std::size_t offset = 0) const noexcept = 0;

    /// @brief Get requested part of calculated digest to pre-reserved managed container.
    ///        This method sets the size of the output container according to actually saved value.
    ///        Entire digest value is kept in the context up to next call Start(), therefore any its part can be
    ///        extracted again or verified. If (full_digest_size <= offset) then return_size = 0 bytes; else return_size
    ///        = min(output.capacity(), (full_digest_size - offset)) bytes.
    /// @param offset position of first byte of digest that should be placed to the output buffer
    /// @return --
    /// @uptrace{SWS_CRYPT_21117}
    template <typename Alloc = <implementation - defined>>
    ara::core::Result<ByteVector<Alloc>> GetDigest(std::size_t offset = 0) const noexcept;

    /// @brief Initialize the context for a new data stream processing or generation (depending on the primitive)
    /// without IV.
    /// @return --
    /// @uptrace{SWS_CRYPT_21118}
    virtual ara::core::Result<void> Start() noexcept = 0;

    /// @brief Initialize the context for a new data stream processing or generation (depending on the primitive).
    ///        If IV size is greater than maximally supported by the algorithm then an implementation may use the
    ///        leading bytes only from the sequence.
    /// @param iv an optional Initialization Vector (IV) or "nonce" value
    /// @return --
    /// @uptrace{SWS_CRYPT_21110}
    virtual ara::core::Result<void> Start(ReadOnlyMemRegion iv) noexcept = 0;

    /// @brief Initialize the context for a new data stream processing or generation (depending on the primitive).
    ///        If IV size is greater than maximally supported by the algorithm then an implementation may use the
    ///        leading bytes only from the sequence.
    /// @param iv the Initialization Vector (IV) or "nonce" object
    /// @return --
    /// @uptrace{SWS_CRYPT_21111}
    virtual ara::core::Result<void> Start(const SecretSeed& iv) noexcept = 0;

    /// @brief Update the digest calculation context by a new part of the message.
    ///        This method is dedicated for cases then the RestrictedUseObject is a part of the "message".
    /// @param in a part of input message that should be processed
    /// @return --
    /// @uptrace{SWS_CRYPT_21112}
    virtual ara::core::Result<void> Update(const RestrictedUseObject& in) noexcept = 0;

    /// @brief Update the digest calculation context by a new part of the message.
    /// @param in a part of the input message that should be processed
    /// @return --
    /// @uptrace{SWS_CRYPT_21113}
    virtual ara::core::Result<void> Update(ReadOnlyMemRegion in) noexcept = 0;

    /// @brief Update the digest calculation context by a new part of the message. This method is convenient for
    /// processing of constant tags.
    /// @param in a byte value that is a part of input message
    /// @return --
    /// @uptrace{SWS_CRYPT_21114}
    virtual ara::core::Result<void> Update(std::uint8_t in) noexcept = 0;
};

}  // namespace cryp
}  // namespace crypto
}  // namespace ara

#endif  // ARA_CRYPTO_CRYP_HASH_FUNCTION_CTX_H