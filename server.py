import socket
import threading

def handle_client(client_socket):
    while True:
        data = client_socket.recv(1024).decode('utf-8')
        if not data:
            break
        converted_data = data.upper()
        print("Received from client: ", data)
        print("Converted data: ", converted_data)
    client_socket.close()

host = socket.gethostname()
port = 5000
server_socket = socket.socket()  
server_socket.bind((host, port))  
server_socket.listen(10)

print("Server started.")

while True:
    client_socket, client_address = server_socket.accept()
    print("Connected client: ", client_address)
    threading.Thread(target=handle_client, args=(client_socket,)).start()
