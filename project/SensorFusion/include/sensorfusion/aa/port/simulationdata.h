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
/// GENERATED FILE NAME               : simulationdata.h
/// SOFTWARE COMPONENT NAME           : SimulationData
/// GENERATED DATE                    : 2024-11-04 08:32:44
///////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef PARA_AA_GEN_SOFTWARE_COMPONENT_RPORT_SENSORFUSION_AA_SIMULATIONDATA_H
#define PARA_AA_GEN_SOFTWARE_COMPONENT_RPORT_SENSORFUSION_AA_SIMULATIONDATA_H
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/// INCLUSION HEADER FILES
///////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "deepracer/service/simdata/svsimulationdata_proxy.h"
 
#include "ara/log/logger.h"
 
#include <mutex>
#include <thread>
 
namespace sensorfusion
{
namespace aa
{
namespace port
{
 
class SimulationData
{
public:
    /// @brief Constructor
    SimulationData();
    
    /// @brief Destructor
    ~SimulationData();
    
    /// @brief Start port
    void Start();
    
    /// @brief Terminate port
    void Terminate();
    
    /// @brief Subscribe event, SEvent
    void SubscribeSEvent();
     
    /// @brief Stop event subscription, SEvent
    void StopSubscribeSEvent();
     
    /// @brief Event receive handler, SEvent
    void ReceiveEventSEventTriggered();
     
    /// @brief Event receive handler, SEvent
    void ReceiveEventSEventCyclic();
     
    /// @brief Read event data, SEvent
    void ReadDataSEvent(ara::com::SamplePtr<deepracer::service::simdata::proxy::events::SEvent::SampleType const> samplePtr);
    
    
    
private:
    /// @brief Callback for find service
    void Find(ara::com::ServiceHandleContainer<deepracer::service::simdata::proxy::SvSimulationDataProxy::HandleType> handles,
              ara::com::FindServiceHandle findHandle);
    
    /// @brief Callback for event receiver, SEvent
    void RegistReceiverSEvent();
    
    
private:
    /// @brief Logger for this port
    ara::log::Logger& m_logger;
    
    /// @brief Flag of port status
    bool m_running;
    
    /// @brief Flag of find service status
    bool m_found;
    
    /// @brief Mutex for this port
    std::mutex m_mutex; 
    
    /// @brief AUTOSAR Port Interface
    std::shared_ptr<deepracer::service::simdata::proxy::SvSimulationDataProxy> m_interface;
    
    /// @brief Find service handle
    std::shared_ptr<ara::com::FindServiceHandle> m_findHandle;
};
 
} /// namespace port
} /// namespace aa
} /// namespace sensorfusion
 
#endif /// PARA_AA_GEN_SOFTWARE_COMPONENT_RPORT_SENSORFUSION_AA_SIMULATIONDATA_H