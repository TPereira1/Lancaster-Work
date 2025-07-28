#!/usr/bin/env python3
# -*- coding: UTF-8 -*-

import argparse
import select
import socket
import os
import sys
import struct
import time
import random
import traceback # useful for exception handling
import threading
# NOTE: Do not import any other modules - the ones above should be sufficient

def setupArgumentParser() -> argparse.Namespace:
        parser = argparse.ArgumentParser(
            description='A collection of Network Applications developed for SCC.203.')
        parser.set_defaults(func=ICMPPing, hostname='lancaster.ac.uk')
        subparsers = parser.add_subparsers(help='sub-command help')
        
        parser_p = subparsers.add_parser('ping', aliases=['p'], help='run ping')
        parser_p.set_defaults(timeout=2, count=10)
        parser_p.add_argument('hostname', type=str, help='host to ping towards')
        parser_p.add_argument('--count', '-c', nargs='?', type=int,
                              help='number of times to ping the host before stopping')
        parser_p.add_argument('--timeout', '-t', nargs='?',
                              type=int,
                              help='maximum timeout before considering request lost')
        parser_p.set_defaults(func=ICMPPing)

        parser_t = subparsers.add_parser('traceroute', aliases=['t'],
                                         help='run traceroute')
        parser_t.set_defaults(timeout=2, protocol='icmp')
        parser_t.add_argument('hostname', type=str, help='host to traceroute towards')
        parser_t.add_argument('--timeout', '-t', nargs='?', type=int,
                              help='maximum timeout before considering request lost')
        parser_t.add_argument('--protocol', '-p', nargs='?', type=str,
                              help='protocol to send request with (UDP/ICMP)')
        parser_t.set_defaults(func=Traceroute)
        
        parser_w = subparsers.add_parser('web', aliases=['w'], help='run web server')
        parser_w.set_defaults(port=8080)
        parser_w.add_argument('--port', '-p', type=int, nargs='?',
                              help='port number to start web server listening on')
        parser_w.set_defaults(func=WebServer)

        parser_x = subparsers.add_parser('proxy', aliases=['x'], help='run proxy')
        parser_x.set_defaults(port=8000)
        parser_x.add_argument('--port', '-p', type=int, nargs='?',
                              help='port number to start web server listening on')
        parser_x.set_defaults(func=Proxy)

        args = parser.parse_args()
        return args

class NetworkApplication:

    def checksum(self, dataToChecksum: bytes) -> int:
        csum = 0
        countTo = (len(dataToChecksum) // 2) * 2
        count = 0

        while count < countTo:
            thisVal = dataToChecksum[count+1] * 256 + dataToChecksum[count]
            csum = csum + thisVal
            csum = csum & 0xffffffff
            count = count + 2

        if countTo < len(dataToChecksum):
            csum = csum + dataToChecksum[len(dataToChecksum) - 1]
            csum = csum & 0xffffffff

        csum = (csum >> 16) + (csum & 0xffff)
        csum = csum + (csum >> 16)
        answer = ~csum
        answer = answer & 0xffff
        answer = answer >> 8 | (answer << 8 & 0xff00)

        answer = socket.htons(answer)

        return answer

    def printOneResult(self, destinationAddress: str, packetLength: int, time: float, seq: int, ttl: int, destinationHostname=''):
        if destinationHostname:
            print("%d bytes from %s (%s): icmp_seq=%d ttl=%d time=%.3f ms" % (packetLength, destinationHostname, destinationAddress, seq, ttl, time))
        else:
            print("%d bytes from %s: icmp_seq=%d ttl=%d time=%.3f ms" % (packetLength, destinationAddress, seq, ttl, time))

    def printAdditionalDetails(self, packetLoss=0.0, minimumDelay=0.0, averageDelay=0.0, maximumDelay=0.0):
        print("%.2f%% packet loss" % (packetLoss))
        if minimumDelay > 0 and averageDelay > 0 and maximumDelay > 0:
            print("rtt min/avg/max = %.2f/%.2f/%.2f ms" % (minimumDelay, averageDelay, maximumDelay))

    def printOneTraceRouteIteration(self, ttl: int, destinationAddress: str, measurements: list, destinationHostname=''):
        latencies = ''
        noResponse = True
        for rtt in measurements:
            if rtt is not None:
                latencies += str(round(rtt, 3))
                latencies += ' ms  '
                noResponse = False
            else:
                latencies += '* ' 

        if noResponse is False:
            print("%d %s (%s) %s" % (ttl, destinationHostname, destinationAddress, latencies))
        else:
            print("%d %s" % (ttl, latencies))

class ICMPPing(NetworkApplication):

    def receiveOnePing(self, icmpSocket, destinationAddress, ID, timeout):
            # 1. Wait for the socket to receive a reply
            while True: 
                wait = select.select([icmpSocket],[],[],timeout)
                # 2. If reply received, record time of receipt, otherwise, handle timeout 
                recvTime = time.time() 
                if wait[0] == []:
                    return
                response,addr = icmpSocket.recvfrom(1024)
                # 3. Unpack the imcp and ip headers for useful information, including Identifier, TTL, sequence number 
                icpm_header = struct.unpack("BBHHH",response[20:28])
                ttl = struct.unpack("b",response[8:9])
                total_length = struct.unpack("!H",response[2:4])
                # 5. Check that the Identifier (ID) matches between the request and reply
                if(icpm_header[3]  == ID):
                    # 6. Return time of receipt, TTL, packetSize, sequence number
                    return (recvTime,ttl[0],total_length[0],icpm_header[4])  

    def sendOnePing(self, icmpSocket, destinationAddress, ID, seq_num):
        # 1. Build ICMP header
        header = struct.pack("BBHHH",8,0,0,ID,seq_num)
        pad = b'p'*36
        # 2. Checksum ICMP packet using given function
        checksum = self.checksum(header+pad)
        # 3. Insert checksum into packet
        header = struct.pack("BBHHH",8,0,checksum, ID, seq_num)
        # 4. Send packet using socket
        icmpSocket.sendto(header+pad,(destinationAddress,0))
        # 5. Return time of sending
        return time.time()

    def doOnePing(self, destinationAddress, packetID, seq_num, timeout):
        # 1. Create ICMP socket
        s = socket.socket(socket.AF_INET,socket.SOCK_RAW,socket.IPPROTO_ICMP)
        # 2. Call sendOnePing function
        startTime = self.sendOnePing(s, destinationAddress, packetID,seq_num)
        # 3. Call receiveOnePing function
        response = self.receiveOnePing(s, destinationAddress, packetID, timeout)
        # 4. Close ICMP socket
        s.close
        # 5. Print out the delay (and other relevant details) using the printOneResult method, below is just an example.
        self.printOneResult(destinationAddress, response[2], (response[0]-startTime)*1000, response[3], response[1])


    def __init__(self, args):   
        print('Ping to: %s...' % (args.hostname))     
        # 1. Look up hostname, resolving it to an IP address
        ipAddress = socket.gethostbyname(args.hostname)
        # 2. Repeat below args.count times
        for i in range(args.count):
            # 3. Call doOnePing function, approximately every second, below is just an example
            self.doOnePing(ipAddress, 256, i+1, 1)
            time.sleep(1)

class Traceroute(NetworkApplication):

    def sendOneICMPTraceRoute(self, icmpSocket, destinationAddress, ID):
        header = struct.pack("BBHHH",8,0,0,ID,1)
        pad = b'p'*36
        checksum = self.checksum(header+pad)
        header = struct.pack("BBHHH",8,0,checksum, ID, 1)
        icmpSocket.sendto(header+pad,(destinationAddress,0))
        return time.time()
    
    def sendOneUDPTraceRoute(self, udpSocket, destinationAddress, ID):
        udpSocket.sendto(b"trace", (destinationAddress, 33434))
        return time.time()
    
    def receiveOneTraceRoute(self, icmpSocket, timeout, startTime):
            while True: 
                wait = select.select([icmpSocket],[],[],timeout)
                recvTime = time.time()
                if (wait[0] == [] and timeout < recvTime - startTime):
                    return 
                response,addr = icmpSocket.recvfrom(1024)
                sourceAdress = struct.unpack("BBBB",response[12:16])
                sourceIP = ("%d.%d.%d.%d" % (sourceAdress[0],sourceAdress[1],sourceAdress[2],sourceAdress[3]))
                icpm_header = struct.unpack("BBHHH",response[20:28])
                return ((recvTime - startTime)*1000, sourceIP)
                
    def __init__(self, args):
        ipAddress = socket.gethostbyname(args.hostname)
        print('Traceroute to: %s...' % (args.hostname))
        # Create a socket
        if args.protocol == "icmp":
            sendSocket = socket.socket(socket.AF_INET,socket.SOCK_RAW,socket.IPPROTO_ICMP)
        elif args.protocol == "udp":
            sendSocket = socket.socket(socket.AF_INET,socket.SOCK_DGRAM,socket.IPPROTO_UDP)
        else: 
            print("Unknown protocol")
            exit()
        recvSocket = socket.socket(socket.AF_INET,socket.SOCK_RAW,socket.IPPROTO_ICMP)
        arrived = False
        # Increase TTL up to set range 30hops max at the moment
        for i in range (30):
            # Set TTL
            sendSocket.setsockopt(socket.IPPROTO_IP, socket.IP_TTL, i+1)
            arrivalTime = []
            destinationAddress = ""
            
            for x in range(3):
                # Send one traceroute
                if args.protocol == "icmp":
                    startTime = self.sendOneICMPTraceRoute(sendSocket, ipAddress, 256)
                else:
                    startTime = self.sendOneUDPTraceRoute(sendSocket, ipAddress, 256)
                # Recieve one traceRote
                response = self.receiveOneTraceRoute(recvSocket, args.timeout, startTime)
                #handles no responces 
                if(response != None):
                    if(response[1] == ipAddress):
                        arrived = True
                    destinationAddress = response[1]
                    arrivalTime.append(response[0])
                else:
                    arrivalTime.append(response)
                hostname = ""
                try:
                    hostname = socket.gethostbyaddr(destinationAddress)
                except:
                    hostname = destinationAddress
            
            self.printOneTraceRouteIteration(i+1,destinationAddress,arrivalTime, hostname)
            if(arrived):
                break
        # close sockets
        sendSocket.close()
        recvSocket.close()

class WebServer(NetworkApplication):

    def handleRequest(self, tcpSocket):
        # 1. Receive request message from the client on connection socket
        data = tcpSocket.recv(1024)
        # 2. Extract the path of the requested object from the message (second part of the HTTP header)
        filename = data.split()[1]
        try:
            # 3. Read the corresponding file from disk
            fileToRead = open(filename[1:].decode("utf-8"),"r")
            # 4. Store in temporary buffer
            output = fileToRead.read()
            # 5. Send the correct HTTP response error
            tcpSocket.send("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n".encode('utf-8'))
            # 6. Send the content of the file to the socket
            tcpSocket.sendall(output.encode('utf-8'))
        except:
            tcpSocket.send("HTTP/1.1 404 Not Found\r\n\r\n".encode('utf-8'))
        # 7. Close the connection socket
        tcpSocket.close()

    def __init__(self, args):
        print('Web Server starting on port: %i...' % (args.port))
        # 1. Create server socket
        serverSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        # 2. Bind the server socket to server address and server port
        serverSocket.bind(("127.0.0.1",args.port))
        # 3. Continuously listen for connections to server socket
        serverSocket.listen(1)
        # 4. When a connection is accepted, call handleRequest function, passing new connection socket (see https://docs.python.org/3/library/socket.html#socket.socket.accept)
        while True:
            (clientSocket,clientAdress)= serverSocket.accept()
            if(clientSocket != -1):
                self.handleRequest(clientSocket)
        # 5. Close server socket
        serverSocket.close()

class Proxy(NetworkApplication):
    cache = {}

    def readCache(self):
        cacheDir = "Cache"
        if(os.path.exists(cacheDir)):
            fileList = os.listdir(cacheDir)
            for i in fileList:
                self.cache[i.replace(".html","")] = "Cache/"+i

    def gethost(self, request):
        hostnameStartIndex = request.find('Host: ')+6
        host = request[hostnameStartIndex:].split()[0]
        return host
    
    def createCache(self,url,data):
        cacheDir = "Cache"
        if(not os.path.exists(cacheDir)):
            os.mkdir(cacheDir)

        cd = cacheDir+"/"+url+".html"
        cache = open(cd,"wb")
        cache.write(data)
        cache.close()
        self.cache[url] = cd


    def getPageFromServer(self, tcpSocket, clientData, hostname, url):
        # Create socket to server
        hostSocket = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
        hostSocket.connect((socket.gethostbyname(hostname),80))
        # Send client request to server
        hostSocket.sendall(clientData)
        # Recieve data from server
        storeData = b""
        while True:
            sdata = hostSocket.recv(1024)
            if len(sdata) <= 0:
                break
            else:
                # Send data to client
                tcpSocket.sendall(sdata)
                storeData += sdata
        self.createCache(url,storeData)
        # 7. Close the connection to server socket
        hostSocket.close()

    def getFromCache(self, tcpSocket, request):
        if(request in self.cache):
            page = open(self.cache[request],"rb")
            tcpSocket.sendall(page.read())
            return True
        return False   
        
    def handleRequest(self, tcpSocket):
        # Receive request message from the client on connection socket
        try:
            clientData = tcpSocket.recv(1024)
            # Extract the host name
            clientRequest = clientData.decode('utf-8')
            requestPage = (clientData.split()[1]).decode().replace("/","")
            itemInCache = self.getFromCache(tcpSocket,requestPage)
            if(itemInCache == False):
                print("server")
                hostname = self.gethost(clientRequest)
                # Request page from server
                self.getPageFromServer(tcpSocket, clientData, hostname, requestPage)
            else:
                print("cache")
        except:
            tcpSocket.send("HTTP/1.1 404 Not Found\r\n\r\n".encode('utf-8'))
        # Close the connection to client socket
        tcpSocket.close()

    def __init__(self, args):
        print('Web Proxy starting on port: %i...' % (args.port))
        self.readCache()
        # web proxy socket for clients to connect to
        server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        #server.setsockopt(socket.SOL_SOCKET,socket.SO_REUSEPORT,1)
        # binding proxy
        server.bind(('127.0.0.1', args.port))
        # listening for multiple clients
        server.listen(10)
        try:
            while True:
                # Accepting client connection
                (clientSocket,clientAdress)= server.accept()
                self.handleRequest(clientSocket)
        except:
            # close socket
            server.close()
        finally:
            # close socket
            server.close()
    

# Do not delete or modify the code below
if __name__ == "__main__":
    args = setupArgumentParser()
    args.func(args)
