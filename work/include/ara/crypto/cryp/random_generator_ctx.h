// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef ARA_CRYPTO_CRYP_RANDOM_GENERATOR_CTX_H
#define ARA_CRYPTO_CRYP_RANDOM_GENERATOR_CTX_H

#include <memory>

#include "ara/core/result.h"
#include "ara/core/utility.h"
#include "ara/core/vector.h"
#include "ara/crypto/common/mem_region.h"
#include "ara/crypto/cryp/cryobj/secret_seed.h"
#include "ara/crypto/cryp/cryobj/symmetric_key.h"
#include "ara/crypto/cryp/crypto_context.h"
#include "ara/crypto/cryp/extension_service.h"

namespace ara
{
namespace crypto
{
namespace cryp
{

/// @brief Interface of Random Number Generator Context.
/// @uptrace{SWS_CRYPT_22900}
class RandomGeneratorCtx : public CryptoContext
{
  public:
    /// @brief Shared smart pointer of the interface.
    /// @uptrace{SWS_CRYPT_22901}
    using Uptr = std::unique_ptr<RandomGeneratorCtx>;

    /// @brief Update the internal state of the RNG by mixing it with the provided additional entropy.
    ///        This method is optional for implementation. An implementation of this method may "accumulate" provided
    ///        entropy for future use.
    /// @param entropy a memory region with the additional entropy value
    /// @return true if the method is supported and the entropy has been updated successfully
    /// @uptrace{SWS_CRYPT_22914}
    virtual bool AddEntropy(ReadOnlyMemRegion entropy) noexcept = 0;

    /// @brief Return an allocated buffer with a generated random sequence of the requested size.
    /// @param count number of random bytes to generate
    /// @return a buffer filled with the generated random sequence
    /// @uptrace{SWS_CRYPT_22915}
    virtual ara::core::Result<ara::core::Vector<ara::core::Byte>> Generate(std::uint32_t count) noexcept = 0;

    /// @brief Get ExtensionService instance.
    /// @return --
    /// @uptrace{SWS_CRYPT_22902}
    virtual ExtensionService::Uptr GetExtensionService() const noexcept = 0;

    /// @brief Set the internal state of the RNG using the provided seed.
    /// @param seed a memory region with the seed value
    /// @return true if the method is supported and the state has been set successfully
    /// @uptrace{SWS_CRYPT_22911}
    virtual bool Seed(ReadOnlyMemRegion seed) noexcept = 0;

    /// @brief Set the internal state of the RNG using the provided seed.
    /// @param seed a memory region with the seed value
    /// @return true if the method is supported and the state has been set successfully
    /// @uptrace{SWS_CRYPT_22912}
    virtual bool Seed(const SecretSeed& seed) noexcept = 0;

    /// @brief Set the internal state of the RNG using the provided seed.
    /// @param key a SymmetricKey with the key used as seed value
    /// @return true if the method is supported and the key has been set successfully
    /// @uptrace{SWS_CRYPT_22913}
    virtual bool SetKey(const SymmetricKey& key) noexcept = 0;
};

}  // namespace cryp
}  // namespace crypto
}  // namespace ara

#endif  // ARA_CRYPTO_CRYP_RANDOM_GENERATOR_CTX_H