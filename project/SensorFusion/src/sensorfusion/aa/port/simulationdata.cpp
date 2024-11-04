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
/// GENERATED FILE NAME               : simulationdata.cpp
/// SOFTWARE COMPONENT NAME           : SimulationData
/// GENERATED DATE                    : 2024-11-04 08:32:44
///////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "sensorfusion/aa/port/simulationdata.h"
 
namespace sensorfusion
{
namespace aa
{
namespace port
{
 
SimulationData::SimulationData()
    : m_logger(ara::log::CreateLogger("SENF", "PORT", ara::log::LogLevel::kVerbose))
    , m_running{false}
    , m_found{false}
{
}
 
SimulationData::~SimulationData()
{
}
 
void SimulationData::Start()
{
    m_logger.LogVerbose() << "SimulationData::Start";
    
    // regist callback
    ara::core::InstanceSpecifier specifier{"SensorFusion/AA/SimulationData"};
    auto handler = [this](ara::com::ServiceHandleContainer<deepracer::service::simdata::proxy::SvSimulationDataProxy::HandleType> handles,
                          ara::com::FindServiceHandle findHandle) {
        this->Find(handles, findHandle);
    };
    
    // find service
    auto find = deepracer::service::simdata::proxy::SvSimulationDataProxy::StartFindService(handler, specifier);
    if (find.HasValue())
    {
        m_logger.LogVerbose() << "SimulationData::Start::StartFindService";
    }
    else
    {
        m_logger.LogError() << "SimulationData::Start::StartFindService::" << find.Error().Message();
    }
    
    // run port
    m_running = true;
}
 
void SimulationData::Terminate()
{
    m_logger.LogVerbose() << "SimulationData::Terminate";
    
    // stop port
    m_running = false;
    
    // clear service proxy
    if (m_interface)
    {
        // stop subscribe
        StopSubscribeSEvent();
        
        // stop find service
        m_interface->StopFindService(*m_findHandle);
        m_found = false;
        
        m_logger.LogVerbose() << "SimulationData::Terminate::StopFindService";
    }
}
 
void SimulationData::Find(ara::com::ServiceHandleContainer<deepracer::service::simdata::proxy::SvSimulationDataProxy::HandleType> handles, ara::com::FindServiceHandle findHandle)
{
    // check finding handles
    if (handles.empty())
    {
        m_logger.LogVerbose() << "SimulationData::Find::Service Instances not found";
        return;
    }
    else
    {
        for (auto& handle : handles)
        {
            m_logger.LogVerbose() << "SimulationData::Find::Searched Instance::ServiceId =" << 
                                     handle.GetServiceHandle().serviceId << 
                                     ", InstanceId =" << 
                                     handle.GetServiceHandle().instanceId;
        }
    }
    
    // create proxy
    if (m_interface)
    {
        m_logger.LogVerbose() << "SimulationData::Find::Proxy is already running";
    }
    else
    {
        m_logger.LogVerbose() << "SimulationData::Find::Using Instance::ServiceId =" << 
                                 handles[0].GetServiceHandle().serviceId << 
                                 ", InstanceId =" << 
                                 handles[0].GetServiceHandle().instanceId;
        m_interface = std::make_shared<deepracer::service::simdata::proxy::SvSimulationDataProxy>(handles[0]);
        m_findHandle = std::make_shared<ara::com::FindServiceHandle>(findHandle);
        m_found = true;
        
        // subscribe events
        SubscribeSEvent();
    }
}
 
void SimulationData::SubscribeSEvent()
{
    if (m_found)
    {
        // regist receiver handler
        // if you want to enable it, please uncomment below code
        // 
        // RegistReceiverSEvent();
        
        // request subscribe
        auto subscribe = m_interface->SEvent.Subscribe(1);
        if (subscribe.HasValue())
        {
            m_logger.LogVerbose() << "SimulationData::SubscribeSEvent::Subscribed";
        }
        else
        {
            m_logger.LogError() << "SimulationData::SubscribeSEvent::" << subscribe.Error().Message();
        }
    }
}
 
void SimulationData::StopSubscribeSEvent()
{
    if (m_found)
    {
        // request stop subscribe
        m_interface->SEvent.Unsubscribe();
        m_logger.LogVerbose() << "SimulationData::StopSubscribeSEvent::Unsubscribed";
    }
}
 
void SimulationData::RegistReceiverSEvent()
{
    if (m_found)
    {
        // set callback
        auto receiver = [this]() -> void {
            return ReceiveEventSEventTriggered();
        };
        
        // regist callback
        auto callback = m_interface->SEvent.SetReceiveHandler(receiver);
        if (callback.HasValue())
        {
            m_logger.LogVerbose() << "SimulationData::RegistReceiverSEvent::SetReceiveHandler";
        }
        else
        {
            m_logger.LogError() << "SimulationData::RegistReceiverSEvent::SetReceiveHandler::" << callback.Error().Message();
        }
    }
}
 
void SimulationData::ReceiveEventSEventTriggered()
{
    if (m_found)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (m_interface->SEvent.GetSubscriptionState() == ara::com::SubscriptionState::kSubscribed)
        {
            auto recv = std::make_unique<ara::core::Result<size_t>>(m_interface->SEvent.GetNewSamples([&](auto samplePtr) {
                SimulationData::ReadDataSEvent(std::move(samplePtr));
            }));
            if (recv->HasValue())
            {
                m_logger.LogVerbose() << "SimulationData::ReceiveEventSEvent::GetNewSamples::" << recv->Value();
            }
            else
            {
                m_logger.LogError() << "SimulationData::ReceiveEventSEvent::GetNewSamples::" << recv->Error().Message();
            }
        }
    }
}
 
void SimulationData::ReceiveEventSEventCyclic()
{
    while (m_running)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (m_found)
        {
            if (m_interface->SEvent.GetSubscriptionState() == ara::com::SubscriptionState::kSubscribed)
            {
                auto recv = std::make_unique<ara::core::Result<size_t>>(m_interface->SEvent.GetNewSamples([&](auto samplePtr) {
                    SimulationData::ReadDataSEvent(std::move(samplePtr));
                }));
                if (recv->HasValue())
                {
                    m_logger.LogVerbose() << "SimulationData::ReceiveEventSEvent::GetNewSamples::" << recv->Value();
                }
                else
                {
                    m_logger.LogError() << "SimulationData::ReceiveEventSEvent::GetNewSamples::" << recv->Error().Message();
                }
            }
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}
 
void SimulationData::ReadDataSEvent(ara::com::SamplePtr<deepracer::service::simdata::proxy::events::SEvent::SampleType const> samplePtr)
{
    auto data = *samplePtr.Get();
    // put your logic
}
 
} /// namespace port
} /// namespace aa
} /// namespace sensorfusion
 
/// EOF