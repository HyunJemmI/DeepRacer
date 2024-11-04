// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef ARA_CRYPTO_CRYP_CRYOBJ_CRYPTO_PRIMITIVE_ID_H
#define ARA_CRYPTO_CRYP_CRYOBJ_CRYPTO_PRIMITIVE_ID_H

#include <memory>

#include "ara/core/string_view.h"
#include "ara/crypto/common/base_id_types.h"

namespace ara
{
namespace crypto
{
namespace cryp
{

/// @brief Common interface for identification of all Crypto Primitives and their keys & parameters.
/// @uptrace{SWS_CRYPT_20600}
class CryptoPrimitiveId
{
  public:
    /// @brief Type definition of vendor specific binary Crypto Primitive ID.
    /// @uptrace{SWS_CRYPT_20641}
    using AlgId = CryptoAlgId;

    /// @brief type definition pointer
    /// @uptrace{SWS_CRYPT_20644}
    using Uptrc = std::unique_ptr<const CryptoPrimitiveId>;

    /// @brief type definition pointer to const
    /// @uptrace{SWS_CRYPT_20643}
    using Uptr = std::unique_ptr<CryptoPrimitiveId>;

    /// @brief Destructor
    /// @uptrace{SWS_CRYPT_10808}
    virtual ~CryptoPrimitiveId() noexcept = default;

    /// @brief Get vendor specific ID of the primitive.
    /// @return the binary Crypto Primitive ID
    /// @uptrace{SWS_CRYPT_20652}
    virtual AlgId GetPrimitiveId() const noexcept = 0;

    /// @brief Get a unified name of the primitive. The crypto primitive name can be fully or partially specified
    ///        (see "Crypto Primitives Naming Convention" for more details). The life-time of the returned
    ///        StringView instance should not exceed the life-time of this CryptoPrimitiveId instance!
    /// @return the unified name of the crypto primitive
    /// @uptrace{SWS_CRYPT_20651}
    virtual const ara::core::StringView GetPrimitiveName() const noexcept = 0;

    /// @brief Copy-assign another CryptoPrimitiveId to this instance.
    /// @param other the other instance
    /// @return *this, containing the contents of other
    /// @uptrace{SWS_CRYPT_30212}
    CryptoPrimitiveId& operator=(const CryptoPrimitiveId& other) = default;

    /// @brief Move-assign another CryptoPrimitiveId to this instance.
    /// @param other the other instance
    /// @return *this, containing the contents of other
    /// @uptrace{SWS_CRYPT_30213}
    CryptoPrimitiveId& operator=(CryptoPrimitiveId&& other) = default;
};

}  // namespace cryp
}  // namespace crypto
}  // namespace ara

#endif  // ARA_CRYPTO_CRYP_CRYOBJ_CRYPTO_PRIMITIVE_ID_H