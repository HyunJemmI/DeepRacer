// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef ARA_CRYPTO_COMMON_VOLATILE_TRUSTED_CONTAINER_H
#define ARA_CRYPTO_COMMON_VOLATILE_TRUSTED_CONTAINER_H

#include <memory>

#include "ara/crypto/common/io_interface.h"

namespace ara
{
namespace crypto
{

/// @brief This explicit interface of a volatile Trusted Container is used for buffering CryptoAPI objects in RAM.
///        This class represents a "smart buffer" in that it provides access to the IOInterface,
///        which can be used for querying meta-data of the buffer content.
/// @uptrace{SWS_CRYPT_10850}
class VolatileTrustedContainer
{
    /// @brief Destructor
    /// @uptrace{SWS_CRYPT_10851}
    virtual ~VolatileTrustedContainer() noexcept = default;

    /// @brief Retrieve the IOInterface used for importing/exporting objects into this container.
    /// @return a reference to the IOInterface of this container
    /// @uptrace{SWS_CRYPT_10853}
    virtual IOInterface& GetIOInterface() const noexcept = 0;

    /// @brief Copy-assign another VolatileTrustedContainer to this instance.
    /// @param other the other instance
    /// @return *this, containing the contents of other
    /// @uptrace{SWS_CRYPT_30206}
    VolatileTrustedContainer& operator=(const VolatileTrustedContainer& other) = default;

    /// @brief Move-assign another VolatileTrustedContainer to this instance.
    /// @param other the other instance
    /// @return *this, containing the contents of other
    /// @uptrace{SWS_CRYPT_30207}
    VolatileTrustedContainer& operator=(VolatileTrustedContainer&& other) = default;
};

/// @brief Unique smart pointer of the interface.
/// @uptrace{SWS_CRYPT_10852}
using Uptr = std::unique_ptr<VolatileTrustedContainer>;

}  // namespace crypto
}  // namespace ara

#endif  // ARA_CRYPTO_COMMON_VOLATILE_TRUSTED_CONTAINER_H