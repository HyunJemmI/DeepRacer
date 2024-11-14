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
/// GENERATED FILE NAME               : sm.cpp
/// SOFTWARE COMPONENT NAME           : SM
/// GENERATED DATE                    : 2024-11-14 15:25:13
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/// INCLUSION HEADER FILES
///////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "sm/para/sm.h"
 
namespace sm
{
namespace para
{
 
SM::SM()
    : m_logger(ara::log::CreateLogger("SM", "SWC", ara::log::LogLevel::kVerbose))
    , m_workers(3)
{
}
 
SM::~SM()
{
}
 
bool SM::Initialize(int argc, char *argv[])
{
    m_logger.LogVerbose() << "SM::Initialize";
    
    bool init{true};
    
    m_DeepRacerFG = std::make_shared<sm::para::port::DeepRacerFG>();
    m_MachineFG = std::make_shared<sm::para::port::MachineFG>();
    
    ParseArgumentToState(argc, argv);

    return init;
}
 
void SM::Start()
{
    m_logger.LogVerbose() << "SM::Start";
    
    m_DeepRacerFG->Start();
    m_MachineFG->Start();
    
    // run software component
    Run();
}
 
void SM::Terminate()
{
    m_logger.LogVerbose() << "SM::Terminate";
    
    m_DeepRacerFG->Terminate();
    m_MachineFG->Terminate();
}
 
void SM::Run()
{
    m_logger.LogVerbose() << "SM::Run";
    
    m_workers.Async([this] { TaskChangeDeepRacerFGState(); });
    m_workers.Async([this] { m_DeepRacerFG->NotifyDeepRacerFGCyclic(); });
    m_workers.Async([this] { m_MachineFG->NotifyMachineFGCyclic(); });
    
    m_workers.Wait();
}

void SM::TaskChangeDeepRacerFGState()
{
    // DeepRacerFG 포트에서 FG를 SM에서 직접 제어하여 State를 변경하도록 한다.
    if (m_stateType == ara::sm::DeepRacerStateType::kDevice || m_stateType == ara::sm::DeepRacerStateType::kSimulation)
    {
        m_DeepRacerFG->ChangeDeepRacerFGManual(m_stateType);
    }
}

// Initialize에서 호출되는 함수로 Program argument를 통해 SM Run시에 바꿔야 할 State를 지정하여 주는 함수
void SM::ParseArgumentToState(int argc, char *argv[])
{
    if (argc >= 2)
    {
        std::string argument {argv[1]};
        std::transform(argument.begin(), argument.end(), argument.begin(), ::tolower);

        // Program Argument가 simulation으로 설정되어 있다면 Simulation State로 설정하게 된다.
        // 즉, SimActuator를 실행한다.
        if (argument == "simulation") {
            m_stateType = ara::sm::DeepRacerStateType::kSimulation;
        }
        // Program Argument가 device로 설정되어 있다면 Device State로 설정하게 된다.
        // 즉, Actuator를 실행한다.
        else if (argument == "device") {
            m_stateType = ara::sm::DeepRacerStateType::kDevice;
        }
    }
}
 
} /// namespace para
} /// namespace sm
