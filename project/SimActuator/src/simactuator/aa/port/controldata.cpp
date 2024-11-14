///////////////////////////////////////////////////////////////////////////////////////////////////////////
///                                                                                                        
/// Copyright, 2021 PopcornSAR Co., Ltd. All rights reserved.                                              
/// This software is copyright protected and proprietary to PopcornSAR Co., Ltd.                           
/// PopcornSAR Co., Ltd. grants to you only those rights as set out in the license conditions.             
///                                                                                                        
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/// AUTOSAR VERSION                   : R20-11
/// GENERATED BY                      : PARA, Adaptive Application Generator
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/// GENERATED FILE NAME               : controldata.cpp
/// SOFTWARE COMPONENT NAME           : ControlData
/// GENERATED DATE                    : 2024-11-14 15:25:13
///////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "simactuator/aa/port/controldata.h"
 
namespace simactuator
{
namespace aa
{
namespace port
{
 
ControlData::ControlData()
    : m_logger(ara::log::CreateLogger("SACT", "PORT", ara::log::LogLevel::kVerbose))
    , m_running{false}
    , m_found{false}
{
}
 
ControlData::~ControlData()
{
}
 
void ControlData::Start()
{
    m_logger.LogVerbose() << "ControlData::Start";
    
    // regist callback
    ara::core::InstanceSpecifier specifier{"SimActuator/AA/ControlData"};
    auto handler = [this](ara::com::ServiceHandleContainer<deepracer::service::controldata::proxy::SvControlDataProxy::HandleType> handles,
                          ara::com::FindServiceHandle findHandle) {
        this->Find(handles, findHandle);
    };
    
    // find service
    auto find = deepracer::service::controldata::proxy::SvControlDataProxy::StartFindService(handler, specifier);
    if (find.HasValue())
    {
        m_logger.LogVerbose() << "ControlData::Start::StartFindService";
    }
    else
    {
        m_logger.LogError() << "ControlData::Start::StartFindService::" << find.Error().Message();
    }
    
    // run port
    m_running = true;
}
 
void ControlData::Terminate()
{
    m_logger.LogVerbose() << "ControlData::Terminate";
    
    // stop port
    m_running = false;
    
    // clear service proxy
    if (m_interface)
    {
        // stop subscribe
        StopSubscribeCEvent();
        
        // stop find service
        m_interface->StopFindService(*m_findHandle);
        m_found = false;
        
        m_logger.LogVerbose() << "ControlData::Terminate::StopFindService";
    }
}
 
void ControlData::Find(ara::com::ServiceHandleContainer<deepracer::service::controldata::proxy::SvControlDataProxy::HandleType> handles, ara::com::FindServiceHandle findHandle)
{
    // check finding handles
    if (handles.empty())
    {
        m_logger.LogVerbose() << "ControlData::Find::Service Instances not found";
        return;
    }
    else
    {
        for (auto& handle : handles)
        {
            m_logger.LogVerbose() << "ControlData::Find::Searched Instance::ServiceId =" << 
                                     handle.GetServiceHandle().serviceId << 
                                     ", InstanceId =" << 
                                     handle.GetServiceHandle().instanceId;
        }
    }
    
    // create proxy
    if (m_interface)
    {
        m_logger.LogVerbose() << "ControlData::Find::Proxy is already running";
    }
    else
    {
        m_logger.LogVerbose() << "ControlData::Find::Using Instance::ServiceId =" << 
                                 handles[0].GetServiceHandle().serviceId << 
                                 ", InstanceId =" << 
                                 handles[0].GetServiceHandle().instanceId;
        m_interface = std::make_shared<deepracer::service::controldata::proxy::SvControlDataProxy>(handles[0]);
        m_findHandle = std::make_shared<ara::com::FindServiceHandle>(findHandle);
        m_found = true;
        
        // subscribe events
        SubscribeCEvent();
    }
}
 
void ControlData::SubscribeCEvent()
{
    if (m_found)
    {
        // regist receiver handler
        // if you want to enable it, please uncomment below code
        // 
        // RegistReceiverCEvent();
        
        // request subscribe
        auto subscribe = m_interface->CEvent.Subscribe(1);
        if (subscribe.HasValue())
        {
            m_logger.LogVerbose() << "ControlData::SubscribeCEvent::Subscribed";
        }
        else
        {
            m_logger.LogError() << "ControlData::SubscribeCEvent::" << subscribe.Error().Message();
        }
    }
}
 
void ControlData::StopSubscribeCEvent()
{
    if (m_found)
    {
        // request stop subscribe
        m_interface->CEvent.Unsubscribe();
        m_logger.LogVerbose() << "ControlData::StopSubscribeCEvent::Unsubscribed";
    }
}
 
void ControlData::RegistReceiverCEvent()
{
    if (m_found)
    {
        // set callback
        auto receiver = [this]() -> void {
            return ReceiveEventCEventTriggered();
        };
        
        // regist callback
        auto callback = m_interface->CEvent.SetReceiveHandler(receiver);
        if (callback.HasValue())
        {
            m_logger.LogVerbose() << "ControlData::RegistReceiverCEvent::SetReceiveHandler";
        }
        else
        {
            m_logger.LogError() << "ControlData::RegistReceiverCEvent::SetReceiveHandler::" << callback.Error().Message();
        }
    }
}
 
void ControlData::ReceiveEventCEventTriggered()
{
    if (m_found)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (m_interface->CEvent.GetSubscriptionState() == ara::com::SubscriptionState::kSubscribed)
        {
            auto recv = std::make_unique<ara::core::Result<size_t>>(m_interface->CEvent.GetNewSamples([&](auto samplePtr) {
                ControlData::ReadDataCEvent(std::move(samplePtr));
            }));
            if (recv->HasValue())
            {
                m_logger.LogVerbose() << "ControlData::ReceiveEventCEvent::GetNewSamples::" << recv->Value();
            }
            else
            {
                m_logger.LogError() << "ControlData::ReceiveEventCEvent::GetNewSamples::" << recv->Error().Message();
            }
        }
    }
}
 
void ControlData::ReceiveEventCEventCyclic()
{
    while (m_running)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (m_found)
        {
            if (m_interface->CEvent.GetSubscriptionState() == ara::com::SubscriptionState::kSubscribed)
            {
                auto recv = std::make_unique<ara::core::Result<size_t>>(m_interface->CEvent.GetNewSamples([&](auto samplePtr) {
                    ControlData::ReadDataCEvent(std::move(samplePtr));
                }));
                if (recv->HasValue())
                {
                    m_logger.LogVerbose() << "ControlData::ReceiveEventCEvent::GetNewSamples::" << recv->Value();
                }
                else
                {
                    m_logger.LogError() << "ControlData::ReceiveEventCEvent::GetNewSamples::" << recv->Error().Message();
                }
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}
 
void ControlData::ReadDataCEvent(ara::com::SamplePtr<deepracer::service::controldata::proxy::events::CEvent::SampleType const> samplePtr)
{
    auto data = *samplePtr.Get();
    // put your logic
}
 
} /// namespace port
} /// namespace aa
} /// namespace simactuator
 
/// EOF