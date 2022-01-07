
#include "../include/TCPHandler.hpp"

int TCPHandler::Socket_create()
{
    int sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock == -1)
    {
		NetworkException e("Could not create socket");
		throw e;
    }
	cout << "TCPHandler: Socket created." << endl;
	return sock;
}

void TCPHandler::Socket_close(int sock)
{
	close(sock);
	cout << "TCPHandler: Socket closed" << endl;
}

struct sockaddr_in TCPHandler::Gen_addr(short sin_fam, unsigned short port, string addr)
{
	struct sockaddr_in server;
    server.sin_family = sin_fam;
    server.sin_addr.s_addr = (addr == "0") ? INADDR_ANY : inet_addr(addr.c_str());
    server.sin_port = htons(port);
	return server;
}

int TCPHandler::Bind(int sock, struct sockaddr_in server)
{
    if(bind(sock,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
       	NetworkException e("Bind failed"); 
		throw e;
    }
    cout << "TCPHandler: Binding done" << endl;
	return sock;
}

void TCPHandler::Listen(int sock)
{
	listen(sock, LISTEN_MAX);
    cout << "TCPHandler: Server is listening" << endl;
}

void TCPHandler::Connect(int sock, struct sockaddr_in addr)
{
    if(connect(sock , (struct sockaddr *)&addr , sizeof(addr)) < 0)
    {
		NetworkException e("Connection with remote server failed");
		throw e;
    }
	cout << "TCPHandler: Client connected." << endl;
}

int TCPHandler::Accept(int socket_desc, struct sockaddr_in addr)
{
    int c = sizeof(struct sockaddr_in);

    //accept connection from an incoming client
    int client_sock = accept(socket_desc, (struct sockaddr *)&addr, (socklen_t*)&c);
    if (client_sock < 0)
    {
		NetworkException e("Server could not accept connection from client");
		throw e;
    }
    cout << "TCPHandler: Server accepted the connection" << endl;
	return client_sock;
}

int TCPHandler::Send(int sock, string message, int len, int flag)
{
    if(send(sock , message.c_str(), len, flag) < 0)
    {
		NetworkException e("Failed to send message");
		throw e;
    }
	return 0;
}

int TCPHandler::Recieve(int sock, char* message, int flag)
{
	//TODO figure out how to print when the client had disconnected
    if(recv(sock , message , DEFAULT_BUFLEN , flag) > 0 )
    {
		return 0;
    }
	return 1;
}

