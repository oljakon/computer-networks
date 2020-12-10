all: client server

client: client.cpp
	g++ -pthread client.cpp -o client

server: server.o thread_pool.o
	g++ -pthread -o server server.o thread_pool.o

server.o: server.cpp thread_pool.hpp
	g++ -c server.cpp

thread_pool.o: thread_pool.cpp thread_pool.hpp
	g++ -c thread_pool.cpp
