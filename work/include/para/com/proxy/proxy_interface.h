// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef PARA_COM_PROXY_PROXY_INTERFACE_H
#define PARA_COM_PROXY_PROXY_INTERFACE_H

#include <cstdint>
#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "ara/com/types.h"
#include "para/com/types.h"
#include "para/serialization/serializer.h"

namespace para
{
namespace com
{

using FindServiceHandler = std::function<void(std::vector<para::com::ServiceHandle>)>;
using MethodReturnHandler = std::function<void(std::uint8_t, const std::vector<std::uint8_t>&, void*)>;

class ProxyInterface
{
  public:
    ProxyInterface(const ara::core::InstanceSpecifier& instanceSpecifier, ServiceHandle targetService);
    virtual ~ProxyInterface();

    static ara::core::Result<std::vector<ServiceHandle>> FindService(
        const ara::core::InstanceSpecifier& instanceSpecifier);
    static ara::core::Result<ara::com::FindServiceHandle> StartFindService(
        const ara::core::InstanceSpecifier& instanceSpecifier, FindServiceHandler handler);
    static ara::com::FindServiceHandle GetFindServiceHandle(const ara::core::InstanceSpecifier& instanceSpecifier);
    void StopFindService();

    void SetMethodReturnHandler(const std::string& callSign, MethodReturnHandler handler);
    void CallMethod(const std::string& callSign, const std::vector<std::uint8_t>& data, void* promise);
    void CallMethodNoReturn(const std::string& callSign, const std::vector<std::uint8_t>& data);

    ara::core::Result<void> SetSubscriptionStateChangeHandler(const std::string& callSign,
                                                              ara::com::SubscriptionStateChangeHandler handler);
    void UnsetSubscriptionStateChangeHandler(const std::string& callSign);
    ara::core::Result<void> SetEventReceiveHandler(const std::string& callSign, ara::com::EventReceiveHandler handler);
    ara::core::Result<void> UnsetEventReceiveHandler(const std::string& callSign);
    ara::com::SubscriptionState GetSubscriptionState(const std::string& callSign);
    ara::core::Result<void> SubscribeEvent(const std::string& callSign, size_t maxSampleCount);
    void UnsubscribeEvent(const std::string& callSign);
    std::vector<std::vector<std::uint8_t>> GetNewSamples(const std::string& callSign, size_t maxNumberOfSamples);
    std::size_t GetFreeSampleCount(const std::string& callSign);
    ara::com::e2e::SMState GetE2EStateMachineState(const std::string& callSign);
    ara::com::e2e::ProfileCheckStatus GetProfileCheckStatus(const std::string& callSign);

  private:
    class Impl;
    std::unique_ptr<Impl> mImpl;
};

}  // namespace com
}  // namespace para

#endif  // PARA_COM_PROXY_PROXY_INTERFACE_H
