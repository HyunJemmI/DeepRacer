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
/// GENERATED FILE NAME               : rawdata.cpp
/// SOFTWARE COMPONENT NAME           : RawData
/// GENERATED DATE                    : 2024-11-14 15:25:13
///////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "calc/aa/port/rawdata.h"
 
namespace calc
{
namespace aa
{
namespace port
{
 
RawData::RawData()
    : m_logger(ara::log::CreateLogger("CALC", "PORT", ara::log::LogLevel::kVerbose))
    , m_running{false}
    , m_found{false}
{
}
 
RawData::~RawData()
{
}
 
void RawData::Start()
{
    m_logger.LogVerbose() << "RawData::Start";
    
    // regist callback
    ara::core::InstanceSpecifier specifier{"Calc/AA/RawData"};
    auto handler = [this](ara::com::ServiceHandleContainer<deepracer::service::rawdata::proxy::SvRawDataProxy::HandleType> handles,
                          ara::com::FindServiceHandle findHandle) {
        this->Find(handles, findHandle);
    };
    
    // find service
    auto find = deepracer::service::rawdata::proxy::SvRawDataProxy::StartFindService(handler, specifier);
    if (find.HasValue())
    {
        m_logger.LogVerbose() << "RawData::Start::StartFindService";
    }
    else
    {
        m_logger.LogError() << "RawData::Start::StartFindService::" << find.Error().Message();
    }
    
    // run port
    m_running = true;
}
 
void RawData::Terminate()
{
    m_logger.LogVerbose() << "RawData::Terminate";
    
    // stop port
    m_running = false;
    
    // clear service proxy
    if (m_interface)
    {
        // stop subscribe
        StopSubscribeREvent();
        StopSubscribeRField();
        
        // stop find service
        m_interface->StopFindService(*m_findHandle);
        m_found = false;
        
        m_logger.LogVerbose() << "RawData::Terminate::StopFindService";
    }
}
 
void RawData::Find(ara::com::ServiceHandleContainer<deepracer::service::rawdata::proxy::SvRawDataProxy::HandleType> handles, ara::com::FindServiceHandle findHandle)
{
    // check finding handles
    if (handles.empty())
    {
        m_logger.LogVerbose() << "RawData::Find::Service Instances not found";
        return;
    }
    else
    {
        for (auto& handle : handles)
        {
            m_logger.LogVerbose() << "RawData::Find::Searched Instance::ServiceId =" << 
                                     handle.GetServiceHandle().serviceId << 
                                     ", InstanceId =" << 
                                     handle.GetServiceHandle().instanceId;
        }
    }
    
    // create proxy
    if (m_interface)
    {
        m_logger.LogVerbose() << "RawData::Find::Proxy is already running";
    }
    else
    {
        m_logger.LogVerbose() << "RawData::Find::Using Instance::ServiceId =" << 
                                 handles[0].GetServiceHandle().serviceId << 
                                 ", InstanceId =" << 
                                 handles[0].GetServiceHandle().instanceId;
        m_interface = std::make_shared<deepracer::service::rawdata::proxy::SvRawDataProxy>(handles[0]);
        m_findHandle = std::make_shared<ara::com::FindServiceHandle>(findHandle);
        m_found = true;
        
        // subscribe events
        SubscribeREvent();
        // subscribe field notifications
        SubscribeRField();
    }
}
 
void RawData::SubscribeREvent()
{
    if (m_found)
    {
        // regist receiver handler
        // if you want to enable it, please uncomment below code
        // 
        // RegistReceiverREvent();
        
        // request subscribe
        auto subscribe = m_interface->REvent.Subscribe(1);
        if (subscribe.HasValue())
        {
            m_logger.LogVerbose() << "RawData::SubscribeREvent::Subscribed";
        }
        else
        {
            m_logger.LogError() << "RawData::SubscribeREvent::" << subscribe.Error().Message();
        }
    }
}
 
void RawData::StopSubscribeREvent()
{
    if (m_found)
    {
        // request stop subscribe
        m_interface->REvent.Unsubscribe();
        m_logger.LogVerbose() << "RawData::StopSubscribeREvent::Unsubscribed";
    }
}
 
void RawData::RegistReceiverREvent()
{
    if (m_found)
    {
        // set callback
        auto receiver = [this]() -> void {
            return ReceiveEventREventTriggered();
        };
        
        // regist callback
        auto callback = m_interface->REvent.SetReceiveHandler(receiver);
        if (callback.HasValue())
        {
            m_logger.LogVerbose() << "RawData::RegistReceiverREvent::SetReceiveHandler";
        }
        else
        {
            m_logger.LogError() << "RawData::RegistReceiverREvent::SetReceiveHandler::" << callback.Error().Message();
        }
    }
}
 
void RawData::ReceiveEventREventTriggered()
{
    if (m_found)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (m_interface->REvent.GetSubscriptionState() == ara::com::SubscriptionState::kSubscribed)
        {
            auto recv = std::make_unique<ara::core::Result<size_t>>(m_interface->REvent.GetNewSamples([&](auto samplePtr) {
                RawData::ReadDataREvent(std::move(samplePtr));
            }));
            if (recv->HasValue())
            {
                m_logger.LogVerbose() << "RawData::ReceiveEventREvent::GetNewSamples::" << recv->Value();
            }
            else
            {
                m_logger.LogError() << "RawData::ReceiveEventREvent::GetNewSamples::" << recv->Error().Message();
            }
        }
    }
}
 
void RawData::ReceiveEventREventCyclic()
{
    while (m_running)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (m_found)
        {
            if (m_interface->REvent.GetSubscriptionState() == ara::com::SubscriptionState::kSubscribed)
            {
                auto recv = std::make_unique<ara::core::Result<size_t>>(m_interface->REvent.GetNewSamples([&](auto samplePtr) {
                    RawData::ReadDataREvent(std::move(samplePtr));
                }));
                if (recv->HasValue())
                {
                    m_logger.LogVerbose() << "RawData::ReceiveEventREvent::GetNewSamples::" << recv->Value();
                }
                else
                {
                    m_logger.LogError() << "RawData::ReceiveEventREvent::GetNewSamples::" << recv->Error().Message();
                }
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}
 
void RawData::ReadDataREvent(ara::com::SamplePtr<deepracer::service::rawdata::proxy::events::REvent::SampleType const> samplePtr)
{
    auto data = *samplePtr.Get();
    // put your logic
    m_logger.LogInfo() << "RawData::ReadDataREvent::data::" << data.size();

    // REvent 핸들러가 등록되어 있을시 해당 핸들러는 값과 함께 호출한다.
    if (m_receiveEventREventHandler != nullptr)
    {
        m_receiveEventREventHandler(data);
    }
}
 
void RawData::SubscribeRField()
{
    if (m_found)
    {
        // regist receiver handler
        // if you want to enable it, please uncomment below code
        // 
        // RegistReceiverRField();
        
        // request subscribe
        auto subscribe = m_interface->RField.Subscribe(1);
        if (subscribe.HasValue())
        {
            m_logger.LogVerbose() << "RawData::SubscribeRField::Subscribed";
        }
        else
        {
            m_logger.LogError() << "RawData::SubscribeRField::" << subscribe.Error().Message();
        }
    }
}
 
void RawData::StopSubscribeRField()
{
    if (m_found)
    {
        // request stop subscribe
        m_interface->RField.Unsubscribe();
        m_logger.LogVerbose() << "RawData::StopSubscribeRField::Unsubscribed";
    }
}
 
void RawData::RegistReceiverRField()
{
    if (m_found)
    {
        // set callback
        auto receiver = [this]() -> void {
            return ReceiveFieldRFieldTriggered();
        };
        
        // regist callback
        auto callback = m_interface->RField.SetReceiveHandler(receiver);
        if (callback.HasValue())
        {
            m_logger.LogVerbose() << "RawData::RegistReceiverRField::SetReceiveHandler";
        }
        else
        {
            m_logger.LogError() << "RawData::RegistReceiverRField::SetReceiveHandler::" << callback.Error().Message();
        }
    }
}
 
void RawData::ReceiveFieldRFieldTriggered()
{
    if (m_found)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (m_interface->RField.GetSubscriptionState() == ara::com::SubscriptionState::kSubscribed)
        {
            auto recv = std::make_unique<ara::core::Result<size_t>>(m_interface->RField.GetNewSamples([&](auto samplePtr) {
                RawData::ReadValueRField(std::move(samplePtr));
            }));
            if (recv->HasValue())
            {
                m_logger.LogVerbose() << "RawData::ReceiveFieldRField::GetNewSamples::" << recv->Value();
            }
            else
            {
                m_logger.LogError() << "RawData::ReceiveFieldRField::GetNewSamples::" << recv->Error().Message();
            }
        }
    }
}
 
void RawData::ReceiveFieldRFieldCyclic()
{
    while (m_running)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (m_found)
        {
            if (m_interface->RField.GetSubscriptionState() == ara::com::SubscriptionState::kSubscribed)
            {
                auto recv = std::make_unique<ara::core::Result<size_t>>(m_interface->RField.GetNewSamples([&](auto samplePtr) {
                    RawData::ReadValueRField(std::move(samplePtr));
                }));
                if (recv->HasValue())
                {
                    m_logger.LogVerbose() << "RawData::ReceiveFieldRField::GetNewSamples::" << recv->Value();
                }
                else
                {
                    m_logger.LogError() << "RawData::ReceiveFieldRField::GetNewSamples::" << recv->Error().Message();
                }
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}
 
void RawData::ReadValueRField(ara::com::SamplePtr<deepracer::service::rawdata::proxy::fields::RField::FieldType const> samplePtr)
{
    auto value = *samplePtr.Get();
    // put your logic
}
 
void RawData::GetRField()
{
    if (m_found)
    {
        auto request = m_interface->RField.Get();
        request.wait();
        auto response = request.GetResult();
        if (response.HasValue())
        {
            m_logger.LogVerbose() << "RawData::GetRField::Responded";
            
            auto result = response.Value();
            // put your logic
        }
        else
        {
            m_logger.LogError() << "RawData::GetRField::" << response.Error().Message();
        }
    }
}
 
void RawData::SetRField(const deepracer::service::rawdata::proxy::fields::RField::FieldType& value)
{
    if (m_found)
    {
        auto request = m_interface->RField.Set(value);
        request.wait();
        auto response = request.GetResult();
        if (response.HasValue())
        {
            m_logger.LogVerbose() << "RawData::SetRField::Responded";
            
            auto result = response.Value();
            // put your logic
        }
        else
        {
            m_logger.LogError() << "RawData::GetRField::" << response.Error().Message();
        }
    }
}
 
void RawData::RequestRMethod(const double& a, const deepracer::type::Arithmetic& artihmetic, const double& b)
{
    if (m_found)
    {
        auto request = m_interface->RMethod(a, artihmetic, b);
        request.wait();
        auto response = request.GetResult();
        if (response.HasValue())
        {
            m_logger.LogVerbose() << "RawData::RequestRMethod::Responded";
            
            auto result = response.Value();
            // put your logic
        }
        else
        {
            m_logger.LogError() << "RawData::RequestRMethod::" << response.Error().Message();
        }
    }
}

// 개발자 추가 함수
// REvent 수신에 대한 핸들러 등록 함수.
void RawData::SetReceiveEventREventHandler(std::function<void(const deepracer::service::rawdata::proxy::events::REvent::SampleType &)> handler)
{
    m_receiveEventREventHandler = handler;
}
 
} /// namespace port
} /// namespace aa
} /// namespace calc
 
/// EOF