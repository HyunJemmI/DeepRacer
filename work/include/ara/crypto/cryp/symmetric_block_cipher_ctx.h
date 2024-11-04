// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef ARA_CRYPTO_CRYP_SYMMETRIC_BLOCK_CIPHER_CTX_H
#define ARA_CRYPTO_CRYP_SYMMETRIC_BLOCK_CIPHER_CTX_H

#include "ara/core/result.h"
#include "ara/core/utility.h"
#include "ara/core/vector.h"
#include "ara/crypto/common/base_id_types.h"
#include "ara/crypto/common/mem_region.h"
#include "ara/crypto/cryp/cryobj/symmetric_key.h"
#include "ara/crypto/cryp/crypto_context.h"
#include "ara/crypto/cryp/crypto_service.h"

namespace ara
{
namespace crypto
{
namespace cryp
{

/// @brief Interface of a Symmetric Block Cipher Context with padding.
/// @uptrace{SWS_CRYPT_23700}
class SymmetricBlockCipherCtx : public CryptoContext
{
  public:
    /// @brief Unique smart pointer of the interface.
    /// @uptrace{SWS_CRYPT_23701}
    using Uptr = std::unique_ptr<SymmetricBlockCipherCtx>;

    /// @brief Get CryptoService instance.
    /// @return --
    /// @uptrace{SWS_CRYPT_23702}
    virtual CryptoService::Uptr GetCryptoService() const noexcept = 0;

    /// @brief Get the kind of transformation configured for this context: kEncrypt or kDecrypt.
    /// @return CryptoTransform
    /// @uptrace{SWS_CRYPT_23711}
    virtual ara::core::Result<CryptoTransform> GetTransformation() const noexcept = 0;

    /// @brief Indicate that the currently configured transformation accepts only complete blocks of input data.
    /// @return true if the transformation requires the maximum size of input data and false otherwise
    /// @uptrace{SWS_CRYPT_23712}
    ara::core::Result<bool> IsMaxInputOnly() const noexcept;

    /// @brief Indicate that the currently configured transformation can produce only complete blocks of output data.
    /// @return true if the transformation can produce only the maximum size of output data and false otherwise
    /// @uptrace{SWS_CRYPT_23713}
    ara::core::Result<bool> IsMaxOutputOnly() const noexcept;

    /// @brief Process (encrypt / decrypt) an input block according to the cryptor configuration.
    /// @param in the input data block
    /// @param suppressPadding if true then the method doesn’t apply the padding, but the payload should fill the whole
    /// block of the plain data
    /// @return the output buffer Encryption with (suppressPadding == true) expects that: in.size() ==
    /// GetMaxInputSize(true) && out.size() >= GetMaxOutputSize(true).
    ///         Encryption with (suppressPadding == false) expects that: in.size() <= GetMaxInputSize(false) &&
    ///         in.size() > 0 && out.size() >= GetMaxOutputSize(false). Decryption expects that: in.size() ==
    ///         GetMaxInputSize() && out.size() >= GetMaxOutputSize(suppressPadding). The case (out.size() <
    ///         GetMaxOutputSize()) should be used with caution, only if you are strictly certain about the size of the
    ///         output data! In case of (suppressPadding == true) the actual size of plain text should be equal to full
    ///         size of the plain data block (defined by the algorithm)! actual size of output data (it always <=
    ///         out.size()) or 0 if the input data block has incorrect content
    /// @uptrace{SWS_CRYPT_23716}
    virtual ara::core::Result<ara::core::Vector<ara::core::Byte>> ProcessBlock(
        ReadOnlyMemRegion in, bool suppressPadding = false) const noexcept = 0;

    /// @brief Process (encrypt / decrypt) an input block according to the cryptor configuration.
    ///        This method sets the size of the output container according to actually saved value! Encryption with
    ///        (suppressPadding == true) expects what: in.size() == GetMaxInputSize(true) && out.capacity() >=
    ///        GetMaxOutputSize(true). Encryption with (suppressPadding == false) expects what: in.size() <=
    ///        GetMaxInputSize(false) && in.size() > 0 && out.capacity() >= GetMaxOutputSize(false). Decryption expects
    ///        what: in.size() == GetMaxInputSize() && out.capacity() >= GetMaxOutputSize(suppressPadding). The case
    ///        (out.capacity() < GetMaxOutputSize()) should be used with caution, only if you are strictly certain about
    ///        the size of the output data! In case of (suppressPadding == true) the actual size of plain text should be
    ///        equal to full size of the plain data block (defined by the algorithm)!
    /// @param in the input data block
    /// @param suppressPadding if true then the method doesn’t apply the padding, but the payload should fill the whole
    /// block of the plain data
    /// @return the managed container for output block
    /// @uptrace{SWS_CRYPT_23717}
    template <typename Alloc = <implementation - defined>>
    ara::core::Result<ByteVector<Alloc>> ProcessBlock(ReadOnlyMemRegion in,
                                                      bool suppressPadding = false) const noexcept;

    /// @brief Processe provided blocks without padding.
    ///        The in and out buffers must have same size and this size must be divisible by the block size (see
    ///        GetBlockSize()). Pointers to the input and output buffers must be aligned to the block-size boundary!
    /// @param in an input data buffer
    /// @return an output data buffer
    /// @uptrace{SWS_CRYPT_23715}
    virtual ara::core::Result<ara::core::Vector<ara::core::Byte>> ProcessBlocks(
        ReadOnlyMemRegion in) const noexcept = 0;

    /// @brief Clear the crypto context.
    /// @return --
    /// @uptrace{SWS_CRYPT_23714}
    virtual ara::core::Result<void> Reset() noexcept = 0;

    /// @brief Set (deploy) a key to the symmetric algorithm context.
    /// @param key the source key object
    /// @param transform --
    /// @return --
    /// @uptrace{SWS_CRYPT_23710}
    virtual ara::core::Result<void> SetKey(const SymmetricKey& key,
                                           CryptoTransform transform = CryptoTransform::kEncrypt) noexcept = 0;
};

}  // namespace cryp
}  // namespace crypto
}  // namespace ara

#endif  // ARA_CRYPTO_CRYP_SYMMETRIC_BLOCK_CIPHER_CTX_H
