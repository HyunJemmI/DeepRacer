import argparse
import socket
import struct

import numpy as np
import tensorflow as tf


DEFAULT_MODEL_PATH = "/home/ubuntu/test_socket/model.pb"
DEFAULT_HOST = "0.0.0.0"
DEFAULT_PORT = 8080
IMAGE_HEIGHT = 120
IMAGE_WIDTH = 160
STEREO_CHANNELS = 2
BUFFER_SIZE = IMAGE_HEIGHT * IMAGE_WIDTH * STEREO_CHANNELS
INPUT_TENSOR_NAME = "main_level/agent/main/online/network_0/STEREO_CAMERAS/STEREO_CAMERAS:0"
OUTPUT_TENSOR_NAME = "main_level/agent/main/online/network_1/ppo_head_0/policy:0"


def parse_args():
    parser = argparse.ArgumentParser(description="Run DeepRacer TensorFlow inference server.")
    parser.add_argument("--model-path", default=DEFAULT_MODEL_PATH)
    parser.add_argument("--host", default=DEFAULT_HOST)
    parser.add_argument("--port", type=int, default=DEFAULT_PORT)
    return parser.parse_args()


def load_session(model_path):
    graph_def = tf.compat.v1.GraphDef()
    with tf.io.gfile.GFile(model_path, "rb") as model_file:
        graph_def.ParseFromString(model_file.read())

    session = tf.compat.v1.Session()
    tf.import_graph_def(graph_def, name="")

    input_tensor = session.graph.get_tensor_by_name(INPUT_TENSOR_NAME)
    output_tensor = session.graph.get_tensor_by_name(OUTPUT_TENSOR_NAME)
    return session, input_tensor, output_tensor


def receive_exact(client_socket, size):
    data = bytearray()
    while len(data) < size:
        packet = client_socket.recv(size - len(data))
        if not packet:
            return None
        data.extend(packet)
    return data


def run_prediction(session, input_tensor, output_tensor, data):
    matrix_data = np.frombuffer(data, dtype=np.uint8).reshape(
        IMAGE_HEIGHT,
        IMAGE_WIDTH,
        STEREO_CHANNELS,
    )
    input_data = np.expand_dims(matrix_data, axis=0)
    output = session.run(output_tensor, feed_dict={input_tensor: input_data})
    steering = float(output[0][0])
    throttle = float(output[0][1])
    return steering, throttle


def handle_client(client_socket, session, input_tensor, output_tensor):
    with client_socket:
        while True:
            data = receive_exact(client_socket, BUFFER_SIZE)
            if data is None:
                break

            steering, throttle = run_prediction(session, input_tensor, output_tensor, data)
            client_socket.sendall(struct.pack("ff", steering, throttle))
            print(f"Predicted Steering: {steering}, Throttle: {throttle}")


def run_server(host, port, model_path):
    session, input_tensor, output_tensor = load_session(model_path)

    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as server_socket:
        server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        server_socket.bind((host, port))
        server_socket.listen(1)
        print(f"Inference server listening on {host}:{port}")

        try:
            while True:
                print("Waiting for client connection...")
                client_socket, address = server_socket.accept()
                print(f"Client connected: {address[0]}:{address[1]}")

                try:
                    handle_client(client_socket, session, input_tensor, output_tensor)
                except Exception as exc:
                    print(f"Client handling error: {exc}")
        finally:
            session.close()


def main():
    args = parse_args()
    run_server(args.host, args.port, args.model_path)


if __name__ == "__main__":
    main()
