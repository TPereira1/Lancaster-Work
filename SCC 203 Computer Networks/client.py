
import socket

 

# Create a TCP based client socket

echoClient =  socket.socket();

 

# Note: No need for bind() call in client sockets...

# Just use the socket by calling connect()

echoClient.connect(("127.0.0.1", 8080));

 

# Send a message

echoClient.send("Learning Python is fun".encode());

 

