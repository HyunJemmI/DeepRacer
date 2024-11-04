// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef ARA_CRYPTO_CRYP_CRYPTO_PROVIDER_H
#define ARA_CRYPTO_CRYP_CRYPTO_PROVIDER_H

#include <memory>
#include <utility>

#include "ara/core/result.h"
#include "ara/core/string.h"
#include "ara/core/string_view.h"
#include "ara/core/utility.h"
#include "ara/core/vector.h"
#include "ara/crypto/common/io_interface.h"
#include "ara/crypto/common/mem_region.h"
#include "ara/crypto/common/serializable.h"
#include "ara/crypto/common/volatile_trusted_container.h"
#include "ara/crypto/cryp/auth_cipher_ctx.h"
#include "ara/crypto/cryp/cryobj/crypto_primitive_id.h"
#include "ara/crypto/cryp/cryobj/private_key.h"
#include "ara/crypto/cryp/cryobj/public_key.h"
#include "ara/crypto/cryp/cryobj/restricted_use_object.h"
#include "ara/crypto/cryp/cryobj/secret_seed.h"
#include "ara/crypto/cryp/cryobj/signature.h"
#include "ara/crypto/cryp/cryobj/symmetric_key.h"
#include "ara/crypto/cryp/decryptor_private_ctx.h"
#include "ara/crypto/cryp/encryptor_public_ctx.h"
#include "ara/crypto/cryp/hash_function_ctx.h"
#include "ara/crypto/cryp/key_agreement_private_ctx.h"
#include "ara/crypto/cryp/key_decapsulator_private_ctx.h"
#include "ara/crypto/cryp/key_derivation_function_ctx.h"
#include "ara/crypto/cryp/key_encapsulator_public_ctx.h"
#include "ara/crypto/cryp/message_authn_code_ctx.h"
#include "ara/crypto/cryp/msg_recovery_public_ctx.h"
#include "ara/crypto/cryp/random_generator_ctx.h"
#include "ara/crypto/cryp/sig_encode_private_ctx.h"
#include "ara/crypto/cryp/signer_private_ctx.h"
#include "ara/crypto/cryp/stream_cipher_ctx.h"
#include "ara/crypto/cryp/symmetric_block_cipher_ctx.h"
#include "ara/crypto/cryp/symmetric_key_wrapper_ctx.h"
#include "ara/crypto/cryp/verifier_public_ctx.h"

namespace ara
{
namespace crypto
{
namespace cryp
{

/// @brief Crypto Provider is a "factory" interface of all supported Crypto Primitives and a "trusted
///        environmet" for internal communications between them. All Crypto Primitives should have an
///        actual reference to their parent Crypto Provider. A Crypto Provider can be destroyed only after
///        destroying of all its daughterly Crypto Primitives. Each method of this interface that creates a
///        Crypto Primitive instance is non-constant, because any such creation increases a references
///        counter of the Crypto Primitive.
/// @uptrace{SWS_CRYPT_20700}
class CryptoProvider
{
  public:
    /// @brief A short alias for Algorithm ID type definition.
    /// @uptrace{SWS_CRYPT_20703}
    using AlgId = CryptoPrimitiveId::AlgId;

    /// @brief Shared smart pointer of the interface.
    /// @uptrace{SWS_CRYPT_20701}
    using Uptr = std::unique_ptr<CryptoProvider>;

    /// @brief Destructor.
    /// @uptrace{SWS_CRYPT_20710}
    virtual ~CryptoProvider() noexcept = default;

    /// @brief Allocate a Volatile (virtual) Trusted Container according to directly specified capacity.
    ///        The Volatile Trusted Container can be used for execution of the import operations.
    ///        Current process obtains the "Owner" rights for allocated Container. If (capacity == 0) then the capacity
    ///        of the container will be selected automatically according to a maximal size of supported crypto objects.
    ///        A few volatile (temporary) containers can coexist at same time without any affecting each-other.
    /// @param capacity the capacity required for this volatile trusted container (in bytes)
    /// @return unique smart pointer to an allocated volatile trusted container
    /// @uptrace{SWS_CRYPT_20726}
    virtual ara::core::Result<VolatileTrustedContainer::Uptr> AllocVolatileContainer(
        std::size_t capacity = 0) noexcept = 0;

    /// @brief Allocate a Volatile (virtual) Trusted Container according to indirect specification of a minimal
    ///        required capacity for hosting of any listed object. The Volatile Trusted Container can be used for
    ///        execution of the import operations. Current process obtains the "Owner" rights for allocated Container.
    ///        Real container capacity is calculated as a maximal storage size of all listed objects.
    /// @param theObjectDef the list of objects that can be stored to this volatile trusted container
    /// @return unique smart pointer to an allocated volatile trusted container
    /// @uptrace{SWS_CRYPT_20727}
    virtual ara::core::Result<VolatileTrustedContainer::Uptr> AllocVolatileContainer(
        std::pair<AlgId, CryptoObjectType> theObjectDef) noexcept = 0;

    /// @brief Convert a common name of crypto algorithm to a correspondent vendor specific binary algorithm ID.
    /// @param primitiveName the unified name of the crypto primitive (see "Crypto Primitives Naming Convention" for
    /// more details)
    /// @return vendor specific binary algorithm ID or kAlgId Undefined if a primitive with provided name is not
    /// supported
    /// @uptrace{SWS_CRYPT_20711}
    virtual AlgId ConvertToAlgId(ara::core::StringView primitiveName) const noexcept = 0;

    /// @brief Convert a vendor specific binary algorithm ID to a correspondent common name of the crypto algorithm.
    /// @param algId the vendor specific binary algorithm ID
    /// @return the common name of the crypto algorithm (see "Crypto Primitives Naming Convention" for more details)
    /// @uptrace{SWS_CRYPT_20712}
    virtual ara::core::Result<ara::core::String> ConvertToAlgName(AlgId algId) const noexcept = 0;

    /// @brief Create a symmetric authenticated cipher context.
    /// @param algId identifier of the target crypto algorithm
    /// @return unique smart pointer to the created context
    /// @uptrace{SWS_CRYPT_20745}
    virtual ara::core::Result<AuthCipherCtx::Uptr> CreateAuthCipherCtx(AlgId algId) noexcept = 0;

    /// @brief Create a decryption private key context.
    /// @param algId identifier of the target asymmetric encryption/decryption algorithm
    /// @return unique smart pointer to the created context
    /// @uptrace{SWS_CRYPT_20751}
    virtual ara::core::Result<DecryptorPrivateCtx::Uptr> CreateDecryptorPrivateCtx(AlgId algId) noexcept = 0;

    /// @brief Create an encryption public key context.
    /// @param algId identifier of the target asymmetric encryption/decryption algorithm
    /// @return unique smart pointer to the created context
    /// @uptrace{SWS_CRYPT_20750}
    virtual ara::core::Result<EncryptorPublicCtx::Uptr> CreateEncryptorPublicCtx(AlgId algId) noexcept = 0;

    /// @brief Construct Signature object from directly provided components of a hash digest.
    /// @param hashAlgId identifier of an applied hash function crypto algorithm
    /// @param value raw BLOB value of the hash digest
    /// @return unique smart pointer to the created Signature object
    /// @uptrace{SWS_CRYPT_20761}
    virtual ara::core::Result<Signature::Uptrc> CreateHashDigest(AlgId hashAlgId, ReadOnlyMemRegion value) noexcept = 0;

    /// @brief Create a hash function context.
    /// @param algId identifier of the target crypto algorithm
    /// @return unique smart pointer to the created context
    /// @uptrace{SWS_CRYPT_20747}
    virtual ara::core::Result<HashFunctionCtx::Uptr> CreateHashFunctionCtx(AlgId algId) noexcept = 0;

    /// @brief Create a key-agreement private key context.
    /// @param algId identifier of the target key-agreement crypto algorithm
    /// @return unique smart pointer to the created context
    /// @uptrace{SWS_CRYPT_20758}
    virtual ara::core::Result<KeyAgreementPrivateCtx::Uptr> CreateKeyAgreementPrivateCtx(AlgId algId) noexcept = 0;

    /// @brief Create a key-decapsulator private key context of a Key Encapsulation Mechanism (KEM).
    /// @param algId identifier of the target KEM crypto algorithm
    /// @return unique smart pointer to the created context
    /// @uptrace{SWS_CRYPT_20753}
    virtual ara::core::Result<KeyDecapsulatorPrivateCtx::Uptr> CreateKeyDecapsulatorPrivateCtx(
        AlgId algId) noexcept = 0;

    /// @brief Create a key derivation function context.
    /// @param algId identifier of the target crypto algorithm
    /// @return unique smart pointer to the created context
    /// @uptrace{SWS_CRYPT_20748}
    virtual ara::core::Result<KeyDerivationFunctionCtx::Uptr> CreateKeyDerivationFunctionCtx(AlgId algId) noexcept = 0;

    /// @brief Create a key-encapsulator public key context of a Key Encapsulation Mechanism (KEM).
    /// @param algId identifier of the target KEM crypto algorithm
    /// @return unique smart pointer to the created context
    /// @uptrace{SWS_CRYPT_20752}
    virtual ara::core::Result<KeyEncapsulatorPublicCtx::Uptr> CreateKeyEncapsulatorPublicCtx(AlgId algId) noexcept = 0;

    /// @brief Create a symmetric message authentication code context.
    /// @param algId identifier of the target crypto algorithm
    /// @return unique smart pointer to the created context
    /// @uptrace{SWS_CRYPT_20746}
    virtual ara::core::Result<MessageAuthnCodeCtx::Uptr> CreateMessageAuthCodeCtx(AlgId algId) noexcept = 0;

    /// @brief Create a message recovery public key context.
    /// @param algId identifier of the target asymmetric crypto algorithm
    /// @return unique smart pointer to the created context
    /// @uptrace{SWS_CRYPT_20755}
    virtual ara::core::Result<MsgRecoveryPublicCtx::Uptr> CreateMsgRecoveryPublicCtx(AlgId algId) noexcept = 0;

    /// @brief Create a Random Number Generator (RNG) context.
    /// @param algId identifier of target RNG algorithm. If no algId is given, the default RNG is returned
    /// @param initialize indicates whether the returned context shall be initialized (i.e., seeded) by the stack
    /// @return unique smart pointer to the created RNG context
    /// @uptrace{SWS_CRYPT_20741}
    virtual ara::core::Result<RandomGeneratorCtx::Uptr> CreateRandomGeneratorCtx(AlgId algId = kAlgIdDefault,
                                                                                 bool initialize = true) noexcept = 0;

    /// @brief Create a signature encoding private key context.
    /// @param algId identifier of the target asymmetric crypto algorithm
    /// @return unique smart pointer to the created context
    /// @uptrace{SWS_CRYPT_20754}
    virtual ara::core::Result<SigEncodePrivateCtx::Uptr> CreateSigEncodePrivateCtx(AlgId algId) noexcept = 0;

    /// @brief Construct Signature object from directly provided components of a digital signature/MAC or authenticated
    /// encryption (AE/AEAD).
    ///        All integers inside a digital signature BLOB value are always presented in Big Endian bytes order (i.e.
    ///        MSF - Most Significant byte First).
    /// @param signAlgId identifier of an applied signature/MAC/AE/AEAD crypto algorithm
    /// @param value raw BLOB value of the signature/MAC
    /// @param key symmetric or asymmetric key (according to signAlgId) applied for the sign or MAC/AE/AEAD operation
    /// @param hashAlgId identifier of a hash function algorithm applied together with the signature algorithm
    /// @return unique smart pointer to the created Signature object
    /// @uptrace{SWS_CRYPT_20760}
    virtual ara::core::Result<Signature::Uptrc> CreateSignature(AlgId signAlgId, ReadOnlyMemRegion value,
                                                                const RestrictedUseObject& key,
                                                                AlgId hashAlgId = kAlgIdNone) noexcept = 0;

    /// @brief Create a signature private key context.
    /// @param algId identifier of the target signature crypto algorithm
    /// @return unique smart pointer to the created context
    /// @uptrace{SWS_CRYPT_20756}
    virtual ara::core::Result<SignerPrivateCtx::Uptr> CreateSignerPrivateCtx(AlgId algId) noexcept = 0;

    /// @brief Create a symmetric stream cipher context.
    /// @param algId identifier of the target crypto algorithm
    /// @return unique smart pointer to the created context
    /// @uptrace{SWS_CRYPT_20744}
    virtual ara::core::Result<StreamCipherCtx::Uptr> CreateStreamCipherCtx(AlgId algId) noexcept = 0;

    /// @brief Create a symmetric block cipher context.
    /// @param algId identifier of the target crypto algorithm
    /// @return unique smart pointer to the created context
    /// @uptrace{SWS_CRYPT_20742}
    virtual ara::core::Result<SymmetricBlockCipherCtx::Uptr> CreateSymmetricBlockCipherCtx(AlgId algId) noexcept = 0;

    /// @brief Create a symmetric key-wrap algorithm context.
    /// @param algId identifier of the target crypto algorithm
    /// @return unique smart pointer to the created context
    /// @uptrace{SWS_CRYPT_20743}
    virtual ara::core::Result<SymmetricKeyWrapperCtx::Uptr> CreateSymmetricKeyWrapperCtx(AlgId algId) noexcept = 0;

    /// @brief Create a signature verification public key context.
    /// @param algId identifier of the target signature crypto algorithm
    /// @return unique smart pointer to the created context
    /// @uptrace{SWS_CRYPT_20757}
    virtual ara::core::Result<VerifierPublicCtx::Uptr> CreateVerifierPublicCtx(AlgId algId) noexcept = 0;

    /// @brief Export publicly an object from a IOInterface (i.e. without an intermediate creation of a crypto object).
    /// @param container the IOInterface that contains an object for export
    /// @param formatId the CryptoProvider specific identifier of the output format
    /// @return actual capacity required for the serialized data
    /// @uptrace{SWS_CRYPT_20731}
    virtual ara::core::Result<ara::core::Vector<ara::core::Byte>> ExportPublicObject(
        const IOInterface& container, Serializable::FormatId formatId = Serializable::kFormatDefault) noexcept = 0;

    /// @brief Export a crypto object in a secure manner. if (serialized.empty() == true) then the method returns
    /// required size only,
    ///        but content of the transportContext stays unchanged!
    ///        Only an exportable and completed object (i.e. that have a UUID) can be exported!
    /// @param object the crypto object for export
    /// @param transportContext the symmetric key wrap context initialized by a transport key (allowed usage:
    /// kAllowKeyExporting)
    /// @return the wrapped crypto object data
    /// @uptrace{SWS_CRYPT_20728}
    virtual ara::core::Result<ara::core::Vector<ara::core::Byte>> ExportSecuredObject(
        const CryptoObject& object, SymmetricKeyWrapperCtx& transportContext) noexcept = 0;

    /// @brief Export securely an object directly from an IOInterface (i.e. without an intermediate creation of a crypto
    /// object).
    ///        if (serialized == nullptr) then the method returns required size only, but content of the
    ///        transportContext stays unchanged. This method can be used for re-exporting of just imported object but on
    ///        another transport key.
    /// @param container the IOInterface that refers an object for export
    /// @param transportContext the symmetric key wrap context initialized by a transport key (allowed usage:
    /// kAllowKeyExporting)
    /// @return actual capacity required for the serialized data
    /// @uptrace{SWS_CRYPT_20729}
    virtual ara::core::Result<ara::core::Vector<ara::core::Byte>> ExportSecuredObject(
        const IOInterface& container, SymmetricKeyWrapperCtx& transportContext) noexcept = 0;

    /// @brief Allocate a new private key context of correspondent type and generates the key value randomly.
    ///        A common COUID should be shared for both private and public keys. Any serializable (i.e.
    ///        savable/non-session or exportable) key must generate own COUID!
    /// @param algId the identifier of target public-private key crypto algorithm
    /// @param allowedUsage the flags that define a list of allowed transformations’ types in which the target key can
    /// be used (see constants in scope of RestrictedUseObject)
    /// @param isSession the "session" (or "temporary") attribute for the target key (if true)
    /// @param isExportable the exportability attribute of the target key (if true)
    /// @return smart unique pointer to the created private key object
    /// @uptrace{SWS_CRYPT_20722}
    virtual ara::core::Result<PrivateKey::Uptrc> GeneratePrivateKey(AlgId algId, AllowedUsageFlags allowedUsage,
                                                                    bool isSession = false,
                                                                    bool isExportable = false) noexcept = 0;

    /// @brief Generate a random Secret Seed object of requested algorithm.
    /// @param algId the identifier of target crypto algorithm
    /// @param allowedUsage the lags that define a list of allowed transformations’ types in which the target seed can
    /// be used (see constants in scope of RestrictedUseObject)
    /// @param isSession the "session" (or "temporary") attribute of the target seed (if true)
    /// @param isExportable the exportability attribute of the target seed (if true)
    /// @return unique smart pointer to generated SecretSeed object
    /// @uptrace{SWS_CRYPT_20723}
    virtual ara::core::Result<SecretSeed::Uptrc> GenerateSeed(AlgId algId, SecretSeed::Usage allowedUsage,
                                                              bool isSession = true,
                                                              bool isExportable = false) noexcept = 0;

    /// @brief Allocate a new symmetric key object and fill it by a new randomly generated value.
    ///        Any serializable (i.e. savable/non-session or exportable) key must generate own COUID!
    ///        By default Crypto Provider should use an internal instance of a best from all supported RNG (ideally
    ///        TRNG).
    /// @param algId the identifier of target symmetric crypto algorithm
    /// @param allowedUsage the flags that define a list of allowed transformations’ types in which the target key can
    /// be used (see constants in scope of RestrictedUseObject)
    /// @param isSession the "session" (or "temporary") attribute of the target key (if true)
    /// @param isExportable the exportability attribute of the target key (if true)
    /// @return smart unique pointer to the created symmetric key object
    /// @uptrace{SWS_CRYPT_20721}
    virtual ara::core::Result<SymmetricKey::Uptrc> GenerateSymmetricKey(AlgId algId, AllowedUsageFlags allowedUsage,
                                                                        bool isSession = true,
                                                                        bool isExportable = false) noexcept = 0;

    /// @brief Return minimally required capacity of a key slot for saving of the object’s payload.
    ///        Returned value does not take into account the object’s meta-information properties, but their size is
    ///        fixed and common for all crypto objects independently from their actual type. During an allocation of a
    ///        TrustedContainer, Crypto Providers (and Key Storage Providers) reserve space for an object’s
    ///        meta-information automatically, according to their implementation details.
    /// @param cryptoObjectType --
    /// @param algId a CryptoProvider algorithm ID of the target object
    /// @return minimal size required for storing of the object in a TrustedContainer (persistent or volatile)
    /// @uptrace{SWS_CRYPT_20725}
    virtual ara::core::Result<std::size_t> GetPayloadStorageSize(CryptoObjectType cryptoObjectType,
                                                                 AlgId algId) const noexcept = 0;

    /// @brief Return required buffer size for serialization of an object in specific format.
    /// @param cryptoObjectType --
    /// @param algId the Crypto Provider algorithm ID of the target object
    /// @param formatId the Crypto Provider specific identifier of the output format
    /// @return size required for storing of the object serialized in the specified format
    /// @uptrace{SWS_CRYPT_20724}
    virtual ara::core::Result<std::size_t> GetSerializedSize(
        CryptoObjectType cryptoObjectType, AlgId algId,
        Serializable::FormatId formatId = Serializable::kFormatDefault) const noexcept = 0;

    /// @brief Import publicly serialized object to a storage location pointed to by an IOInterface for following
    /// processing (without allocation of a crypto object).
    ///        If (expectedObject != CryptoObjectType::kUnknown) and an actual object type differs from the expected one
    ///        then this method fails. If the serialized contains incorrect data then this method fails.
    /// @param container the IOInterface for storing of the imported object
    /// @param serialized the memory region that contains a securely serialized object that should be imported to the
    /// IOInterface
    /// @param expectedObject the expected object type (default value CryptoObjectType::kUnknown means without check)
    /// @return the IOInterface for storing of the imported object
    /// @uptrace{SWS_CRYPT_20732}
    virtual ara::core::Result<void> ImportPublicObject(
        IOInterface& container, ReadOnlyMemRegion serialized,
        CryptoObjectType expectedObject = CryptoObjectType::kUndefined) noexcept = 0;

    /// @brief Import securely serialized object to the persistent or volatile storage represented by an IOInterface for
    /// following processing.
    /// @param container the IOInterface for storing of the imported object
    /// @param serialized the memory region that contains a securely serialized object that should be imported to the
    /// IOInterface
    /// @param transportContext the symmetric key wrap context initialized by a transport key (allowed usage:
    /// kAllowKeyImporting)
    /// @param isExportable the exportability attribute of the target object
    /// @param expectedObject the expected object type (default value CryptoObjectType::kUnknown means without check)
    /// @return --
    /// @uptrace{SWS_CRYPT_20730}
    virtual ara::core::Result<void> ImportSecuredObject(
        IOInterface& container, ReadOnlyMemRegion serialized, SymmetricKeyWrapperCtx& transportContext,
        bool isExportable = false, CryptoObjectType expectedObject = CryptoObjectType::kUndefined) noexcept = 0;

    /// @brief Load any crypto object from the IOInterface provided.
    /// @param container the IOInterface that contains the crypto object for loading
    /// @return unique smart pointer to the created object
    /// @uptrace{SWS_CRYPT_20733}
    virtual ara::core::Result<CryptoObject::Uptrc> LoadObject(const IOInterface& container) noexcept = 0;

    /// @brief Load a private key from the IOInterface provided.
    /// @param container the IOInterface that contains the crypto object for loading
    /// @return unique smart pointer to the PrivateKey
    /// @uptrace{SWS_CRYPT_20764}
    virtual ara::core::Result<PrivateKey::Uptrc> LoadPrivateKey(const IOInterface& container) noexcept = 0;

    /// @brief Load a public key from the IOInterface provided.
    /// @param container the IOInterface that contains the crypto object for loading
    /// @return unique smart pointer to the PublicKey
    /// @uptrace{SWS_CRYPT_20763}
    virtual ara::core::Result<PublicKey::Uptrc> LoadPublicKey(const IOInterface& container) noexcept = 0;

    /// @brief Load secret seed from the IOInterface provided.
    /// @param container the IOInterface that contains the crypto object for loading
    /// @return unique smart pointer to the SecretSeed
    /// @uptrace{SWS_CRYPT_20765}
    virtual ara::core::Result<SecretSeed::Uptrc> LoadSecretSeed(const IOInterface& container) noexcept = 0;

    /// @brief Load a symmetric key from the IOInterface provided.
    /// @param container the IOInterface that contains the crypto object for loading
    /// @return unique smart pointer to the SymmetricKey
    /// @uptrace{SWS_CRYPT_20762}
    virtual ara::core::Result<SymmetricKey::Uptrc> LoadSymmetricKey(const IOInterface& container) noexcept = 0;

    /// @brief Copy-assign another CryptoProvider to this instance.
    /// @param other the other instance
    /// @return *this, containing the contents of other
    /// @uptrace{SWS_CRYPT_30216}
    CryptoProvider& operator=(const CryptoProvider& other) = default;

    /// @brief Move-assign another CryptoProvider to this instance.
    /// @param other the other instance
    /// @return *this, containing the contents of other
    /// @uptrace{SWS_CRYPT_30217}
    CryptoProvider& operator=(CryptoProvider&& other) = default;
};

}  // namespace cryp
}  // namespace crypto
}  // namespace ara

#endif  // ARA_CRYPTO_CRYP_CRYPTO_PROVIDER_H