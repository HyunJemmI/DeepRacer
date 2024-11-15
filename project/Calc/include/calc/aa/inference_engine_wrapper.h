#ifndef INFERENCE_ENGINE_WRAPPER_H
#define INFERENCE_ENGINE_WRAPPER_H

#include <inference_engine.hpp> // Inference Engine API 헤더
#include <string>
#include <vector>

class InferenceEngineWrapper {
public:
    InferenceEngineWrapper(const std::string& modelPath, const std::string& deviceName);
    ~InferenceEngineWrapper();

    void setInputData(const std::vector<float>& inputData);
    std::vector<float> runInference();

private:
    InferenceEngine::Core ie;                          // Inference Engine Core 객체
    InferenceEngine::ExecutableNetwork executableNet; // 컴파일된 네트워크
    InferenceEngine::InferRequest inferRequest;       // 추론 요청 객체
    InferenceEngine::InputsDataMap inputInfo;         // 입력 정보
    InferenceEngine::OutputsDataMap outputInfo;       // 출력 정보

    std::string modelPath;
    std::string deviceName;

    void loadModel(); // 모델 로드 함수
};

#endif // INFERENCE_ENGINE_WRAPPER_H
