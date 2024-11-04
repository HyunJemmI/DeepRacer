// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef PARA_COM_SKELETON_SKELETON_INTERFACE_H
#define PARA_COM_SKELETON_SKELETON_INTERFACE_H

#include <cstdint>
#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "ara/com/e2e/e2e_error_domain.h"
#include "ara/com/e2e/types.h"
#include "ara/com/types.h"
#include "para/com/types.h"
#include "para/serialization/serializer.h"

namespace para
{
namespace com
{

using MethodCallHandler = std::function<void(const std::vector<std::uint8_t>& data, const MethodToken token)>;
using E2EErrorHandler = std::function<void(const ara::com::e2e::E2EErrorDomain& errorCode, ara::com::e2e::DataID dataID,
                                           ara::com::e2e::MessageCounter messageCounter)>;

class SkeletonInterface
{
  public:
    SkeletonInterface(const ara::core::InstanceSpecifier& instanceSpecifier,
                      ara::com::MethodCallProcessingMode callProcessingMode);
    virtual ~SkeletonInterface();

    ara::core::Result<void> OfferService();
    void StopOfferService();
    void SetMethodCallHandler(const std::string& callSign, MethodCallHandler handler);
    void SetE2EErrorHandler(E2EErrorHandler handler);
    void ReturnMethod(const std::string& callSign, std::uint8_t result, const std::vector<std::uint8_t>& data,
                      MethodToken token);
    ara::core::Result<bool> ProcessNextMethodCall();
    ara::core::Result<void> SendEvent(const std::string& callSign, const std::vector<std::uint8_t>& data);

  private:
    class Impl;
    std::unique_ptr<Impl> mImpl;
};

}  // namespace com
}  // namespace para

#endif  // PARA_COM_SKELETON_SKELETON_INTERFACE_H
