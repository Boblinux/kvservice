LIB_SRC = 			\
	Acceptor.cc		\
	Buffer.cc		\
	Channel.cc		\
	EPoller.cc		\
	EventLoop.cc		\
	EventLoopThread.cc	\
	EventLoopThreadPool.cc	\
	InetAddress.cc		\
	Poller.cc		\
	Socket.cc		\
	SocketsOps.cc		\
	TcpServer.cc		\
	TimerQueue.cc		\
	Timer.cc		\
	Thread.cc	\
	Timestamp.cc \
	TcpConnection.cc	
#	TcpClient.cc		\
	Connector.cc		\		

#BINARIES = test9 test10 test11 test12 test13
BINARIES = test9
CC = g++
CXXFLAGS += -std=c++11 -pthread -Wl,--no-as-needed
all: $(BINARIES)

test9: test9.cc $(LIB_SRC)
#echo: echo.cc $(LIB_SRC)
#test10: test10.cc $(LIB_SRC)
#test11: test11.cc $(LIB_SRC)
#test12: test12.cc $(LIB_SRC)
#test13: test13.cc $(LIB_SRC)

