// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef ARA_CRYPTO_CRYP_EXTENSION_SERVICE_H
#define ARA_CRYPTO_CRYP_EXTENSION_SERVICE_H

#include <memory>

#include "ara/crypto/common/crypto_object_uid.h"

namespace ara
{
namespace crypto
{
namespace cryp
{

/// @brief Basic meta-information service for all contexts.
/// @uptrace{SWS_CRYPT_29040}
class ExtensionService
{
  public:
    /// @brief Unique smart pointer of the interface.
    /// @uptrace{SWS_CRYPT_29042}
    using Uptr = std::unique_ptr<ExtensionService>;

    /// @brief Destructor.
    /// @uptrace{SWS_CRYPT_29041}
    virtual ~ExtensionService() noexcept = default;

    /// @brief Get actual bit-length of a key loaded to the context. If no key was set to the context yet then 0 is
    /// returned.
    /// @return actual length of a key (now set to the algorithm context) in bits
    /// @uptrace{SWS_CRYPT_29045}
    virtual std::size_t GetActualKeyBitLength() const noexcept = 0;

    /// @brief Get the COUID of the key deployed to the context this extension service is attached to.
    ///        If no key was set to the context yet then an empty COUID (Nil) is returned.
    /// @return the COUID of the CryptoObject
    /// @uptrace{SWS_CRYPT_29047}
    virtual CryptoObjectUid GetActualKeyCOUID() const noexcept = 0;

    /// @brief Get allowed usages of this context (according to the key object attributes loaded to this context).
    ///        If the context is not initialized by a key object yet then zero (all flags are reset) must be returned.
    /// @return a combination of bit-flags that specifies allowed usages of the context
    /// @uptrace{SWS_CRYPT_29046}
    virtual AllowedUsageFlags GetAllowedUsage() const noexcept = 0;

    /// @brief Get maximal supported key length in bits.
    /// @return maximal supported length of the key in bits
    /// @uptrace{SWS_CRYPT_29044}
    virtual std::size_t GetMaxKeyBitLength() const noexcept = 0;

    /// @brief Get minimal supported key length in bits.
    /// @return minimal supported length of the key in bits
    /// @uptrace{SWS_CRYPT_29043}
    virtual std::size_t GetMinKeyBitLength() const noexcept = 0;

    /// @brief Verify supportness of specific key length by the context.
    /// @param keyBitLength length of the key in bits
    /// @return true if provided value of the key length is supported by the context
    /// @uptrace{SWS_CRYPT_29048}
    virtual bool IsKeyBitLengthSupported(std::size_t keyBitLength) const noexcept = 0;

    /// @brief Check if a key has been set to this context.
    /// @return FALSE if no key has been set
    /// @uptrace{SWS_CRYPT_29049}
    virtual bool IsKeyAvailable() const noexcept = 0;

    /// @brief Copy-assign another ExtensionService to this instance.
    /// @param other the other instance
    /// @return *this, containing the contents of other
    /// @uptrace{SWS_CRYPT_30218}
    ExtensionService& operator=(const ExtensionService& other) = default;

    /// @brief Move-assign another ExtensionService to this instance.
    /// @param other the other instance
    /// @return *this, containing the contents of other
    /// @uptrace{SWS_CRYPT_30219}
    ExtensionService& operator=(ExtensionService&& other) = default;
};

}  // namespace cryp
}  // namespace crypto
}  // namespace ara

#endif  // ARA_CRYPTO_CRYP_EXTENSION_SERVICE_H