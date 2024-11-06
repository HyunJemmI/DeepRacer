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
/// GENERATED FILE NAME               : main.cpp
/// EXECUTABLE NAME                   : SM
/// GENERATED DATE                    : 2024-08-14 09:44:02
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/// INCLUSION HEADER FILES
///////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "ara/core/initialization.h"
#include "ara/exec/execution_client.h"
#include "ara/log/logger.h"
 
#include "sm/para/sm.h"
 
#include <csignal>
 
sm::para::SM* g_swcSM{nullptr};

// Signal 처리 함수
static void SignalHandler(std::int32_t signal)
{
    // SIGTERM 혹은 SIGINT 시그널을 받았을때의 종료 처리 수행
    if (signal == SIGTERM || signal == SIGINT)
    {
        // Software Component의 Terminate() 함수를 호출하여 종료 처리
        g_swcSM->Terminate();
    }
}
 
int main(int argc, char *argv[], char* envp[])
{
    bool proceed{true};
    bool araInitialized{true};

    // AUTOSAR Adaptive Application 에 대한 초기화
    // 모든 Adaptive Application (AA)는 처음에 ara::core::Initialize()를 호출하여 초기화를 진행해야 한다.
    auto appInit = ara::core::Initialize();
    if (!appInit.HasValue())
    {
        proceed = false;
        araInitialized = false;
    }
    
    if (araInitialized)
    {
        ara::log::Logger& appLogger{ara::log::CreateLogger("SM", "SM's main function")};

        // Signal Handler 등록
        std::signal(SIGTERM, SignalHandler);
        std::signal(SIGINT, SignalHandler);

        // Software Component 객체 선언 및 포인터 할당
        sm::para::SM swcSM;
        g_swcSM = &swcSM;

        // Software Component에 대한 초기화 함수 호출
        // SM 수정사항 : Program Argument (argc, argv)를 Software Component 초기화 함수 인자로 추가.
        proceed = swcSM.Initialize(argc, argv);
        
        if (proceed)
        {
            // AA에 대한 초기화 처리가 완료되었다면 현재 AA가 Running 상태가 되었다는 것을 EM에 보고해야 한다.
            ara::exec::ExecutionClient executionClient;
            auto exec = executionClient.ReportExecutionState(ara::exec::ExecutionState::kRunning);
            if (exec.HasValue())
            {
                appLogger.LogVerbose() << "Running adaptive application";
            }
            else
            {
                appLogger.LogError() << "Unable to report execution state";
                araInitialized = false;
            }
            // Software Component의 시작함수 호출
            swcSM.Start();
        }
        else
        {
            appLogger.LogError() << "Unable to start application";
        }

        // AUTOSAR Adaptive Application의 Deinitialize() 호출
        // 모든 AA들은 종료 전 ara::core::Deinitialize()를 호출해주어야 한다.
        auto appDeinit = ara::core::Deinitialize();
        if (!appDeinit.HasValue())
        {
            araInitialized = false;
        }
    }
    
    return (araInitialized && proceed) ? EXIT_SUCCESS : EXIT_FAILURE;
}
 
