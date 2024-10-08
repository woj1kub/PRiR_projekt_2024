import socket
from datetime import datetime

DEBUG = True

def save_text_to_file(text):
    now = datetime.now()
    file_name = f'backup_{now.strftime("%Y_%m_%d_%H_%M_%S")}.txt'
    with open(file_name, 'w') as file:
        file.write(text)

def start_server(host='localhost', port=12345):
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.bind((host, port))
        s.listen()
        firstRun = True
        
        while True:
            if firstRun:
                print(f'Server listening on {host}:{port}...')
                firstRun = False
            else:
                print(f'Server again listening on {host}:{port}...')
            conn, addr = s.accept()
            with conn:
                print(f'Connected by {addr}')
                while True:
                    data = conn.recv(4096)
                    if not data:
                        break
                    if DEBUG:
                        print(f'Received: {data.decode()}')
                    save_text_to_file(data.decode())
                    conn.sendall(b'Acknowledged')  # opcjonalnie wysyłamy potwierdzenie

if __name__ == '__main__':
    start_server()
    # save_text_to_file('test')