Compile:
Run `make` from the main directory. This will compile the client, and symlink/+execute bit the server.

Execute:
Run server with `./chatserve [port]`. Example:

    ./chatserve 32534

Run client with `./chatclient [host] [port]`. Example:

    ./chatclient flip2.engr.oregonstate.edu 32534

You can also specify an IPv4 address instead of a DNS name for the client host.

If you get a traceback from the server that ends in this:
      File "/usr/lib64/python3.6/socket.py", line 205, in accept
        fd, addr = self._accept()
    OSError: [Errno 22] Invalid argument
You may need to pick a different port

References:
https://docs.python.org/3.6/howto/sockets.html
https://wiki.python.org/moin/TcpCommunication
https://github.com/captainGeech42/CS344/tree/master/otp (this is my own GitHub, sockets assignment from OS1)
https://stackoverflow.com/questions/34069937/c-socket-programming-client-server-connection-on-host-name
https://stackoverflow.com/questions/791982/determine-if-a-string-is-a-valid-ipv4-address-in-c
http://man7.org/linux/man-pages/man3/gethostbyname.3.html
