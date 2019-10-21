"""
Program: Chat Server (server.py)
Description: Spawns a TCP server for a novel chat system
Written by: Alexander Nead-Work (neadwora)
Course: CS 372-400 (Fall 2019, Redfield)
Last Modified: 2019 Oct 22
"""

import socket

username = "i_am_groot"
prompt = f"{username}> "
max_buf = 513

class Server:
    def __init__(self, port: int):
        self.port = port
        self.host = socket.gethostname()

        self.start()

    def start(self):
        """Start the chat server"""

        self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

        try:
            self.socket.bind((self.host, self.port))
            self.socket.listen(1)
            print(f"[*] server listening on {self.host}:{self.port}")
        except:
            print("[!] error spawning server, try different port?")

        conn, addr = self.socket.accept() # blocking
        print(f"[*] got connection from {addr}")

        handle(conn)

    def handle(self, conn: socket):
        """Handle a client socket connection"""

        # client sends first message
        while True:
            data = conn.receive(max_buf) # blocking
            
            if "\quit" in data:
                print("[*] closing (requested by client)")
                conn.close()
                break

            print(data)

            msg = get_input()
            
            if msg == "\quit":
                print("[*] closing (requested by server)")
                conn.close()
                break

            if len(msg) > 500:
                print("[!] msg longer than 500 bytes, truncating")
                msg = msg[:500]
            data = prompt + msg
            conn.sendall(data)
        
    def get_input(self):
        return input(prompt)