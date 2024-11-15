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
/// GENERATED FILE NAME               : sensor.cpp
/// SOFTWARE COMPONENT NAME           : Sensor
/// GENERATED DATE                    : 2024-11-01 13:31:53
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/// INCLUSION HEADER FILES
///////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "sensor/aa/sensor.h"
#include "sensor/aa/homomorphic_filter.hpp"

namespace sensor
{
    namespace aa
    {

        Sensor::Sensor()
            : m_logger(ara::log::CreateLogger("SENS", "SWC", ara::log::LogLevel::kVerbose)), m_workers(3), m_running(false), m_simulation(false), udp_ip("172.31.41.14") // IP on the receiving side of the data
              ,
              udp_port(65534) // Port Number
              ,
              sock(socket(AF_INET, SOCK_DGRAM, 0)) // udp 통신 소켓
              ,
              data_path("/home/ubuntu/test_socket_AA_data"), last_save_time(std::chrono::system_clock::now()) // 데이터 저장 시간
              ,
              save_interval(std::chrono::seconds(5)) // path로 데이터 저장 주기
              ,
              capR(), capL()
        {
        }

        Sensor::~Sensor()
        {
        }

        bool Sensor::Initialize()
        {
            m_logger.LogVerbose() << "Sensor::Initialize";

            bool init{true};

            m_RawData = std::make_shared<sensor::aa::port::RawData>();

            cv::namedWindow("filtering", cv::WINDOW_AUTOSIZE);

            std::string path = "/home/hjshin/Downloads/deepracerimg/1.png";

            cv::Mat frameR = cv::imread(path, cv::IMREAD_ANYCOLOR);

            // if(image.empty())
            // {
            //     std::cerr << "Image Loaded Fail !!" << std::endl;
            //     return -1;
            // }

            return init;
        }

        void Sensor::Start()
        {
            m_logger.LogVerbose() << "Sensor::Start";

            m_RawData->Start();

            // run software component
            Run();
        }

        void Sensor::Terminate()
        {
            m_logger.LogVerbose() << "Sensor::Terminate";

            m_running = false;

            if (!m_simulation)
            {
                capR.release();
                capL.release();
            }
            else
            {
                close(sock);
            }

            m_RawData->Terminate();
        }

        void Sensor::Run()
        {
            m_logger.LogVerbose() << "Sensor::Run";

            m_running = true;

            m_workers.Async([this]
                            { TaskGenerateREventValue(); });
            m_workers.Async([this]
                            { m_RawData->SendEventREventCyclic(); });
            m_workers.Async([this]
                            { m_RawData->NotifyFieldRFieldCyclic(); });

            m_workers.Wait();
        }

        void Sensor::TaskGenerateREventValue()
        {
            cv::Mat frameR;               // 카메라1 이미지 프레임
            cv::Mat frameL;               // 카메라2 이미지 프레임
            cv::Mat frameR_grayscaled;    // GrayScaled 처리된 프레임1
            cv::Mat frameL_grayscaled;    // GrayScaled 처리된 프레임2
            cv::Mat resultR;              // homomorphic filter 처리된 프레임1
            cv::Mat resultL;              // homomorphic filter 처리된 프레임2
            std::vector<uint8_t> bufferR; // 비트맵 Flatten vector1
            std::vector<uint8_t> bufferL; // 비트맵 Flatten vector2
            hf::GaussianHighPassFilter hpf; //고주파 필터 인스턴스 생성
            int borderType = cv::BORDER_REPLICATE; //테두리 타입 설정
            bufferR.reserve(19200);
            bufferL.reserve(19200);

            char buffer[65536]; // udp 통신 데이터 받을 버퍼
            sockaddr_in addr;
            socklen_t addr_len = sizeof(addr);

            while (m_running)
            {
                m_logger.LogVerbose() << "Sensor::Running Enter --------------- ";
                if (m_simulation)
                {
                    ssize_t len = recvfrom(sock, buffer, sizeof(buffer), 0, (struct sockaddr *)&addr, &addr_len);
                    if (len > 0)
                    {
                        try
                        {
                            double timestamp;
                            std::memcpy(&timestamp, buffer, sizeof(double)); // Copy timestamp

                            bufferL.assign(buffer + 8, buffer + 19208);     // Extract left image data
                            bufferR.assign(buffer + 19208, buffer + 38408); // Extract right image data
                            std::vector<float> lidar_data(8);               // Extract lidar data
                            std::memcpy(lidar_data.data(), buffer + 38408, 8 * sizeof(float));

                            // // Sensor::data_path에 데이터 저장
                            // auto current_time = std::chrono::system_clock::now();
                            // if (current_time - last_save_time >= save_interval) {
                            //     save_data(timestamp, bufferL, bufferR, lidar_data);
                            //     last_save_time = current_time;
                            // }
                        }
                        catch (const std::exception &e)
                        {
                            m_logger.LogVerbose() << "Sensor::TaskGenerateREventValue - Error unpacking data: " << e.what();
                        }
                    }
                }
                else
                {

                    m_logger.LogVerbose() << "Sensor::Camera process Enter --------------- ";
                    // // 카메라 캡처
                    // capR >> frameR;
                    // capL >> frameL;

                    // GrayScale
                    cv::cvtColor(frameR, frameR_grayscaled, cv::COLOR_BGR2GRAY);

                    // Homomorphic Filter
                    hf::homomorphicFilter(frameR_grayscaled, resultR, 15.0, 0.5, 2.0, hpf, borderType);

                    // 19200 고정된 크기로 Flatten(imencode하면 벡터 사이즈 변환 가능...)
                    bufferR.assign(resultR.datastart, resultR.dataend);

                    cv::imshow("frameR_grayscaled", resultR);
                    // cv::imshow("frameL_grayscaled", frameL_grayscaled);
                    // // esc 누르면 끄기
                    // if (cv::waitKey(10) == 27)
                    // {
                    //     m_running = false;
                    // }
                    std::this_thread::sleep_for(std::chrono::milliseconds(100)); // fps
                    m_logger.LogVerbose() << "Sensor::imshow pass --------------- ";
                }

                std::vector<uint8_t> bufferCombined; // Calc로 보낼 벡터
                bufferCombined.reserve(bufferR.size() + bufferL.size());
                bufferCombined.insert(bufferCombined.end(), bufferR.begin(), bufferR.end());
                bufferCombined.insert(bufferCombined.end(), bufferL.begin(), bufferL.end());

                deepracer::service::rawdata::skeleton::events::REvent::SampleType settingSampleValue = bufferCombined;
                // RawData 서비스의 REvent로 전송해야 할 값을 변경한다. 이 함수는 전송 타겟 값을 변경할 뿐 실제 전송은 다른 부분에서 진행된다.
                m_RawData->WriteDataREvent(settingSampleValue);

                m_logger.LogInfo() << "Sensor::Call RawData->WriteDataREvent size (R = " << bufferR.size() << " , L = " << bufferL.size() << ")";
            }
        }

        void Sensor::save_data(double timestamp, const std::vector<uint8_t> &left_image, const std::vector<uint8_t> &right_image, const std::vector<float> &lidar_data)
        {
            save_camera_data(left_image, timestamp, "left");
            save_camera_data(right_image, timestamp, "right");
            save_lidar_data(lidar_data, timestamp);
        }

        void Sensor::save_lidar_data(const std::vector<float> &lidar_data, double timestamp)
        {
            try
            {
                std::ofstream file(data_path + "/lidar_" + std::to_string(timestamp) + ".txt");
                for (const auto &value : lidar_data)
                {
                    file << value << "\n";
                }
                file.close();
                m_logger.LogInfo() << "Sensor::save_lidar_data - Lidar data saved";
            }
            catch (const std::exception &e)
            {
                m_logger.LogVerbose() << "Sensor::save_lidar_data - Error saving lidar data: " << e.what();
            }
        }

        void Sensor::save_camera_data(const std::vector<uint8_t> &img_bytes, double timestamp, const std::string &camera_name)
        {
            try
            {
                if (img_bytes.size() != 160 * 120)
                {
                    m_logger.LogVerbose() << "Sensor::save_camera_data - Warning: received image size does not match expected size";
                    m_running = false;
                }
                cv::Mat img(120, 160, CV_8UC1, const_cast<uint8_t *>(img_bytes.data()));
                cv::imwrite(data_path + "/" + camera_name + "_" + std::to_string(timestamp) + ".png", img);
                m_logger.LogInfo() << "Sensor::save_camera_data - Camera data (" << camera_name.c_str() << ") saved";
            }
            catch (const std::exception &e)
            {
                m_logger.LogVerbose() << "Sensor::save_camera_data - Error saving camera data: " << e.what();
            }
        }

    } /// namespace aa
} /// namespace sensor
