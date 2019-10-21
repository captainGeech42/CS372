#!/usr/bin/env python3

"""
Program: Chat Server (chatserve.py)
Description: Spawns a TCP server for a novel chat system
Written by: Alexander Nead-Work (neadwora)
Course: CS 372-400 (Fall 2019, Redfield)
Last Modified: 2019 Oct 22
"""

import sys

from server import Server

def main(argv):
    if len(argv) != 2:
        print(f"[!] usage: {argv[0]} <port>")
        return 1

    try:
        port = int(argv[1])
        assert(port >= 1024)
        assert(port < 65535)
    except:
        print("[!] invalid port specified (must be int between 1024 and 65535")
        return 1

    s = Server(port)

    return 0

if __name__ == "__main__":
    sys.exit(main(sys.argv))