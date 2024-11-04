// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef ARA_CRYPTO_X509_X509_PROVIDER_H
#define ARA_CRYPTO_X509_X509_PROVIDER_H

#include "ara/core/instance_specifier.h"
#include "ara/core/optional.h"
#include "ara/core/result.h"
#include "ara/core/string_view.h"
#include "ara/crypto/common/mem_region.h"
#include "ara/crypto/common/serializable.h"
#include "ara/crypto/cryp/signer_private_ctx.h"
#include "ara/crypto/x509/cert_sign_request.h"
#include "ara/crypto/x509/certificate.h"
#include "ara/crypto/x509/ocsp_request.h"
#include "ara/crypto/x509/ocsp_response.h"
#include "ara/crypto/x509/x509_dn.h"
#include "ara/crypto/x509/x509_extensions.h"

namespace ara
{
namespace crypto
{
namespace x509
{

/// @brief X.509 Provider interface. The X.509 Provider supports two internal storages: volatile (or session) and
/// persistent.
///        All X.509 objects created by the provider should have an actual reference to their parent X.509 Provider.
///        The X.509 Provider can be destroyed only after destroying of all its daughterly objects.
///        Each method of this interface that creates a X.509 object is non-constant,
///        because any such creation increases a references counter of the X.509 Provider.
/// @uptrace{SWS_CRYPT_40600}
class X509Provider
{
  public:
    /// @brief Shared smart pointer of the interface.
    /// @uptrace{SWS_CRYPT_40601}
    using Uptr = std::unique_ptr<X509Provider>;

    /// @brief Type of an internal index inside the certificate storage.
    /// @uptrace{SWS_CRYPT_40602}
    using StorageIndex = std::size_t;

    /// @brief Destructor.
    /// @uptrace{SWS_CRYPT_40604}
    virtual ~X509Provider() noexcept = default;

    /// @brief Create completed X.500 Distinguished Name structure from the provided string representation.
    /// @param dn string representation of the Distinguished Name
    /// @return unique smart pointer for the created X509DN object
    /// @uptrace{SWS_CRYPT_40612}
    virtual ara::core::Result<X509DN::Uptrc> BuildDn(ara::core::StringView dn) noexcept = 0;

    /// @brief Check certificate status by directly provided OCSP response.
    ///        This method may be used for implementation of the "OCSP stapling".
    ///        This method updates the Certificate::Status associated with the certificate.
    /// @param cert a certificate that should be verified
    /// @param ocspResponse an OCSP response
    /// @return true if the certificate is verified successfully and false otherwise
    /// @uptrace{SWS_CRYPT_40629}
    virtual ara::core::Result<bool> CheckCertStatus(Certificate& cert,
                                                    const OcspResponse& ocspResponse) const noexcept = 0;

    /// @brief Check status of a certificates list by directly provided OCSP response.
    ///        This method may be used for implementation of the "OCSP stapling".
    ///        This method updates the Certificate::Status associated with the certificates in the list.
    /// @param certList a certificates list that should be verified
    /// @param ocspResponse an OCSP response
    /// @return true if the certificates list is verified successfully and false otherwise
    /// @uptrace{SWS_CRYPT_40630}
    virtual ara::core::Result<bool> CheckCertStatus(const ara::core::Vector<Certificate*>& certList,
                                                    const OcspResponse& ocspResponse) const noexcept = 0;

    /// @brief Cleanup the volatile certificates storage.
    ///        After execution of this command the certificates previously imported to the volatile storage cannot be
    ///        found by a search, but it doesn’t influence to already loaded Certificate instances!
    /// @uptrace{SWS_CRYPT_40635}
    virtual void CleanupVolatileStorage() noexcept = 0;

    /// @brief Create certification request for a private key loaded to the context.
    /// @param signerCtx the fully-configured SignerPrivateCtx to be used for signing this certificate request
    /// @param derSubjectDN the DER-encoded subject distinguished name (DN) of the private key owner
    /// @param x509Extensions the DER-encoded X.509 Extensions that should be included to the certification request
    /// @param version the format version of the target certification request
    /// @return unique smart pointer to created certification request
    /// @uptrace{SWS_CRYPT_40640}
    virtual ara::core::Result<CertSignRequest::Uptrc> CreateCertSignRequest(
        cryp::SignerPrivateCtx::Uptr signerCtx, ReadOnlyMemRegion derSubjectDN,
        ReadOnlyMemRegion x509Extensions = ReadOnlyMemRegion(), unsigned version = 1) const noexcept = 0;

    /// @brief Count number of certificates in a serialized certificate chain represented by a single BLOB.
    /// @param certChain DER/PEM-encoded certificate chain (in form of a single BLOB)
    /// @param formatId input format identifier (kFormatDefault means auto-detect)
    /// @return number of certificates in the chain
    /// @uptrace{SWS_CRYPT_40615}
    virtual ara::core::Result<std::size_t> CountCertsInChain(
        ReadOnlyMemRegion certChain, Serializable::FormatId formatId = Serializable::kFormatDefault) const noexcept = 0;

    /// @brief Create an empty X.500 Distinguished Name (DN) structure.
    ///        If (0 == capacity) then a maximally supported (by the implementation) capacity must be reserved.
    /// @param capacity number of bytes that should be reserved for the content of the target X509DN object
    /// @return Unique smart pointer to created empty X509DN object
    /// @uptrace{SWS_CRYPT_40611}
    virtual ara::core::Result<X509DN::Uptr> CreateEmptyDn(std::size_t capacity = 0) noexcept = 0;

    /// @brief Create an empty X.509 Extensions structure. If (0 == capacity) then a maximally supported (by the
    /// implementation) capacity must be reserved.
    /// @param capacity number of bytes that should be reserved for the content of the target X509Extensions object
    /// @return Shared smart pointer to created empty X509X509Extensions object
    /// @uptrace{SWS_CRYPT_40636}
    virtual ara::core::Result<X509Extensions::Uptr> CreateEmptyExtensions(std::size_t capacity = 0) noexcept = 0;

    /// @brief Create OCSP request for specified certificate. This method may be used for implementation of the "OCSP
    /// stapling".
    /// @param cert a certificate that should be verified
    /// @param signer an optional pointer to initialized signer context (if the request should be signed)
    /// @return unique smart pointer to the created OCSP request
    /// @uptrace{SWS_CRYPT_40626}
    virtual ara::core::Result<OcspRequest::Uptrc> CreateOcspRequest(
        const Certificate& cert, ara::core::Optional<const cryp::SignerPrivateCtx> signer) noexcept = 0;

    /// @brief Create OCSP request for specified list of certificates. This method may be used for implementation of the
    /// "OCSP stapling".
    /// @param certList a certificates’ list that should be verified
    /// @param signer an optional pointer to initialized signer context (if the request should be signed)
    /// @return unique smart pointer to the created OCSP request
    /// @uptrace{SWS_CRYPT_40627}
    virtual ara::core::Result<OcspRequest::Uptrc> CreateOcspRequest(
        const ara::core::Vector<const Certificate*>& certList,
        ara::core::Optional<const cryp::SignerPrivateCtx> signer) noexcept = 0;

    /// @brief Decode X.500 Distinguished Name structure from the provided serialized format.
    /// @param dn DER/PEM-encoded representation of the Distinguished Name
    /// @param formatId input format identifier (kFormatDefault means auto-detect)
    /// @return unique smart pointer for the created X509DN object
    /// @uptrace{SWS_CRYPT_40613}
    virtual ara::core::Result<X509DN::Uptrc> DecodeDn(
        ReadOnlyMemRegion dn, Serializable::FormatId formatId = Serializable::kFormatDefault) noexcept = 0;

    /// @brief Find a certificate by the subject and issuer Distinguished Names (DN).
    ///        Argument certIndex represents an internal index of current certificate in the storage.
    ///        In order to start certificate search from begin, set: certIndex = kInvalidIndex
    /// @param subjectDn subject DN of the target certificate
    /// @param issuerDn issuer DN of the target certificate
    /// @param validityTimePoint a time point when the target certificate should be valid
    /// @param certIndex an index for iteration through all suitable certificates in the storage
    ///        (input: index of previous found cerificate, output: index of current found cerificate)
    /// @return unique smart pointer to found certificate or nullptr if nothing is found
    /// @uptrace{SWS_CRYPT_40631}
    virtual Certificate::Uptrc FindCertByDn(const X509DN& subjectDn, const X509DN& issuerDn, time_t validityTimePoint,
                                            StorageIndex& certIndex) noexcept = 0;

    /// @brief Find a certificate by its SKID & AKID.
    /// @param subjectKeyId subject key identifier (SKID)
    /// @param authorityKeyId optional authority key identifier (AKID)
    /// @return unique smart pointer to found certificate or nullptr if nothing is found
    /// @uptrace{SWS_CRYPT_40632}
    virtual Certificate::Uptrc FindCertByKeyIds(ReadOnlyMemRegion subjectKeyId,
                                                ReadOnlyMemRegion authorityKeyId = ReadOnlyMemRegion()) noexcept = 0;

    /// @brief Find a certificate by its serial number.
    /// @param sn serial number of the target certificate
    /// @param issuerDn authority’s Distinguished Names (DN)
    /// @return unique smart pointer to a found certificate or nullptr if nothing is found
    /// @uptrace{SWS_CRYPT_40633}
    virtual Certificate::Uptrc FindCertBySn(ReadOnlyMemRegion sn, const X509DN& issuerDn) noexcept = 0;

    /// @brief Parse a certificate signing request (CSR) provided by the user.
    /// @param csr the buffer containing a certificate signing request
    /// @param withMetaData specifies the format of the buffer content:
    ///        TRUE means the object has been previously serialized by using the Serializable interface;
    ///        FALSE means the CSR was exported using the CertSignRequest::ExportASN1CertSignRequest() interface
    /// @return unique smart pointer to the certificate signing request
    /// @uptrace{SWS_CRYPT_40634}
    virtual ara::core::Result<CertSignRequest::Uptrc> ParseCertSignRequest(ReadOnlyMemRegion csr,
                                                                           bool withMetaData = true) noexcept = 0;

    /// @brief Import Certificate Revocation List (CRL) or Delta CRL from a memory BLOB.
    ///        If the imported CRL lists some certificates kept in the persistent or volatile storages then their status
    ///        must be automatically updated to Status::kInvalid. If some of these certificates were already openned
    ///        during this operation, then this status change becomes available immediately (via method call
    ///        Certificate::GetStatus())! All certificates with the status kInvalid should be automatically removed from
    ///        correspondent storages (immediately if a certificate not in use now or just after its closing otherwise).
    /// @param crl serialized CRL or Delta CRL (in form of a BLOB)
    /// @return true if the CRL is valid and false if it is already expired
    /// @uptrace{SWS_CRYPT_40620}
    virtual ara::core::Result<bool> ImportCrl(ReadOnlyMemRegion crl) noexcept = 0;

    /// @brief Import the certificate to volatile or persistent storage. Only imported certificate may be found by a
    /// search and applied for automatic verifications!
    ///        A certificate can be imported to only one of storage: volatile or persistent.
    ///        Therefore if you import a certificate already kept in the persistent storage to the volatile one then
    ///        nothing changes. But if you import a certificate already kept in the volatile storage to the persistent
    ///        one then it is "moved" to the persistent realm. If an application successfully imports a certificate that
    ///        correspond to a CSR existing in the storage then this CSR should be removed.
    /// @param cert a valid certificate that should be imported
    /// @param iSpecify optionally a valid InstanceSpecifier can be provided that points to a CertificateSlot for
    /// persistent storage of the certificate,
    ///        otherwise the certificate shall be stored in volatile (session) storage
    /// @return --
    /// @uptrace{SWS_CRYPT_40621}
    virtual ara::core::Result<void> Import(const Certificate& cert,
                                           ara::core::Optional<ara::core::InstanceSpecifier> iSpecify) noexcept = 0;

    /// @brief Load a certificate from the persistent certificate storage.
    /// @param iSpecify the target certificate instance specifier
    /// @return an unique smart pointer to the instantiated certificate
    /// @uptrace{SWS_CRYPT_40641}
    virtual ara::core::Result<Certificate::Uptr> LoadCertificate(ara::core::InstanceSpecifier& iSpecify) noexcept = 0;

    /// @brief Parse a serialized representation of the certificate chain and create their instances.
    ///        Off-line validation of the parsed certification chain may be done via call VerifyCertChainByCrl().
    ///        After validation the certificates may be saved to the session or persistent storage for following search
    ///        and usage. If the certificates are not imported then they will be lost after destroy of the returned
    ///        instances! Only imported certificates may be found by a search and applied for automatic verifications!
    ///        Certificates in the outcome vector will be placed from the root CA certificate (zero index) to the final
    ///        end-entity certificate (last used index of the vector).
    /// @param outcome an output vector for imported certificates
    /// @param certChain DER/PEM-encoded certificate chain (in form of a single BLOB)
    /// @param formatId input format identifier (kFormatDefault means auto-detect)
    /// @return --
    /// @uptrace{SWS_CRYPT_40616}
    virtual ara::core::Result<void> ParseCertChain(
        ara::core::Vector<Certificate::Uptr>& outcome, ReadOnlyMemRegion certChain,
        Serializable::FormatId formatId = Serializable::kFormatDefault) noexcept = 0;

    /// @brief Parse a serialized representation of the certificate chain and create their instances.
    ///        Off-line validation of the parsed certification chain may be done via call VerifyCertChainByCrl().
    ///        After validation the certificates may be imported to the session or persistent storage for following
    ///        search and usage. Capacity of the outcome vector must be equal to the size of the certChain vector. If
    ///        the certificates are not imported then they will be lost after destroy of the returned instances! Only
    ///        imported certificates may be found by a search and applied for automatic verifications! Certificates in
    ///        the outcome vector will be placed from the root CA certificate (zero index) to the final end-entity
    ///        certificate (last used index of the vector).
    /// @param outcome output vector of imported certificates
    /// @param certChain DER/PEM-encoded certificates chain (each certificate is presented by a separate BLOB in the
    /// input vector)
    /// @param formatId input format identifier (kFormatDefault means auto-detect)
    /// @return --
    /// @uptrace{SWS_CRYPT_40617}
    virtual ara::core::Result<void> ParseCertChain(
        ara::core::Vector<Certificate::Uptr>& outcome, const ara::core::Vector<ReadOnlyMemRegion>& certChain,
        Serializable::FormatId formatId = Serializable::kFormatDefault) noexcept = 0;

    /// @brief Parse a serialized representation of the certificate and create its instance.
    ///        Off-line validation of the parsed certificate may be done via call VerifyCertByCrl().
    ///        After validation the certificate may be imported to the session or persistent storage for following
    ///        search and usage. If the parsed certificate is not imported then it will be lost after destroy of the
    ///        returned instance! Only imported certificate may be found by a search and applied for automatic
    ///        verifications!
    /// @param cert DER/PEM-encoded certificate
    /// @param formatId input format identifier (kFormatDefault means auto-detect)
    /// @return unique smart pointer to created certificate
    /// @uptrace{SWS_CRYPT_40614}
    virtual ara::core::Result<Certificate::Uptr> ParseCert(
        ReadOnlyMemRegion cert, Serializable::FormatId formatId = Serializable::kFormatDefault) noexcept = 0;

    /// @brief Parse serialized OCSP response and create correspondent interface instance.
    ///        This method may be used for implementation of the "OCSP stapling".
    /// @param response a serialized OCSP response
    /// @return unique smart pointer to the created OCSP response instance
    /// @uptrace{SWS_CRYPT_40628}
    virtual OcspResponse::Uptrc ParseOcspResponse(ReadOnlyMemRegion response) const noexcept = 0;

    /// @brief Remove specified certificate from the storage (volatile or persistent) and destroy it.
    /// @param cert a unique smart pointer to a certificate that should be removed
    /// @return true if the certificate was found and removed from the storage, false if it was not found
    /// @uptrace{SWS_CRYPT_40622}
    virtual bool Remove(Certificate::Uptrc&& cert) noexcept = 0;

    /// @brief Set specified CA certificate as a "root of trust".
    ///        Only a certificate saved to the volatile or persistent storage may be marked as the "root of trust"!
    ///        Only CA certificate can be a "root of trust"! Multiple certificates on an ECU may be marked as the "root
    ///        of trust". Only an application with permissions "Trust Master" has the right to call this method!
    /// @param caCert a valid CA certificate that should be trusted
    /// @return --
    /// @uptrace{SWS_CRYPT_40625}
    virtual ara::core::Result<void> SetAsRootOfTrust(const Certificate& caCert) noexcept = 0;

    /// @brief Set the "pending" status associated to the CSR that means that the CSR already sent to CA.
    ///        This method do nothing if the CSR already marked as "pending".
    ///        Only an application with permissions "CA Connector" has the right to call this method!
    /// @param request certificate signing request that should be marked as "pending"
    /// @return --
    /// @uptrace{SWS_CRYPT_40624}
    virtual ara::core::Result<void> SetPendingStatus(const CertSignRequest& request) noexcept = 0;

    /// @brief Verify status of the provided certificate by locally stored CA certificates and CRLs only.
    ///        This method updates the Certificate::Status associated with the certificate.
    /// @param cert target certificate for verification
    /// @param myRoot root certificate to be used for verification - if this is nullptr, use machine root certificates
    /// @return verification status of the provided certificate
    /// @uptrace{SWS_CRYPT_40618}
    virtual Certificate::Status VerifyCert(Certificate& cert, Certificate::Uptr myRoot = nullptr) noexcept = 0;

    /// @brief Verify status of the provided certification chain by locally stored CA certificates and CRLs only.
    ///        Verification status of the certificate chain is Certificate::Status::kValid only if all certificates in
    ///        the chain have such status! Certificates in the chain (presented by container vector) must be placed from
    ///        the root CA certificate (zero index) to the target end-entity certificate (last used index of the
    ///        vector). Verification is executed in same order. If the chain verification is failed then status of the
    ///        first failed certificate is returned. This method updates the Certificate::Status associated with the
    ///        certificates in the chain.
    /// @param chain target certificate chain for verification
    /// @param myRoot root certificate to be used for verification - if this is nullptr, use machine root certificates
    /// @return verification status of the provided certificate chain
    /// @uptrace{SWS_CRYPT_40619}
    virtual Certificate::Status VerifyCertChain(ara::core::Span<const Certificate::Uptr> chain,
                                                Certificate::Uptr myRoot = nullptr) const noexcept = 0;

    /// @brief Copy-assign another X509Provider to this instance.
    /// @param other the other instance
    /// @return *this, containing the contents of other
    /// @uptrace{SWS_CRYPT_30226}
    X509Provider& operator=(const X509Provider& other) = default;

    /// @brief Move-assign another X509Provider to this instance.
    /// @param other the other instance
    /// @return *this, containing the contents of other
    /// @uptrace{SWS_CRYPT_30227}
    X509Provider& operator=(X509Provider&& other) = default;

    /// @brief Reserved "invalid index" value for navigation inside the certificate storage.
    /// @uptrace{SWS_CRYPT_40603}
    const StorageIndex kInvalidIndex = static_cast<std::size_t>(-1LL);
};

}  // namespace x509
}  // namespace crypto
}  // namespace ara

#endif  // ARA_CRYPTO_X509_X509_PROVIDER_H
