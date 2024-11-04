// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef ARA_CRYPTO_CRYP_DIGEST_SERVICE_H
#define ARA_CRYPTO_CRYP_DIGEST_SERVICE_H

#include <memory>

#include "ara/crypto/cryp/block_service.h"

namespace ara
{
namespace crypto
{
namespace cryp
{

/// @brief Extension meta-information service for digest producing contexts.
/// @uptrace{SWS_CRYPT_29010}
class DigestService : public BlockService
{
  public:
    /// @brief Unique smart pointer of the interface.
    /// @uptrace{SWS_CRYPT_29011}
    using Uptr = std::unique_ptr<DigestService>;

    /// @brief Get the output digest size.
    /// @return size of the full output from this digest-function in bytes
    /// @uptrace{SWS_CRYPT_29012}
    virtual std::size_t GetDigestSize() const noexcept = 0;

    /// @brief Check current status of the stream processing: finished or no.
    /// @return true if a previously started stream processing was finished by a call of the Finish() or FinishBytes()
    /// methods
    /// @uptrace{SWS_CRYPT_29015}
    virtual bool IsFinished() const noexcept = 0;

    /// @brief Check current status of the stream processing: started or no.
    /// @return true if the processing was start by a call of the Start() methods and was not finished yet
    /// @uptrace{SWS_CRYPT_29014}
    virtual bool IsStarted() const noexcept = 0;
};

}  // namespace cryp
}  // namespace crypto
}  // namespace ara

#endif  // ARA_CRYPTO_CRYP_DIGEST_SERVICE_H