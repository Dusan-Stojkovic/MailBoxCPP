
#ifndef __TCPSERVER__
#define __TCPSERVER__

#include"TCPHandler.hpp"

class TCPServer : protected TCPHandler
{
	private:
		Socket server;
	public:
		TCPServer() { }
		TCPServer(short, unsigned short, string);
		TCPServer(Socket*);
		TCPServer(const TCPServer& s) { this->server = s.server; }
		~TCPServer() { }
		
		int Send(int, string);
		string Receive(Socket);
		void Accept(Socket*);
};

#endif
