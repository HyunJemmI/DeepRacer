import tensorflow as tf
import numpy as np
from PIL import Image
import matplotlib.pyplot as plt  # 이미지를 화면에 띄우기 위한 라이브러리

# 모델 및 이미지 경로 설정
image_num = 4


model_path = 'C:/Users/Ssong/newmodel.pb'
left_image_path = f'C:/Users/Ssong/Desktop/data/left{image_num}.png'   # 왼쪽 이미지 경로
right_image_path = f'C:/Users/Ssong/Desktop/data/right{image_num}.png'  # 오른쪽 이미지 경로

# Action space 범위 설정 (model_metadata.json 파일에서 가져옴)
scaleInfo = {
    "steering_angle": {
        "high": 20,
        "low": -20
    },
    "speed": {
        "high": 3.0,
        "low": 0.5
    }
}

STEER = "steering_angle"
SPEED = "speed"
HIGH = "high"
LOW = "low"

# helper function
def scaler(value, fromMin, fromMax, toMin, toMax):
    if(fromMin == fromMax): raise ZeroDivisionError
    return ((toMax - toMin) / (fromMax - fromMin)) * (value - fromMin) + toMin

# 이미지 전처리 함수
def preprocess_image(image_path):
    image = Image.open(image_path).convert("L")  # 흑백 변환
    image = image.resize((160, 120))  # 모델 입력 크기에 맞게 조정
    #image_data = np.array(image) / 255.0  # 0-1 사이 값으로 정규화
    return image

# 모델 그래프 로드
with tf.io.gfile.GFile(model_path, 'rb') as f:
    graph_def = tf.compat.v1.GraphDef()
    graph_def.ParseFromString(f.read())

# 왼쪽 및 오른쪽 이미지 불러오기 및 전처리
left_image_data = preprocess_image(left_image_path)
right_image_data = preprocess_image(right_image_path)

# 두 이미지를 채널 차원에서 결합하여 (120, 160, 2) 형식으로 만듭니다
combined_image_data = np.stack([left_image_data, right_image_data], axis=-1)

# 입력 데이터 형식에 맞게 배치 차원 추가하여 (1, 120, 160, 2) 형식으로 만들기
input_data = np.expand_dims(combined_image_data, axis=0)

# TensorFlow 세션 생성 및 모델 실행
with tf.compat.v1.Session() as sess:
    tf.import_graph_def(graph_def, name='')

    # 정확한 입력 및 출력 텐서 이름 설정
    input_tensor = sess.graph.get_tensor_by_name('main_level/agent/main/online/network_0/STEREO_CAMERAS/STEREO_CAMERAS:0')
    output_tensor = sess.graph.get_tensor_by_name('main_level/agent/main/online/network_1/ppo_head_0/policy:0')

    # 모델 실행 및 결과값 얻기
    output = sess.run(output_tensor, feed_dict={input_tensor: input_data})

    # 예측된 조향각 및 속도 값을 action space 범위에 맞게 변환
    #steering = scaler(output[0][0], -1.0, 1.0, scaleInfo[STEER][LOW], scaleInfo[STEER][HIGH])
    #speed = scaler(output[0][1], -1.0, 1.0, scaleInfo[SPEED][LOW], scaleInfo[SPEED][HIGH])
    
    steering = output[0][0]
    speed = output[0][1]

    # action_values로 설정하여 값 제한
    #steering = max(min(steering, 1), -1)
    #speed = max(min(speed, 1), -1)

    # 결과 출력
    print("Predicted Steering Angle:", steering)
    print("Predicted Speed:", speed)

# 이미지 표시
plt.figure(figsize=(10, 5))

# 왼쪽 이미지
plt.subplot(1, 2, 1)
plt.imshow(left_image_data, cmap='gray')
plt.title('Left Image')
plt.axis('off')

# 오른쪽 이미지
plt.subplot(1, 2, 2)
plt.imshow(right_image_data, cmap='gray')
plt.title('Right Image')
plt.axis('off')

# 이미지 표시
plt.show()
