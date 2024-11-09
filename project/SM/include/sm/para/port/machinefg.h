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
/// GENERATED FILE NAME               : machinefg.h
/// SOFTWARE COMPONENT NAME           : MachineFG
/// GENERATED DATE                    : 2024-11-01 13:31:52
///////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef PARA_AA_GEN_SOFTWARE_COMPONENT_PPORT_SM_PARA_MACHINEFG_H
#define PARA_AA_GEN_SOFTWARE_COMPONENT_PPORT_SM_PARA_MACHINEFG_H
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/// INCLUSION HEADER FILES
///////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "ara/sm/machinefg/triggerinout_machinefg_skeleton.h"
 
#include "ara/log/logger.h"
#include "ara/exec/function_group.h"
#include "ara/exec/function_group_state.h"
#include "ara/exec/state_client.h"
 
#include <mutex>
#include <thread>
 
namespace ara
{
namespace sm
{
namespace machinefg
{
namespace skeleton
{
 
class TriggerInOut_MachineFGSkeletonImpl : public TriggerInOut_MachineFGSkeleton
{
public:
    /// @brief Constructor
    TriggerInOut_MachineFGSkeletonImpl(ara::core::InstanceSpecifier instanceSpec, ara::com::MethodCallProcessingMode mode = ara::com::MethodCallProcessingMode::kEvent);
    
    
    /// @brief Getter for field, Notifier
    ara::core::Future<fields::Notifier::FieldType> GetNotifier();
    
    /// @brief Notifier for field, Notifier
    void NotifyMachineFG();
    
    /// @brief Update function group state value by software component, Notifier
    void UpdateMachineFG(const fields::Notifier::FieldType& value);
    
    /// @brief Setter for field, Trigger
    ara::core::Future<fields::Trigger::FieldType> SetTrigger(const fields::Trigger::FieldType& value);
    
    /// @brief Request to change function group state
    void RequestTransitFunctionGroupState(const fields::Trigger::FieldType& value);
    
    /// @brief Function for undefined state callback
    void UndefinedStateHandler(ara::exec::FunctionGroup& functionGroup);
    
private:
    /// @brief Logger for this port
    ara::log::Logger& m_logger;
    
    /// @brief Function group state, MachineFG
    fields::Trigger::FieldType m_MachineFGState;
    
    /// @brief State client
    std::unique_ptr<ara::exec::StateClient> m_stateClient;
    
    /// @brief Undefined state callback
    std::function<void(ara::exec::FunctionGroup&)> m_undefinedStateCallback;
};
 
} /// namespace skeleton
} /// namespace machinefg
} /// namespace sm
} /// namespace ara

namespace sm
{
namespace para
{
namespace port
{
 
class MachineFG
{
public:
    /// @brief Constructor
    MachineFG();
    
    /// @brief Destructor
    ~MachineFG();
    
    /// @brief Start port
    void Start();
    
    /// @brief Terminate port
    void Terminate();
    
    /// @brief Write field value to buffer, Notifier
    void WriteValueMachineFG(const ara::sm::machinefg::skeleton::fields::Notifier::FieldType& value);
     
    /// @brief Notify field cyclic from buffer, Notifier
    void NotifyMachineFGCyclic();
     
    /// @brief Notify field directly from buffer data, Notifier
    void NotifyMachineFGTriggered();
     
    /// @brief Notify field directly with argument, Notifier
    void NotifyMachineFGTriggered(const ara::sm::machinefg::skeleton::fields::Notifier::FieldType& value);
     
private:
    /// @brief Logger for this port
    ara::log::Logger& m_logger;
    
    /// @brief Flag of port status
    bool m_running;
    
    /// @brief Mutex for this port
    std::mutex m_mutex;
    
    /// @brief AUTOSAR Port Interface
    std::shared_ptr<ara::sm::machinefg::skeleton::TriggerInOut_MachineFGSkeletonImpl> m_interface;
    
};
 
} /// namespace port
} /// namespace para
} /// namespace sm
 
#endif /// PARA_AA_GEN_SOFTWARE_COMPONENT_PPORT_SM_PARA_MACHINEFG_H