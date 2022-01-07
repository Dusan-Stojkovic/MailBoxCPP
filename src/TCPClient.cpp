
#include "../include/TCPClient.hpp"

TCPClient::TCPClient(string addr)
{
	sock = Socket_create(); 
	server = Gen_addr(AF_INET, DEFAULT_PORT, addr.c_str());
	Connect(sock, server);
}

int TCPClient::Send(string message)
{
	return TCPHandler::Send(sock, message, (int)message.length(), 0);	
}
