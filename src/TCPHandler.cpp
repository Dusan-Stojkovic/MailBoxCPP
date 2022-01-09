
#include "../include/TCPHandler.hpp"

Socket TCPHandler::Socket_create(short sin_fam, unsigned short port, string addr)
{
	Socket s;
    s.sock = socket(AF_INET , SOCK_STREAM , 0);
    if (s.sock == -1)
    {
		NetworkException e("Could not create socket");
		throw e;
    }

    s.addr.sin_family = sin_fam;
    s.addr.sin_addr.s_addr = (addr == "0") ? INADDR_ANY : inet_addr(addr.c_str());
    s.addr.sin_port = htons(port);

	cout << "TCPHandler: Socket created." << endl;
	return s;
}

void TCPHandler::Socket_close(int sock)
{
	close(sock);
	cout << "TCPHandler: Socket closed" << endl;
}

int TCPHandler::Bind(Socket server)
{
	sockaddr_in addr = server.addr;
    if(bind(server.sock,(struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
       	NetworkException e("Bind failed"); 
		throw e;
    }
    cout << "TCPHandler: Binding done" << endl;
	return 0;
}

void TCPHandler::Listen(int sock)
{
	listen(sock, LISTEN_MAX);
    cout << "TCPHandler: Server is listening" << endl;
}

void TCPHandler::Connect(Socket client)
{
    if(connect(client.sock , (struct sockaddr *)&(client.addr), sizeof(client.addr)) < 0)
    {
		NetworkException e("Connection with remote server failed");
		throw e;
    }
	cout << "TCPHandler: Client connected." << endl;
}

int TCPHandler::Accept(int socket_desc, Socket* client) 
{
    int c = sizeof(struct sockaddr_in);

    //accept connection from an incoming client
    client->sock = accept(socket_desc, (struct sockaddr *)&(client->addr), (socklen_t*)&c);
    if (client->sock < 0)
    {
		NetworkException e("Server could not accept connection from client");
		throw e;
    }
    cout << "TCPHandler: Server accepted the connection" << endl;
	return 0;
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
	int read_size;
    if((read_size = recv(sock , message , DEFAULT_BUFLEN , flag)) > 0 )
    {
		return read_size;
    }
	if(read_size == 0)
	{
		cout << "TCPHandler: Client disconnected" << endl;
		return 0;
	}
	else
	{
		NetworkException e("Failed to recv");
		throw e;
	}
}

