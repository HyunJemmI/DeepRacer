import socket

# 서버 IP와 포트를 설정
SERVER_IP = '0.0.0.0'
SERVER_PORT = 15000

# 소켓 생성 및 설정
server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server_socket.bind((SERVER_IP, SERVER_PORT))
server_socket.listen(1)

print(f"서버가 {SERVER_IP}:{SERVER_PORT}에서 대기 중입니다...")

# 클라이언트 연결 대기
client_socket, client_address = server_socket.accept()
print(f"{client_address}에서 연결되었습니다.")

try:
    while True:
        # 클라이언트로부터 메시지 수신
        data = client_socket.recv(1024).decode()
        if not data:
            break
        print(f"클라이언트로부터 받은 메시지: {data}")

        # 클라이언트에 응답 전송
        response = f"서버 응답: {data}"
        client_socket.send(response.encode())
except Exception as e:
    print(f"에러 발생: {e}")
finally:
    client_socket.close()
    server_socket.close()
    print("서버를 종료합니다.")
