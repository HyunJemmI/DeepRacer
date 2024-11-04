// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef ARA_CRYPTO_CRYP_DECRYPTOR_PRIVATE_CTX_H
#define ARA_CRYPTO_CRYP_DECRYPTOR_PRIVATE_CTX_H

#include <memory>

#include "ara/core/result.h"
#include "ara/core/utility.h"
#include "ara/core/vector.h"
#include "ara/crypto/common/base_id_types.h"
#include "ara/crypto/common/mem_region.h"
#include "ara/crypto/cryp/crypto_context.h"

namespace ara
{
namespace crypto
{
namespace cryp
{

/// @brief Asymmetric Decryption Private key Context interface.
/// @uptrace{SWS_CRYPT_20800}
class DecryptorPrivateCtx : public CryptoContext
{
  public:
    /// @brief Unique smart pointer of the interface.
    /// @uptrace{SWS_CRYPT_20801}
    using Uptr = std::unique_ptr<DecryptorPrivateCtx>;

    /// @brief Get CryptoService instance.
    /// @return --
    /// @uptrace{SWS_CRYPT_20802}
    virtual CryptoService::Uptr GetCryptoService() const noexcept = 0;

    /// @brief Process (encrypt / decrypt) an input block according to the cryptor configuration.
    ///        Encryption with (suppressPadding == true) expects that: in.size() == GetMaxInputSize(true) && out.size()
    ///        >= GetMaxOutputSize(true). Encryption with (suppressPadding == false) expects that: in.size() <=
    ///        GetMaxInputSize(false) && in.size() > 0 && out.size() >= GetMaxOutputSize(false). Decryption expects
    ///        that: in.size() == GetMaxInputSize() && out.size() >= GetMaxOutput Size(suppressPadding). The case
    ///        (out.size() < GetMaxOutputSize()) should be used with caution, only if you are strictly certain about the
    ///        size of the output data! In case of (suppressPadding == true) the actual size of plain text should be
    ///        equal to full size of the plain data block (defined by the algorithm)!
    /// @param in the input data block
    /// @param suppressPadding if true then the method doesn’t apply the padding, but the payload should fill the whole
    /// block of the plain data
    /// @return actual size of output data (it always <= out.size()) or 0 if the input data block has incorrect content
    /// @uptrace{SWS_CRYPT_20812}
    virtual ara::core::Result<ara::core::Vector<ara::core::Byte>> ProcessBlock(
        ReadOnlyMemRegion in, bool suppressPadding = false) const noexcept = 0;

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
    /// @param suppressPadding if true then the method doesn’t apply the padding, but the payload should fill the whole
    /// block of the plain data
    /// @return the managed container for output block
    /// @uptrace{SWS_CRYPT_20813}
    template <typename Alloc = <implementation - defined>>
    ara::core::Result<ByteVector<Alloc>> ProcessBlock(ReadOnlyMemRegion in,
                                                      bool suppressPadding = false) const noexcept;

    /// @brief Clear the crypto context.
    /// @return --
    /// @uptrace{SWS_CRYPT_20811}
    virtual ara::core::Result<void> Reset() noexcept = 0;

    /// @brief Set (deploy) a key to the decryptor private algorithm context.
    /// @param key the source key object
    /// @return --
    /// @uptrace{SWS_CRYPT_20810}
    virtual ara::core::Result<void> SetKey(const PrivateKey& key) noexcept = 0;

    /// @brief Compare the calculated digest against an expected value. Entire digest value is kept in the context up to
    /// next call Start(),
    ///        therefore any its part can be verified again or extracted.
    ///        If (full_digest_size <= offset) || (expected.size() == 0) then return false;
    ///        else comparison_size = min(expected.size(), (full_digest_size - offset)) bytes.
    ///        This method can be implemented as "inline" after standartization of function ara::core::memcmp().
    /// @param expected the memory region containing an expected digest value
    /// @param offset position of the first byte in calculated digest for the comparison starting
    /// @return true if the expected bytes sequence is identical to first bytes of calculated digest
    /// @uptrace{SWS_CRYPT_29013}
    virtual ara::core::Result<bool> Compare(ReadOnlyMemRegion expected, std::size_t offset = 0) const noexcept = 0;
};

}  // namespace cryp
}  // namespace crypto
}  // namespace ara

#endif  // ARA_CRYPTO_CRYP_DECRYPTOR_PRIVATE_CTX_H