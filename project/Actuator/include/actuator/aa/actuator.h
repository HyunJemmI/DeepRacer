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
/// GENERATED FILE NAME               : actuator.h
/// SOFTWARE COMPONENT NAME           : Actuator
/// GENERATED DATE                    : 2024-08-14 09:44:02
///////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef PARA_AA_GEN_SOFTWARE_COMPONENT_ACTUATOR_AA_H
#define PARA_AA_GEN_SOFTWARE_COMPONENT_ACTUATOR_AA_H
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/// INCLUSION HEADER FILES
///////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "actuator/aa/port/controldata.h"

#include "para/swc/port_pool.h"

namespace actuator
{
namespace aa
{

class Actuator
{
public:
    /// @brief Constructor
    Actuator();

    /// @brief Destructor
    ~Actuator();

    /// @brief Initialize software component
    bool Initialize();

    /// @brief Start software component
    void Start();

    /// @brief Terminate software component
    void Terminate();

    // Motor control functions
    void ControlMotorStart();    // 모터 시작
    void ControlMotorStop();     // 모터 정지
    void ControlMotorSpeed(float speed); // 모터 속도 제어

private:
    /// @brief Run software component
    void Run();

    /// @brief Task software component : Receive CEvent Cyclic
    void TaskReceiveCEventCyclic();

    /// @brief CEvent Receiver
    void OnReceiveCEvent(const deepracer::service::controldata::proxy::events::CEvent::SampleType& sample);

private:
    /// @brief Pool of port
    ::para::swc::PortPool m_workers;

    /// @brief Logger for software component
    ara::log::Logger& m_logger;

    /// @brief Instance of Port {Actuator.ControlData}
    std::shared_ptr<actuator::aa::port::ControlData> m_ControlData;
};

} /// namespace aa
} /// namespace actuator

#endif /// PARA_AA_GEN_SOFTWARE_COMPONENT_ACTUATOR_AA_H
