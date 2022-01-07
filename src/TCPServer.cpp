#include"../include/TCPServer.hpp"

TCPServer::TCPServer()
{
	conn_clients = vector<client_sock>();
	sock_server = Socket_create();
	server = Gen_addr(AF_INET, DEFAULT_PORT, "0");
	Bind(sock_server, server);
	Listen(sock_server);

	// Probably need to make Accept in a seperate thread
	// this part is just a test, I will need to refactor it
	client_sock tst;
	tst.id = 0;
	tst.sock = Accept(sock_server, tst.client); 
	conn_clients.push_back(tst);
}

string TCPServer::Recieve()
{
	char msg[DEFAULT_BUFLEN];
	TCPHandler::Recieve(conn_clients[0].sock, msg, 0);
	return string(msg);
}
