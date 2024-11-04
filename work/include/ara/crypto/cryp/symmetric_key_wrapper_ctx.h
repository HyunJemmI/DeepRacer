// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef ARA_CRYPTO_CRYP_SYMMETRIC_KEY_WRAPPER_CTX_H
#define ARA_CRYPTO_CRYP_SYMMETRIC_KEY_WRAPPER_CTX_H

#include <memory>

#include "ara/crypto/common/base_id_types.h"
#include "ara/crypto/common/mem_region.h"
#include "ara/crypto/cryp/cryobj/restricted_use_object.h"
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

/// @brief Context of a symmetric key wrap algorithm (for AES it should be compatible with RFC3394 or RFC5649).
///        The public interface of this context is dedicated for raw key material wrapping/unwrapping,
///        i.e. without any meta-information assigned to the key material in source crypto object.
///        But additionally this context type should support some "hidden" low-level methods suitable
///        for whole crypto object exporting/importing.
///        Key Wrapping of a whole crypto object (including associated meta-information) can be done by methods:
///        ExportSecuredObject() and ImportSecuredObject(), but without compliance to RFC3394 or RFC5649.
/// @uptrace{SWS_CRYPT_24000}
class SymmetricKeyWrapperCtx : public CryptoContext
{
  public:
    /// @brief Unique smart pointer of the interface.
    /// @uptrace{SWS_CRYPT_24001}
    using Uptr = std::unique_ptr<SymmetricKeyWrapperCtx>;

    /// @brief Calculate size of the wrapped key in bytes from original key length in bits.
    ///        This method can be useful for some implementations different from RFC3394 / RFC5649.
    /// @param keyLength original key length in bits
    /// @return size of the wrapped key in bytes
    /// @uptrace{SWS_CRYPT_24013}
    virtual std::size_t CalculateWrappedKeySize(std::size_t keyLength) const noexcept = 0;

    /// @brief Get ExtensionService instance.
    /// @return --
    /// @uptrace{SWS_CRYPT_24002}
    virtual ExtensionService::Uptr GetExtensionService() const noexcept = 0;

    /// @brief Get maximum length of the target key supported by the implementation.
    ///        This method can be useful for some implementations different from RFC3394 / RFC5649.
    /// @return maximum length of the target key in bits
    /// @uptrace{SWS_CRYPT_24012}
    virtual std::size_t GetMaxTargetKeyLength() const noexcept = 0;

    /// @brief Get expected granularity of the target key (block size).
    ///        If the class implements RFC3394 (KW without padding) then this method should return 8 (i.e. 8 octets = 64
    ///        bits). If the class implements RFC5649 (KW with padding) then this method should return 1 (i.e. 1 octet =
    ///        8 bits).
    /// @return size of the block in bytes
    /// @uptrace{SWS_CRYPT_24011}
    virtual std::size_t GetTargetKeyGranularity() const noexcept = 0;

    /// @brief Clear the crypto context.
    /// @return --
    /// @uptrace{SWS_CRYPT_24019}
    virtual ara::core::Result<void> Reset() noexcept = 0;

    /// @brief Set (deploy) a key to the symmetric key wrapper algorithm context.
    /// @param key the source key object
    /// @param transform --
    /// @return --
    /// @uptrace{SWS_CRYPT_24018}
    virtual ara::core::Result<void> SetKey(const SymmetricKey& key, CryptoTransform transform) noexcept = 0;

    /// @brief Execute the "key unwrap" operation for provided BLOB and produce a Key object of expected type. For
    /// additional details see UnwrapKey()
    /// @tparam ExpectedKey the expected type of concrete key
    /// @param wrappedKey a memory region that contains wrapped key
    /// @param algId an identifier of the target symmetric crypto algorithm
    /// @param allowedUsage bit-flags that define a list of allowed transformations’ types in which the target key can
    /// be used
    /// @return unique smart pointer to ExpectedKey object, which keeps unwrapped key material
    /// @uptrace{SWS_CRYPT_24017}
    template <typename ExpectedKey>
    ara::core::Result<typename ExpectedKey::Uptrc> UnwrapConcreteKey(ReadOnlyMemRegion wrappedKey, AlgId algId,
                                                                     AllowedUsageFlags allowedUsage) noexcept;

    /// @brief Execute the "key unwrap" operation for provided BLOB and produce Key object.
    ///        This method should be compliant to RFC3394 or RFC5649, if implementation is based on the AES block cipher
    ///        and applied to an AES key. The created Key object has following attributes: session and non-exportable
    ///        (because it was imported without meta-information)! SymmetricKey may be unwrapped in following way:
    ///        SymmetricKey::Uptrc key = SymmetricKey::Cast(UnwrapKey(wrappedKey, ...)); PrivateKey may be unwrapped in
    ///        following way: PrivateKey::Uptrc key = PrivateKey::Cast(UnwrapKey(wrappedKey, ...)); In both examples the
    ///        Cast() method may additionally throw the BadObjectTypeException if an actual type of the unwrapped key
    ///        differs from the target one!
    /// @param wrappedKey a memory region that contains wrapped key
    /// @param algId an identifier of the target symmetric crypto algorithm
    /// @param allowedUsage bit-flags that define a list of allowed transformations’ types in which the target key can
    /// be used
    /// @return unique smart pointer to Key object, which keeps unwrapped key material
    /// @uptrace{SWS_CRYPT_24016}
    virtual ara::core::Result<RestrictedUseObject::Uptrc> UnwrapKey(ReadOnlyMemRegion wrappedKey, AlgId algId,
                                                                    AllowedUsageFlags allowedUsage) const noexcept = 0;

    /// @brief Execute the "key unwrap" operation for provided BLOB and produce SecretSeed object.
    ///        This method should be compliant to RFC3394 or RFC5649, if implementation is based on the AES block cipher
    ///        and applied to an AES key material. The created SecretSeed object has following attributes: session and
    ///        non-exportable (because it was imported without meta-information).
    /// @param wrappedSeed a memory region that contains wrapped seed
    /// @param targetAlgId the target symmetric algorithm identifier (also defines a target seed-length)
    /// @param allowedUsage allowed usage scope of the target seed
    /// @return unique smart pointer to SecretSeed object, which keeps unwrapped key material
    /// @uptrace{SWS_CRYPT_24015}
    virtual ara::core::Result<SecretSeed::Uptrc> UnwrapSeed(ReadOnlyMemRegion wrappedSeed, AlgId targetAlgId,
                                                            SecretSeed::Usage allowedUsage) const noexcept = 0;

    /// @brief Execute the "key wrap" operation for the provided key material.
    ///        This method should be compliant to RFC3394 or RFC5649, if an implementation is based on the AES block
    ///        cipher and applied to an AES key. Method CalculateWrappedKeySize() can be used for size calculation of
    ///        the required output buffer.
    /// @param key a key that should be wrapped
    /// @return --
    /// @uptrace{SWS_CRYPT_24014}
    virtual ara::core::Result<ara::core::Vector<ara::core::Byte>> WrapKeyMaterial(
        const RestrictedUseObject& key) const noexcept = 0;
};

}  // namespace cryp
}  // namespace crypto
}  // namespace ara

#endif  // ARA_CRYPTO_CRYP_SYMMETRIC_KEY_WRAPPER_CTX_H