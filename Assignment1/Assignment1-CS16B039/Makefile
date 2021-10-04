all : clien server srclient srserver
clien:
	g++ GBNC.cpp -o clien -std=c++0x -pthread
server:
	g++ server.cpp -o server -std=c++0x
srclient:
	g++ srclient.cpp -o srclient -std=c++0x -pthread
srserver:	
	g++ srserver.cpp -o srserver -std=c++0x

