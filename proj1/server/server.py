class Server:
    def __init__(self, host, port):
        self.host = host
        self.port = port

        self.start()

    def start(self):
        print("starting")
