
#ifndef __TCPSERVER__
#define __TCPSERVER__

#include<vector>

#include"TCPHandler.hpp"

typedef struct client_sock_st
{
	int id;
	int sock;
	struct sockaddr_in client;
}client_sock;

class TCPServer : protected TCPHandler
{
	private:
		int sock_server;
		struct sockaddr_in server;
		//I don't like this rn; will change it added just as a test
		vector<client_sock> conn_clients;
	public:
		TCPServer();
		~TCPServer() { }

		string Recieve();
};

#endif
