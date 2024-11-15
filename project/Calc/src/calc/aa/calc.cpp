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
/// GENERATED DATE                    : 2024-11-14 15:25:13
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/// INCLUSION HEADER FILES
///////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "calc/aa/calc.h"

#define SERVER_IP "34.233.119.231" // EC2 인스턴스의 public IP
#define PORT 15001

namespace calc
{
namespace aa
{

// 생성자: 클래스 멤버 초기화
Calc::Calc()
    : m_logger(ara::log::CreateLogger("CALC", "SWC", ara::log::LogLevel::kVerbose))
    , m_workers(4)
    , m_running(false)
    , m_socket_fd(-1) // 실제 통신에 사용되는 소켓
    , m_newDataAvailable(false) // 새로운 데이터 가용성 플래그
{
}

Calc::~Calc()
{
    CloseSocket();
}

// Client -> Server 연결
bool Calc::Initialize()
{
    m_logger.LogVerbose() << "Calc::Initialize";

    m_ControlData = std::make_shared<calc::aa::port::ControlData>();
    m_RawData = std::make_shared<calc::aa::port::RawData>();

    if ((m_socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        m_logger.LogError() << "Calc::Initialize - Socket creation failed";
        return false;
    }

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);

    if (inet_pton(AF_INET, SERVER_IP, &server_address.sin_addr) <= 0)
    {
        m_logger.LogError() << "Calc::Initialize - Invalid address/ Address not supported";
        return false;
    }

    if (connect(m_socket_fd, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
    {
        m_logger.LogError() << "Calc::Initialize - Connection Failed";
        return false;
    }

    m_logger.LogInfo() << "Calc::Initialize - Connected to server successfully";
    return true;
}

// 시작 함수: 컴포넌트 실행 시작
void Calc::Start()
{
    m_logger.LogVerbose() << "Calc::Start";

    m_ControlData->Start();
    m_RawData->Start();

    Run();
}

// 종료 함수: 리소스 정리 및 스레드 종료
void Calc::Terminate()
{
    m_logger.LogVerbose() << "Calc::Terminate";

    m_running = false;
    m_dataCV.notify_all();

    m_ControlData->Terminate();
    m_RawData->Terminate();

    CloseSocket();

    m_workers.Wait();
}

// 메인 실행 함수: 작업 스레드 시작
void Calc::Run()
{
    m_logger.LogVerbose() << "Calc::Run";

    m_running = true;

    m_workers.Async([this]{ TaskReceiveREventCyclic(); });
    m_workers.Async([this]{ SocketCommunication(); });
    m_workers.Async([this]{ m_ControlData->SendEventCEventCyclic(); });
    m_workers.Async([this]{ m_RawData->ReceiveFieldRFieldCyclic(); });

    m_workers.Wait();
}

// RawData 이벤트 수신 처리 함수
void Calc::OnReceiveREvent(const deepracer::service::rawdata::proxy::events::REvent::SampleType &sample)
{
    std::vector<uint8_t> bufferCombined = sample;

    m_logger.LogInfo() << "Calc::OnReceiveREvent - buffer size = " << bufferCombined.size() << " , buffer[10000] = "<< bufferCombined[10000];

    if (bufferCombined.size() == 38400){
        {
            std::lock_guard<std::mutex> lock(m_dataMutex);
            m_latestRawData = bufferCombined;
            m_newDataAvailable = true;
        }
        m_dataCV.notify_one(); //  대기 중인 스레드 중 하나를 깨움
    }
}

bool Calc::ReconnectToServer()
{
    CloseSocket();

    if ((m_socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        m_logger.LogError() << "Calc::ReconnectToServer - Socket creation failed during reconnection";
        return false;
    }

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);

    if (inet_pton(AF_INET, SERVER_IP, &server_address.sin_addr) <= 0)
    {
        m_logger.LogError() << "Calc::ReconnectToServer - Invalid address/ Address not supported during reconnection";
        return false;
    }

    if (connect(m_socket_fd, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
    {
        m_logger.LogError() << "Calc::ReconnectToServer - Reconnection Failed";
        return false;
    }

    m_logger.LogInfo() << "Calc::ReconnectToServer - Reconnected to server successfully";
    return true;
}

// 소켓 통신 처리 함수
void Calc::SocketCommunication()
{
    while (m_running)
    {
        // inference로의 데이터 전송
        std::vector<uint8_t> combinedData;
        {
            std::unique_lock<std::mutex> lock(m_dataMutex);
            m_dataCV.wait(lock, [this] { return m_newDataAvailable || !m_running; });

            if (!m_running)
                break;

            combinedData = m_latestRawData;
            m_newDataAvailable = false;
        }

        // 결합된 데이터 전송
        ssize_t sent_bytes = send(m_socket_fd, combinedData.data(), combinedData.size(), 0);

        if (sent_bytes != combinedData.size())
        {
            m_logger.LogError() << "Calc::SocketCommunication - Send failed: " << strerror(errno);
            if (errno == EPIPE)
            {
                m_logger.LogError() << "Calc::SocketCommunication - Broken pipe detected. Attempting to reconnect...";
                if (!ReconnectToServer())
                {
                    m_logger.LogError() << "Calc::SocketCommunication - Reconnection failed. Exiting communication loop.";
                    break;
                }
            }
            continue;
        }

        float32_t received_floats[2];
        ssize_t bytes_received = recv(m_socket_fd, received_floats, sizeof(float32_t) * 2, 0);

        if (bytes_received == sizeof(float) * 2)
        {
            m_logger.LogInfo() << "Calc::SocketCommunication - Received floats: " << received_floats[0] << ", " << received_floats[1];
            ProcessReceivedFloats(received_floats[0], received_floats[1]);
        }
        else if (bytes_received == 0)
        {
            m_logger.LogError() << "Calc::SocketCommunication - Connection closed by server. Attempting to reconnect...";
            if (!ReconnectToServer())
            {
                m_logger.LogError() << "Calc::SocketCommunication - Reconnection failed. Exiting communication loop.";
                break;
            }
        }
        else
        {
            m_logger.LogError() << "Calc::SocketCommunication - Receive failed: " << strerror(errno);
            if (errno == ECONNRESET)
            {
                m_logger.LogError() << "Calc::SocketCommunication - Connection reset by peer. Attempting to reconnect...";
                if (!ReconnectToServer())
                {
                    m_logger.LogError() << "Calc::SocketCommunication - Reconnection failed. Exiting communication loop.";
                    break;
                }
            }
            continue;
        }
    }
}

float Calc::mapsteering(float input_value)
{
    float output = std::max(-1.0f, std::min(1.0f, input_value));
    return output;
}

// mapping
float Calc::mapThrottle(float input_value)
{
    float input = abs(input_value);
    // 이차 함수에 따라 스로틀 값을 매핑 (y = -0.133333x^2 + 0.733333x)
    float output = -0.133333f * input * input + 0.733333f * input;

    // 출력 값이 0 ~ 1 범위 내에 있는지 확인하고 제한
    output = std::max(0.0f, std::min(1.0f, output));

    return output;
}

// 수신된 float 값 처리 함수
void Calc::ProcessReceivedFloats(float value1, float value2)
{
    float steering = mapsteering(value1);
    float throttle = mapThrottle(value2);

    deepracer::service::controldata::skeleton::events::CEvent::SampleType values = {steering, throttle};

    m_ControlData->WriteDataCEvent(values);

    m_logger.LogInfo() << "Calc::ProcessReceivedFloats - send values = { " << values[0] << ", " << values[1] << " }";
}

// RawData 이벤트 수신 작업 함수
void Calc::TaskReceiveREventCyclic()
{
    m_RawData->SetReceiveEventREventHandler([this](const auto &sample)
    {
        OnReceiveREvent(sample);
    });
    m_RawData->ReceiveEventREventCyclic();
}

void Calc::CloseSocket()
{
    if (m_socket_fd != -1)
    {
        close(m_socket_fd);
        m_socket_fd = -1;
    }
}



} /// namespace aa
} /// namespace calc
