
#include "../include/TCPClient.hpp"

TCPClient::TCPClient(string addr)
{
	client = Socket_create(AF_INET, DEFAULT_PORT, addr.c_str()); 
	Connect(client);
}

int TCPClient::Send(string message)
{
	return TCPHandler::Send(client.sock, message.c_str(), (int)message.length(), 0);	
}

string TCPClient::Receive()
{
	char msg[DEFAULT_BUFLEN] = { 0 };
	TCPHandler::Receive(client.sock, msg, 0);
	return string(msg);
}
