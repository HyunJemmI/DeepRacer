// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef ARA_CRYPTO_CRYP_SIGNATURE_SERVICE_H
#define ARA_CRYPTO_CRYP_SIGNATURE_SERVICE_H

#include <memory>

#include "ara/crypto/cryp/cryobj/crypto_primitive_id.h"
#include "ara/crypto/cryp/extension_service.h"

namespace ara
{
namespace crypto
{
namespace cryp
{

/// @brief Extension meta-information service for signature contexts.
/// @uptrace{SWS_CRYPT_29000}
class SignatureService : public ExtensionService
{
  public:
    /// @brief Unique smart pointer of the interface.
    /// @uptrace{SWS_CRYPT_29001}
    using Uptr = std::unique_ptr<SignatureService>;

    /// @brief Get an ID of hash algorithm required by current signature algorithm.
    /// @return required hash algorithm ID or kAlgIdAny if the signature algorithm specification does not include a
    /// concrete hash function
    /// @uptrace{SWS_CRYPT_29003}
    virtual CryptoPrimitiveId::AlgId GetRequiredHashAlgId() const noexcept = 0;

    /// @brief Get the hash size required by current signature algorithm.
    /// @return required hash size in bytes
    /// @uptrace{SWS_CRYPT_29002}
    virtual std::size_t GetRequiredHashSize() const noexcept = 0;

    /// @brief Get size of the signature value produced and required by the current algorithm.
    /// @return size of the signature value in bytes
    /// @uptrace{SWS_CRYPT_29004}
    virtual std::size_t GetSignatureSize() const noexcept = 0;
};

}  // namespace cryp
}  // namespace crypto
}  // namespace ara

#endif  // ARA_CRYPTO_CRYP_SIGNATURE_SERVICE_H