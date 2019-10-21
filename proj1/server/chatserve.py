#!/usr/bin/env python3

import sys

from server import Server

def main(argv):
    s = Server("asdf", 10001)

    return 0

if __name__ == "__main__":
    sys.exit(main(sys.argv))
