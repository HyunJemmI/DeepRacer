import tensorflow as tf
import numpy as np
import socket
import struct

# 모델 및 소켓 설정
model_path = '/home/ubuntu/test_socket/model.pb'
HOST = '0.0.0.0'  # 모든 네트워크 인터페이스에서 연결 수락
PORT = 8080
BUFFER_SIZE = 38400  # 120 * 160 * 2 (이미지의 픽셀 수)

# 소켓 초기화 및 연결 대기
server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
server_socket.bind((HOST, PORT))
server_socket.listen(1)

print(f"{HOST}:{PORT} connect start~⊂((・▽・))⊃\.")

# 모델 그래프 로드
with tf.io.gfile.GFile(model_path, 'rb') as f:
    graph_def = tf.compat.v1.GraphDef()
    graph_def.ParseFromString(f.read())

# TensorFlow 세션 생성
sess = tf.compat.v1.Session()
tf.import_graph_def(graph_def, name='')

# 입력 및 출력 텐서 설정
input_tensor = sess.graph.get_tensor_by_name('main_level/agent/main/online/network_0/STEREO_CAMERAS/STEREO_CAMERAS:0')
output_tensor = sess.graph.get_tensor_by_name('main_level/agent/main/online/network_1/ppo_head_0/policy:0')

try:
    while True:
        print("Wait for connecting Client...")
        client_socket, addr = server_socket.accept()
        print(f"Complete!! ⊂((・▽・))⊃ port: {addr[1]}")

        try:
            while True:
                # 데이터 수신
                data = bytearray()
                while len(data) < BUFFER_SIZE:  # 38400 바이트 수신
                    packet = client_socket.recv(BUFFER_SIZE - len(data))
                    if not packet:
                        break
                    data.extend(packet)

                if len(data) < BUFFER_SIZE:
                    continue

                matrix_data = np.frombuffer(data, dtype=np.uint8).reshape(120, 160, 2)

                # 입력 데이터 형식에 맞게 배치 차원 추가 (1, 120, 160, 2)
                input_data = np.expand_dims(matrix_data, axis=0)

                # 모델 실행 및 결과값 얻기
                output = sess.run(output_tensor, feed_dict={input_tensor: input_data})

                # 결과 처리 (예: steering과 Throttle 추출)
                steering = output[0][0]
                Throttle = output[0][1]

                # 결과를 클라이언트에게 전송
                data = struct.pack('ff', steering, Throttle)
                client_socket.sendall(data)

                print(f"Predicted Steering: {steering}, Throttle: {Throttle}")

        except Exception as e:
            print(f"에러 발생: {e}")
        finally:
            client_socket.close()

except KeyboardInterrupt:
    print("서버를 종료합니다.")
finally:
    server_socket.close()
    sess.close()