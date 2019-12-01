#!/usr/bin/env python3

"""
Program: ftclient (ftclient.py)
Description: Connect to a TCP server for a file transfer system
Written by: Alexander Nead-Work (neadwora)
Course: CS 372-400 (Fall 2019, Redfield)
Last Modified: 2019 Nov 29
"""

import argparse
import sys
import os

def main(argv):
    parser = argparse.ArgumentParser(prog="ftclient")
    parser.add_argument("server")
    parser.add_argument("server_port")
    parser.add_argument("command")
    parser.add_argument("data_port")

    args = parser.parse_args()

    print(f"server: {args.server}")

    return 0

if __name__ == "__main__":
    sys.exit(main(sys.argv))