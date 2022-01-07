
#ifndef __TCPClient__
#define __TCPClient__

#include "TCPHandler.hpp"

class TCPClient : protected TCPHandler 
{
	private:
		int sock;
		struct sockaddr_in server;

	public:
		TCPClient() { }
		~TCPClient() { Socket_close(sock); }

		//After this you should be able to send data and recieve it
		TCPClient(string);
		int Send(string);	
};

#endif
