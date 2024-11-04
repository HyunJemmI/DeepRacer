// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef ARA_CRYPTO_COMMON_ENTRY_POINT_H
#define ARA_CRYPTO_COMMON_ENTRY_POINT_H

#include <cstdint>

#include "ara/core/instance_specifier.h"
#include "ara/core/result.h"
#include "ara/core/utility.h"
#include "ara/core/vector.h"
#include "ara/crypto/cryp/crypto_provider.h"

/// @todo #include "something"

namespace ara
{
namespace crypto
{

/// @brief 128 bit secure counter made up of most significant and least significant quad-word of the hardware counter.
/// @uptrace{SWS_CRYPT_30001}
struct SecureCounter
{
    /// @brief least significant 64 bits
    /// @uptrace{SWS_CRYPT_30002}
    std::uint64_t mLSQW;
    /// @brief most significant 64 bits
    /// @uptrace{SWS_CRYPT_30003}
    std::uint64_t mMSQW;
};

/// @brief  Factory that creates or return existing single instance of specific Crypto Provider.
///         If (providerUid == nullptr) then platform default provider should be loaded.
/// @param iSpecify the globally unique identifier of required Crypto Provider
/// @return unique smart pointer to loaded Crypto Provider
/// @uptrace{SWS_CRYPT_20099}
ara::crypto::cryp::CryptoProvider::Uptr LoadCryptoProvider(const ara::core::InstanceSpecifier& iSpecify) noexcept;

/// @brief Factory that creates or return existing single instance of the Key Storage Provider.
/// @return unique smart pointer to loaded Key Storage Provider
/// @uptrace{SWS_CRYPT_30099}
ara::crypto::keys::KeyStorageProvider::Uptr LoadKeyStorageProvider() noexcept;

/// @brief Factory that creates or return existing single instance of the X.509 Provider.
///        X.509 Provider should use the default Crypto Provider for hashing and signature verification!
///        Therefore when you load the X.509 Provider, in background it loads the default Crypto Provider too.
/// @return unique smart pointer to loaded X.509 Provider
ara::crypto::x509::X509Provider::Uptr LoadX509Provider() noexcept;

/// @brief Return an allocated buffer with a generated random sequence of the requested size.
/// @param count number of random bytes to generate
/// @return a buffer filled with the generated random sequence
ara::core::Result<ara::core::Vector<ara::core::Byte>> GenerateRandomData(std::uint32_t count) noexcept;

/// @brief Get current value of 128 bit Secure Counter supported by the Crypto Stack.
///        Secure Counter is a non-rollover monotonic counter that ensures incrementation of its value for each
///        following call. The Secure Counter is presented by two 64 bit components: Most Significant Quadword (MSQW)
///        and Least Significant Quadword (LSQW). During normal operation of the Crypto Stack, the MSQW value is fixed
///        (unchangeable) and only LSQW should be incremented. The LSQW counter can be implemented in the "low-power"
///        (always-powered-up) domain of the main CPU, but the MSQW in the Flash/EEPROM storage. But the MSQW must be
///        incremented if the LSQW reaches the maximum value of all ones. Also the MSQW must be incremented during
///        reinitialisation of the whole Crypto Stack (e.g. if the "low-power" supply was interrupted by some reason).
///        Permission to execute this routine is subject of Identity and Access Management control and may be restricted
///        by application manifest!
/// @return a SecureCounter struct made up of the two unsigned 64 bit values (LSQW and MSQW)
ara::core::Result<SecureCounter> GetSecureCounter() noexcept;

}  // namespace crypto
}  // namespace ara

#endif  // ARA_CRYPTO_COMMON_ENTRY_POINT_H