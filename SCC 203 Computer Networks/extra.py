def getPageFromServer(self, tcpSocket, clientData, hostname):
        # Create socket to server
        hostSocket = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
        hostSocket.connect((socket.gethostbyname(hostname),80))
        # Send client request to server
        hostSocket.sendall(clientData)
        self.addToCache("url", clientData.decode('utf-8'))
        # Recieve data from server
        while True:
            sdata = hostSocket.recv(1024)
            if len(sdata) <= 0:
                break
            else:
                # Send data to client
                tcpSocket.sendall(sdata)
                # Store data in cache
                self.addToCache('data', sdata.decode('utf-8'))
        # 7. Close the connection to server socket
        hostSocket.close()

def checkCache(self, request):
            cd = os.getcwd()+"\cache.txt"
            if(os.path.exists(cd)):
                cache = open(cd,"r", encoding='utf-8')
                req = "url: "+request.split()[1]
                while True:
                    line = cache.readline()
                    if not line:
                        break
                    index = line.find(req)
                    if index != -1:
                        cache.close()    
                        return True
            else:
                cache = open(cd,"w", encoding='utf-8')
            cache.close()    
            return False   
        
def getDataFromCache(self, tcpSocket, request):
        cd = os.getcwd()+"\cache.txt"
        if(os.path.exists(cd)):
            cache = open(cd,"r", encoding='utf-8')
            req = "url: "+request.split()[1]
            found = False
            while True:
                line = cache.readline()
                if (not line) or (found and line.find("url:") != -1):
                    cache.close()
                    break
                elif found and line.find("url:") == -1:
                    tcpSocket.sendall(line.encode('utf-8'))
                index = line.find(req)
                if index != -1:
                    found = True 

    def handleRequest(self, tcpSocket,cache):
        # Receive request message from the client on connection socket
        clientData = tcpSocket.recv(1024)
        # Extract the host name
        clientRequest = clientData.decode('utf-8')
        itemInCache = self.checkCache(clientRequest)
        if(itemInCache == False):
            hostname = self.gethost(clientRequest)
            # Request page from server
            self.getPageFromServer(tcpSocket, clientData, hostname, cache)
        else:
            self.getDataFromCache(tcpSocket, clientRequest)
        # Close the connection to client socket
        tcpSocket.close()