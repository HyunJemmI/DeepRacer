// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef ARA_CRYPTO_CRYP_RESTRICTED_USE_OBJECT_H
#define ARA_CRYPTO_CRYP_RESTRICTED_USE_OBJECT_H

#include <memory>

#include "ara/crypto/cryp/cryobj/crypto_object.h"

namespace ara
{
namespace crypto
{
namespace cryp
{

/// @brief A common interface for all objects supporting the usage restriction.
/// @uptrace{SWS_CRYPT_24800}
class RestrictedUseObject : public CryptoObject
{
  public:
    /// @brief Alias to the container type for bit-flags of allowed usages of the object.
    /// @uptrace{SWS_CRYPT_24801}
    using Usage = AllowedUsageFlags;

    /// @brief Unique smart pointer of the interface.
    /// @uptrace{SWS_CRYPT_24802}
    using Uptrc = std::unique_ptr<RestrictedUseObject>;

    /// @brief Get allowed usages of this object.
    /// @return a combination of bit-flags that specifies allowed applications of the object
    /// @uptrace{SWS_CRYPT_24811}
    virtual Usage GetAllowedUsage() const noexcept = 0;
};

}  // namespace cryp
}  // namespace crypto
}  // namespace ara

#endif  // ARA_CRYPTO_CRYP_RESTRICTED_USE_OBJECT_H