
#ifndef __TCPClient__
#define __TCPClient__

#include "TCPHandler.hpp"

class TCPClient : protected TCPHandler 
{
	private:
		Socket client;

	public:
		TCPClient() { }
		~TCPClient() { Socket_close(client.sock); }

		//After this you should be able to send data and recieve it
		TCPClient(string);
		int Send(string);	
		string Recieve();
};

#endif
