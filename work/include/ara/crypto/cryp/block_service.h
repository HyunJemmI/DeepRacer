// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef ARA_CRYPTO_CRYP_BLOCK_SERVICE_H
#define ARA_CRYPTO_CRYP_BLOCK_SERVICE_H

#include "ara/core/optional.h"
#include "ara/crypto/cryp/extension_service.h"

namespace ara
{
namespace crypto
{
namespace cryp
{

/// @brief Extension meta-information service for block cipher contexts.
/// @uptrace{SWS_CRYPT_29030}
class BlockService : public ExtensionService
{
  public:
    /// @brief Unique smart pointer of the interface.
    /// @uptrace{SWS_CRYPT_29031}
    using Uptr = std::unique_ptr<BlockService>;

    /// @brief Get actual bit-length of an IV loaded to the context.
    /// @param ivUid optional pointer to a buffer for saving an COUID of a IV object now loaded to the context.
    ///        If the context was initialized by a SecretSeed object then the output buffer *ivUid must be filled by
    ///        COUID of this loaded IV object, in other cases *ivUid must be filled by all zeros.
    /// @return actual length of the IV (now set to the algorithm context) in bits
    /// @uptrace{SWS_CRYPT_29035}
    virtual std::size_t GetActualIvBitLength(ara::core::Optional<CryptoObjectUid> ivUid) const noexcept = 0;

    /// @brief Get block (or internal buffer) size of the base algorithm.
    /// @return size of the block in bytes
    /// @uptrace{SWS_CRYPT_29033}
    virtual std::size_t GetBlockSize() const noexcept = 0;

    /// @brief Get default expected size of the Initialization Vector (IV) or nonce.
    /// @return default expected size of IV in bytes
    /// @uptrace{SWS_CRYPT_29032}
    virtual std::size_t GetIvSize() const noexcept = 0;

    /// @brief Verify validity of specific Initialization Vector (IV) length.
    /// @param ivSize the length of the IV in bytes
    /// @return true if provided IV length is supported by the algorithm and false otherwise
    /// @uptrace{SWS_CRYPT_29034}
    virtual bool IsValidIvSize(std::size_t ivSize) const noexcept = 0;
};

}  // namespace cryp
}  // namespace crypto
}  // namespace ara

#endif  // ARA_CRYPTO_CRYP_BLOCK_SERVICE_H