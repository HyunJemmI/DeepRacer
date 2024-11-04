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
/// GENERATED FILE NAME               : cameradata.cpp
/// SOFTWARE COMPONENT NAME           : CameraData
/// GENERATED DATE                    : 2024-11-04 08:32:44
///////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "sensor/aa/port/cameradata.h"
 
namespace deepracer
{
namespace service
{
namespace cameradata
{
namespace skeleton
{
 
SvCameraDataSkeletonImpl::SvCameraDataSkeletonImpl(ara::core::InstanceSpecifier instanceSpec, ara::com::MethodCallProcessingMode mode)
    : SvCameraDataSkeleton(instanceSpec, mode)
    , m_logger(ara::log::CreateLogger("SENS", "PORT", ara::log::LogLevel::kVerbose))
{
}
 
} /// namespace skeleton
} /// namespace cameradata
} /// namespace service
} /// namespace deepracer

namespace sensor
{
namespace aa
{
namespace port
{
 
CameraData::CameraData()
    : m_logger(ara::log::CreateLogger("SENS", "PORT", ara::log::LogLevel::kVerbose))
    , m_running{false}
    , m_CEventData{0U, 0U, 0U}
{
}
 
CameraData::~CameraData()
{
}
 
void CameraData::Start()
{
    m_logger.LogVerbose() << "CameraData::Start";
    
    // construct skeleton
    ara::core::InstanceSpecifier specifier{"Sensor/AA/CameraData"};
    m_interface = std::make_shared<deepracer::service::cameradata::skeleton::SvCameraDataSkeletonImpl>(specifier);
    
    // offer service
    auto offer = m_interface->OfferService();
    if (offer.HasValue())
    {
        m_running = true;
        m_logger.LogVerbose() << "CameraData::Start::OfferService";
    }
    else
    {
        m_running = false;
        m_logger.LogError() << "CameraData::Start::OfferService::" << offer.Error().Message();
    }
}
 
void CameraData::Terminate()
{
    m_logger.LogVerbose() << "CameraData::Terminate";
    
    // stop port
    m_running = false;
    
    // stop offer service
    m_interface->StopOfferService();
    m_logger.LogVerbose() << "CameraData::Terminate::StopOfferService";
}
 
void CameraData::WriteDataCEvent(const deepracer::service::cameradata::skeleton::events::CEvent::SampleType& data)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_CEventData = data;
}
 
void CameraData::SendEventCEventCyclic()
{
    while (m_running)
    {
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            auto send = m_interface->CEvent.Send(m_CEventData);
            if (send.HasValue())
            {
                m_logger.LogVerbose() << "CameraData::SendEventCEventCyclic::Send";
            }
            else
            {
                m_logger.LogError() << "CameraData::SendEventCEventCyclic::Send::" << send.Error().Message();
            }
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}
 
void CameraData::SendEventCEventTriggered()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    auto send = m_interface->CEvent.Send(m_CEventData);
    if (send.HasValue())
    {
        m_logger.LogVerbose() << "CameraData::SendEventCEventTriggered::Send";
    }
    else
    {
        m_logger.LogError() << "CameraData::SendEventCEventTriggered::Send::" << send.Error().Message();
    }
}
 
void CameraData::SendEventCEventTriggered(const deepracer::service::cameradata::skeleton::events::CEvent::SampleType& data)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_CEventData = data;
    auto send = m_interface->CEvent.Send(m_CEventData);
    if (send.HasValue())
    {
        m_logger.LogVerbose() << "CameraData::SendEventCEventTriggered::Send";
    }
    else
    {
        m_logger.LogError() << "CameraData::SendEventCEventTriggered::Send::" << send.Error().Message();
    }
}
 
} /// namespace port
} /// namespace aa
} /// namespace sensor
 
/// EOF