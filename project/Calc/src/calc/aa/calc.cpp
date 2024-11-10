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
/// GENERATED FILE NAME               : calc.cpp
/// SOFTWARE COMPONENT NAME           : Calc
/// GENERATED DATE                    : 2024-11-01 13:31:52
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/// INCLUSION HEADER FILES
///////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "calc/aa/calc.h"
 
#include <iostream>
#include <fstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <opencv2/opencv.hpp>
#include <vector>
#include <chrono>
#include <cstdint>

namespace calc
{
namespace aa
{
 
Calc::Calc()
    : m_logger(ara::log::CreateLogger("CALC", "SWC", ara::log::LogLevel::kVerbose))
    , m_workers(3)
    , m_running(false)
{
}
 
Calc::~Calc()
{
}
 
bool Calc::Initialize()
{
    m_logger.LogVerbose() << "Calc::Initialize";
    
    bool init{true};
    
    m_ControlData = std::make_shared<calc::aa::port::ControlData>();
    m_RawData = std::make_shared<calc::aa::port::RawData>();
    
    return init;
}
 
void Calc::Start()
{
    m_logger.LogVerbose() << "Calc::Start";
    
    m_ControlData->Start();
    m_RawData->Start();
    
    // run software component
    Run();
}
 
void Calc::Terminate()
{
    m_logger.LogVerbose() << "Calc::Terminate";
    
    m_running = false;

    m_ControlData->Terminate();
    m_RawData->Terminate();
}
 
void Calc::Run()
{
    m_logger.LogVerbose() << "Calc::Run";
    
    m_running = true;

    m_workers.Async([this] { TaskReceiveREventCyclic(); });
    m_workers.Async([this] { m_ControlData->SendEventCEventCyclic(); });
    m_workers.Async([this] { m_RawData->ReceiveFieldRFieldCyclic(); });
    
    m_workers.Wait();
}

void Calc::TaskReceiveREventCyclic()
{
    m_RawData->SetReceiveEventREventHandler([this](const auto& sample)
    {
        OnReceiveREvent(sample);
    });
    m_RawData->ReceiveEventREventCyclic();
}

void Calc::OnReceiveREvent(const deepracer::service::rawdata::proxy::events::REvent::SampleType& sample)
{
    std::vector<uint8_t> bufferCombined = sample;
    std::vector<uint8_t> bufferR(bufferCombined.begin(), bufferCombined.begin() + 19200);
    std::vector<uint8_t> bufferL(bufferCombined.begin() + 19200, bufferCombined.end());

    m_logger.LogInfo() << "Calc::OnReceiveREvent - buffer size R = " << bufferR.size() << ", L = " << bufferL.size();

    // // Image 저장
    // std::string data_path = "/home/ubuntu/test_socket_AA_data";
    // try {
    //     if ((bufferR.size() != 160 * 120) || (bufferL.size() != 160 * 120)) {
    //         m_logger.LogVerbose() << "Calc::OnReceiveREvent - Warning: received image size does not match expected size";
    //         m_running = false;
    //     }else{
    //         cv::Mat imgR(120, 160, CV_8UC1, const_cast<uint8_t*>(bufferR.data()));
    //         cv::Mat imgL(120, 160, CV_8UC1, const_cast<uint8_t*>(bufferL.data()));
    //         cv::imwrite(data_path + "/" + "right" + "_" + std::to_string(bufferCombined[10000]) + ".png", imgR);
    //         m_logger.LogInfo() << "Calc::OnReceiveREvent - Camera data ( right ) saved";
    //         cv::imwrite(data_path + "/" + "left" + "_" + std::to_string(bufferCombined[10000]) + ".png", imgL);
    //         m_logger.LogInfo() << "Calc::OnReceiveREvent - Camera data ( left ) saved";
    //     }
    // } catch (const std::exception& e) {
    //     m_logger.LogVerbose() << "Calc::OnReceiveREvent - Error saving camera data: " << e.what();
    // }

    // ControlData 서비스의 CEvent로 전송해야 할 값을 변경한다. 이 함수는 전송 타겟 값을 변경할 뿐 실제 전송은 다른 부분에서 진행된다.
    m_ControlData->WriteDataCEvent(sample);
    m_logger.LogInfo() << "Calc::OnReceiveREvent - data size = " << bufferCombined.size();
}
 
} /// namespace aa
} /// namespace calc
