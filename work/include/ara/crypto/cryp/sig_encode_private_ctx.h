// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef ARA_CRYPTO_CRYP_SIG_ENCODE_PRIVATE_CTX_H
#define ARA_CRYPTO_CRYP_SIG_ENCODE_PRIVATE_CTX_H

#include <memory>

#include "ara/core/result.h"
#include "ara/core/utility.h"
#include "ara/core/vector.h"
#include "ara/crypto/common/base_id_types.h"
#include "ara/crypto/common/mem_region.h"
#include "ara/crypto/cryp/cryobj/private_key.h"
#include "ara/crypto/cryp/crypto_context.h"
#include "ara/crypto/cryp/extension_service.h"

namespace ara
{
namespace crypto
{
namespace cryp
{

/// @brief A private key context for asymmetric signature calculation and short message encoding(RSA-like).
///        Restricted groups of trusted subscribers can use this primitive for simultaneous
///        provisioning of confidentiality, authenticity and non-repudiation of short messages,
///        if the public key is generated appropriately and kept in secret.
/// @uptrace{SWS_CRYPT_23200}
class SigEncodePrivateCtx : public CryptoContext
{
  public:
    /// @brief Unique smart pointer of the interface.
    /// @uptrace{SWS_CRYPT_23201}
    using Uptr = std::unique_ptr<SigEncodePrivateCtx>;

    /// @brief Extension service member class.
    /// @return --
    /// @uptrace{SWS_CRYPT_23210}
    virtual ExtensionService::Uptr GetExtensionService() const noexcept = 0;

    /// @brief Get maximum expected size of the input data block.
    ///        If (IsEncryption() == false) then a value returned by this method is independent from the suppressPadding
    ///        argument and it will be equal to the block size.
    /// @param suppressPadding if true then the method calculates the size for the case when the whole space of the
    /// plain data block is used for the payload only
    /// @return maximum size of the input data block in bytes
    /// @uptrace{SWS_CRYPT_23213}
    virtual std::size_t GetMaxInputSize(bool suppressPadding = false) const noexcept = 0;

    /// @brief Get maximum possible size of the output data block.
    ///        If (IsEncryption() == true) then a value returned by this method is independent from the suppressPadding
    ///        argument and will be equal to the block size.
    /// @param suppressPadding if true then the method calculates the size for the case when the whole space of the
    /// plain data block is used for the payload only
    /// @return maximum size of the output data block in bytes
    /// @uptrace{SWS_CRYPT_23214}
    virtual std::size_t GetMaxOutputSize(bool suppressPadding = false) const noexcept = 0;

    /// @brief Process (encrypt / decrypt) an input block according to the cryptor configuration.
    ///        Encryption with (suppressPadding == true) expects that: in.size() == GetMaxInputSize(true) && out.size()
    ///        >= GetMaxOutputSize(true). Encryption with (suppressPadding == false) expects that: in.size() <=
    ///        GetMaxInputSize(false) && in.size() > 0 && out.size() >= GetMaxOutputSize(false). Decryption expects
    ///        that: in.size() == GetMaxInputSize() && out.size() >= GetMaxOutputSize(suppressPadding). The case
    ///        (out.size() < GetMaxOutputSize()) should be used with caution, only if you are strictly certain about the
    ///        size of the output data! In case of (suppressPadding == true) the actual size of plain text should be
    ///        equal to full size of the plain data block (defined by the algorithm)!
    /// @param in the input data block
    /// @return the output buffer actual size of output data (it always <= out.size()) or 0 if the input data block has
    /// incorrect content
    /// @uptrace{SWS_CRYPT_23215}
    virtual ara::core::Result<ara::core::Vector<ara::core::Byte>> SignAndEncode(
        ReadOnlyMemRegion in) const noexcept = 0;

    /// @brief Process (encrypt / decrypt) an input block according to the cryptor configuration.
    ///        This method sets the size of the output container according to actually saved value!
    ///        Encryption with (suppressPadding == true) expects what: in.size() == GetMaxInputSize(true) &&
    ///        out.capacity() >= GetMaxOutputSize(true). Encryption with (suppressPadding == false) expects what:
    ///        in.size() <= GetMaxInputSize(false) && in.size() > 0 && out.capacity() >= GetMaxOutputSize(false).
    ///        Decryption expects what: in.size() == GetMaxInputSize() && out.capacity() >=
    ///        GetMaxOutputSize(suppressPadding). The case (out.capacity() < GetMaxOutputSize()) should be used with
    ///        caution, only if you are strictly certain about the size of the output data! In case of (suppressPadding
    ///        == true) the actual size of plain text should be equal to full size of the plain data block (defined by
    ///        the algorithm)!
    /// @param in the input data block
    /// @return the managed container for output block
    /// @uptrace{SWS_CRYPT_23216}
    template <typename Alloc = <implementation - defined>>
    ara::core::Result<ByteVector<Alloc>> SignAndEncode(ReadOnlyMemRegion in) const noexcept;

    /// @brief Clear the crypto context.
    /// @return --
    /// @uptrace{SWS_CRYPT_23212}
    virtual ara::core::Result<void> Reset() noexcept = 0;

    /// @brief Set (deploy) a key to the sig encode private algorithm context.
    /// @param key the source key object
    /// @return --
    /// @uptrace{SWS_CRYPT_23211}
    virtual ara::core::Result<void> SetKey(const PrivateKey& key) noexcept = 0;
};

}  // namespace cryp
}  // namespace crypto
}  // namespace ara

#endif  // ARA_CRYPTO_CRYP_SIG_ENCODE_PRIVATE_CTX_H