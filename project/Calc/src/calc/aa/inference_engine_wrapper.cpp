#include "inference_engine_wrapper.h"
#include <iostream>
#include <stdexcept>

InferenceEngineWrapper::InferenceEngineWrapper(const std::string& modelPath, const std::string& deviceName)
    : modelPath(modelPath), deviceName(deviceName) {
    loadModel();
}

InferenceEngineWrapper::~InferenceEngineWrapper() {
    // 자원 해제는 자동으로 이루어짐
}

void InferenceEngineWrapper::loadModel() {
    // IR 모델 로드
    auto network = ie.ReadNetwork(modelPath);

    // 입력 및 출력 정보 가져오기
    inputInfo = network.getInputsInfo();
    outputInfo = network.getOutputsInfo();

    // 입력 형식 설정
    for (auto& input : inputInfo) {
        input.second->setPrecision(InferenceEngine::Precision::FP32);
    }

    // 네트워크를 특정 디바이스에 로드
    executableNet = ie.LoadNetwork(network, deviceName);

    // 추론 요청 객체 초기화
    inferRequest = executableNet.CreateInferRequest();
}

void InferenceEngineWrapper::setInputData(const std::vector<float>& inputData) {
    // 입력 Blob 생성
    auto inputBlob = inferRequest.GetBlob(inputInfo.begin()->first);
    auto data = inputBlob->buffer().as<float*>();
    std::copy(inputData.begin(), inputData.end(), data);
}

std::vector<float> InferenceEngineWrapper::runInference() {
    // 추론 실행
    inferRequest.Infer();

    // 출력 Blob 가져오기
    auto outputBlob = inferRequest.GetBlob(outputInfo.begin()->first);
    const float* outputData = outputBlob->buffer().as<float*>();
    size_t outputSize = outputBlob->size();

    // 결과 복사 및 반환
    return std::vector<float>(outputData, outputData + outputSize);
}
