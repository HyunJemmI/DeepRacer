// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef ARA_CRYPTO_CRYP_CRYPTO_CONTEXT_H
#define ARA_CRYPTO_CRYP_CRYPTO_CONTEXT_H

#include "ara/crypto/common/base_id_types.h"
#include "ara/crypto/cryp/cryobj/crypto_primitive_id.h"

namespace ara
{
namespace crypto
{
namespace cryp
{

/// @brief A common interface of a mutable cryptographic context,
///        i.e. that is not binded to a single crypto object.
/// @uptrace{SWS_CRYPT_20400}
class CryptoContext
{
  public:
    /// @brief Type definition of vendor specific binary Crypto Primitive ID.
    /// @uptrace{SWS_CRYPT_20402}
    using AlgId = CryptoAlgId;

    /// @brief Destructor
    /// @uptrace{SWS_CRYPT_20401}
    virtual ~CryptoContext() noexcept = default;

    /// @brief Return CryptoPrimitivId instance containing instance identification.
    /// @return --
    /// @uptrace{SWS_CRYPT_20411}
    virtual CryptoPrimitiveId::Uptr GetCryptoPrimitiveId() const noexcept = 0;

    /// @brief Check if the crypto context is already initialized and ready to use.
    ///        It checks all required values, including: key value, IV/seed, etc.
    /// @return true if the crypto context is completely initialized and ready to use, and false otherwise
    /// @uptrace{SWS_CRYPT_20412}
    virtual bool IsInitialized() const noexcept = 0;

    /// @brief Copy-assign another CryptoContext to this instance.
    /// @param other the other instance
    /// @return *this, containing the contents of other
    /// @uptrace{SWS_CRYPT_30214}
    CryptoContext& operator=(const CryptoContext& other) = default;

    /// @brief Move-assign another CryptoContext to this instance.
    /// @param other the other instance
    /// @return *this, containing the contents of other
    /// @uptrace{SWS_CRYPT_30215}
    CryptoContext& operator=(CryptoContext&& other) = default;

    /// @brief Get a reference to Crypto Provider of this context.
    /// @return a reference to Crypto Provider instance that provides this context
    /// @uptrace{SWS_CRYPT_20654}
    virtual CryptoProvider& MyProvider() const noexcept = 0;
};

}  // namespace cryp
}  // namespace crypto
}  // namespace ara

#endif  // ARA_CRYPTO_CRYP_CRYPTO_CONTEXT_H