// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef ARA_CRYPTO_CRYP_CRYPTO_SERVICE_H
#define ARA_CRYPTO_CRYP_CRYPTO_SERVICE_H

#include <memory>

#include "ara/crypto/cryp/extension_service.h"

namespace ara
{
namespace crypto
{
namespace cryp
{

/// @brief Extension meta-information service for cryptographic contexts.
/// @uptrace{SWS_CRYPT_29020}
class CryptoService : public ExtensionService
{
  public:
    /// @brief Unique smart pointer of the interface.
    /// @uptrace{SWS_CRYPT_29024}
    using Uptr = std::unique_ptr<CryptoService>;

    /// @brief Get block (or internal buffer) size of the base algorithm. For digest, byte-wise stream cipher and
    ///        RNG contexts it is an informative method, intended only for optimization of the interface usage.
    /// @return size of the block in bytes
    /// @uptrace{SWS_CRYPT_29023}
    virtual std::size_t GetBlockSize() const noexcept = 0;

    /// @brief Get maximum expected size of the input data block. suppressPadding argument and it will be equal to the
    /// block size.
    /// @param suppressPadding if true then the method calculates the size for the case when the whole space of the
    /// plain data block is used for the payload only
    /// @return maximum size of the input data block in bytes
    /// @uptrace{SWS_CRYPT_29021}
    virtual std::size_t GetMaxInputSize(bool suppressPadding = false) const noexcept = 0;

    /// @brief Get maximum possible size of the output data block. If (IsEncryption() == true) then a value
    ///        returned by this method is independent from the suppressPadding argument and will be equal to the block
    ///        size.
    /// @param suppressPadding if true then the method calculates the size for the case
    ///        when the whole space of the plain data block is used for the payload only
    /// @return maximum size of the output data block in bytes
    virtual std::size_t GetMaxOutputSize(bool suppressPadding = false) const noexcept = 0;
};

}  // namespace cryp
}  // namespace crypto
}  // namespace ara

#endif  // ARA_CRYPTO_CRYP_CRYPTO_SERVICE_H