# Adaptive Application 샘플
AA 설계 및 구현 교육시 설명하였던 AA 샘플 구조에 대한 설명 및 빌드 방식에 대하여 설명한다.

---
## 샘플 프로젝트 구조

```
${PROJECT_ROOT}
├─ /EM
│    ├─ /manifest               
│    │    ├─ /exec
│    │    ├─ /function_group
│    │    └─ /machine
│    └─ CMakeLists.txt
├─ /CM
│    ├─ /manifest               
│    │    ├─ /com
│    │    └─ /exec
│    └─ CMakeLists.txt
├─ /Sensor
│    ├─ /include
│    ├─ /manifest               
│    │    ├─ /exec
│    │    └─ /interface
│    ├─ /src
│    │    ├─ /sensor
│    │    ├─ main.cpp
│    │    └─ CMakeLists.txt
│    └─ CMakeLists.txt
├─ /Calc
│    └─ ...
├─ /Actuator
│    └─ ...
├─ /SimActuator
│    └─ ...
├─ /SM
│    └─ ...
├─ CMakeLists.txt
├─ autosar_design.arxml
└─ ...
```
1. 모든 AA 디렉토리 공통사항
  - manifest/exec
    - **execution-manifest**
    - 해당 AA에 대한 실행 정보가 Manifest로 정의
      - Process 이름, Version, State 의존성, Log 출력 정보 등
2. EM
  - Manifest만 존재
  - 실제 EM Binary는 PARA-SDK에서 제공
  - /manifest
    - /machine
      - **machine-manifest**
      - Machine에 관련된 정보들이 정의됨.
      - 해당 Machine에 대한 Network Endpoint (IP Address) 정보도 포함
    - /function_group
      - **function-group-manifest**
      - FunctionGroup 정보가 포함됨 (MachineFG, DeepRacerFG)
3. CM
  - Manifest만 존재
  - 실제 CM Binary는 PARA_SDK에서 제공
  - /manifest
    - /com
      - **communication-management-manifest**
      - 서비스 관련 정보가 존재
        - IP Address
        - 통신방식 (SOME/IP, IPC)
        - Skeleton (Provider) 정보
        - Proxy (Consumer) 정보
4. Application Level AA (Sensor, Calc, Actuator, SimActuator)
  - /manifest
    - /interface ...
      - **service-instance-manifest**
      - 해당 AA의 PortPrototype으로 정의된 Service Instance 정보가 포함
  - /include
    - /[service interface 설계시 지정한 namespace]
      - 설계 내용이 적용된 Service Interface (Proxy, Skeleton) Stub 코드 헤더 파일
    - /[data type 설계시 지정한 namespace]
      - Service 통신을 위해 지정하였던 C++ Type Data가 Generate된 헤더 파일
    - /[AA name]/[RootSwComponentName]
      - [SwComponentName].h
        - Software Component 처리가 구현된 Stub 코드 헤더 파일
      - /port
        - 설계한 Port들에 대한 처리가 구현된 Stub 코드 헤더 파일
  - /src
    - main.cpp
      - 해당 AA의 Entry Point인 main 함수가 구현된 cpp 파일
    - [AA name]/[RootSWComponentName]
      - [SwComponentName].cpp
        - Software Component 처리가 구현된 Stub 코드 cpp 파일
      - /port
        - 설계한 Port들에 대한 처리가 구현된 Stub 코드 cpp 파일
---
## 프로젝트 빌드 방법
- PARA SDK 관련 환경변수의 구성 확인
```
$ export PARA_SDK=/home/deepracer/Para_SDK
$ export PARA_CORE=$PARA_SDK
$ export PARA_CONF=$PARA_SDK/etc
$ export PARA_APPL=$PARA_SDK/opt
$ export PARA_DATA=$PARA_SDK/var
$ export LD_LIBRARY_PATH=$PARA_SDK/lib
```

1. 개발 프로젝트 내에서 진행
```
$ cd [개발 프로젝트]
```
2. CMake 구성
```
$ cmake -B build -D CMAKE_INSTALL_PREFIX=$PARA_SDK
```
3. 프로젝트 빌드
```
$ cmake --build build -j $(nproc)
```
4. 프로젝트 설치
```
$ cmake --install build
```
- 설치시 빌드 된 실행파일 및 Manifest들이 CMake 구성에 의해 자동으로 PARA_SDK 디렉토리 내의 필요한 위치로 복사됨.
---
## 설치된 AA의 실행
- $PARA_SDK/bin/EM을 통해 실행
```
$ cd $PARA_SDK/bin
$ ./EM
```
- DeepRacer 환경에서 실행시 실행 전 방화벽 해제 처리 필요
```
$ sudo ufw disable
```  
---
