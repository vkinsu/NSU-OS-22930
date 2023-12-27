import socket


host = socket.gethostname()  
port = 5000
client_socket = socket.socket()  
client_socket.connect((host, port)) 

while True:
    data = input("Enter text: ")
    if not data:
        break
    client_socket.send(data.encode('utf-8'))

client_socket.close()
