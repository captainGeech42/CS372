#!/usr/bin/env python3

"""
Program: ftclient (ftclient.py)
Description: Connect to a TCP server for a file transfer system
Written by: Alexander Nead-Work (neadwora)
Course: CS 372-400 (Fall 2019, Redfield)
Last Modified: 2019 Dec 1
"""

import argparse
import socket
import sys
import os

def usage():
    """
    usage
    Arguments: none
    Returns: none
    Pre-cond: none
    Post-cond: none
    """
    print("[!] usage: ./ftclient <server host> <server port> <command> <data port>")
    print("valid commands:")
    print("\t-l: list directory")
    print("\t-g <filename>: get file")

def main(argv):
    if len(argv) < 5:
        usage()
        return 1

    try:
        server = argv[1]
        port = int(argv[2])
        command = argv[3]
        if command == "-l":
            data_port = int(argv[4])
        elif command == "-g":
            command += f" {argv[4]}"
            data_port = int(argv[5])
        else:
            print("[!] invalid command")
            usage()
            return 1
    except:
        print("[!] invalid arg")
        usage()
        return 1

    # spawn control connection
    control_socket = do_control_connection(server, port)

    # spawn data listener
    data_socket = setup_data_connection(data_port)

    # send control data
    configure_control(control_socket, command, data_port)

    # wait for data
    get_data(control_socket, data_socket, command)

    # cleanup
    data_socket.close()
    control_socket.close()

    return 0

def net_getline(s):
    """
    net_getline
    Arguments: socket 
    Returns: string from socket
    Pre-cond: socket is connected
    Post-cond: none
    """
    d = ""
    while True:
        d += s.recv(1).decode("ascii")
        if d[-1] == "\n":
            return d[:-1]

def do_control_connection(server, port):
    """
    do_control_connection
    Arguments: server, port
    Returns: control socket
    Pre-cond: server and port are valid args
    Post-cond: none
    """
    ip = socket.gethostbyname(server)

    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect((ip, port))

    return s

def setup_data_connection(port):
    """
    setup_data_connection
    Arguments: port
    Returns: data socket
    Pre-cond: port is a valid arg
    Post-cond: none
    """
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.bind(("", port))
    s.listen(1)

    return s

def get_data(control_sock, data_sock, command):
    """
    get_data
    Arguments: control_sock, data_sock, command
    Returns: none
    Pre-cond: control_sock and data_sock are connected
    Post-cond: none
    """
    conn, _ = data_sock.accept()
    if "-l" in command:
        print("[*] getting directory listing from server")
        while True:
            de = net_getline(conn)
            if de == "\0\0":
                break
            print(de)
    elif "-g" in command:
        filename = command[3:]
        control_data = net_getline(control_sock)
        if control_data != "good":
            print(f"[!] server sent error: {control_data}")
            conn.close()
            return

        if os.path.isfile(filename):
            print("[!] file already exists, exiting")
            conn.close()
            return

        print(f"[*] getting {filename} from server")

        file_len = int(net_getline(conn))
        file_data = b""
        while len(file_data) < file_len:
            file_data += conn.recv(1500)
            # file_data += conn.recv(1500 if file_len-i >= 1500 else file_len-i)
        with open(filename, "wb") as f:
            f.write(file_data[:file_len])
        print(f"[*] file written to \"{filename}\"")

    conn.close()

def configure_control(s, command, data_port):
    """
    configure_control
    Arguments: s, command, data_port
    Returns: none
    Pre-cond: s is valid/connected control connection
    Post-cond: none
    """
    s.send(f"{data_port}\n".encode("ascii"))
    s.send(f"{command}\n".encode("ascii"))

if __name__ == "__main__":
    sys.exit(main(sys.argv))