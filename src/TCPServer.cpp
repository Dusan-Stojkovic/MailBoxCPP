#include"../include/TCPServer.hpp"

TCPServer::TCPServer(short sin_fam, unsigned short port, string addr)
{
	server = Socket_create(sin_fam, port, "0");
	Bind(server);
	Listen(server.sock);
}

void TCPServer::Accept(Socket* client)
{
	TCPHandler::Accept(server.sock, client);
}

string TCPServer::Recieve(Socket client)
{
	char msg[DEFAULT_BUFLEN] = { 0 };
	TCPHandler::Recieve(client.sock, msg, 0);
	return string(msg);
}

//s is client socket, from mailbox
int TCPServer::Send(int s, string msg)
{
	return TCPHandler::Send(s, msg, (int)msg.length(), 0);	
}
