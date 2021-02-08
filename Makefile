main:
	g++ server.cpp -o server
	g++ server2.cpp -o server2
	g++ server3.cpp -o server3
	g++ client.cpp -o client


clean:                 
	rm -rf server client server2 server3

