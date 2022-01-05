
#include<iostream>
#include<string>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT   27015

using namespace std;

int main(int argc , char *argv[])
{
    int socket_desc , client_sock , c , read_size;
    struct sockaddr_in server , client;
    char client_message[DEFAULT_BUFLEN];
   
    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        cerr << "Error: could not create socket";
		return 1;
    }
    cout << "Socket created" << endl;

    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(DEFAULT_PORT);

    //Bind
    if(bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        //print the error message
        cerr << "Error: bind failed.";
        return 1;
    }
    cout << "Binding done" << endl;

    //Listen
    listen(socket_desc , 3);

    //Accept and incoming connection
    cout << "Waiting for incoming connections...";
    c = sizeof(struct sockaddr_in);

    //accept connection from an incoming client
    client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
    if (client_sock < 0)
    {
        cerr << "Erro: accept failed";
        return 1;
    }
    cout << "Connection accepted" << endl;

    //Receive a message from client
    while( (read_size = recv(client_sock , client_message , DEFAULT_BUFLEN , 0)) > 0 )
    {
		cout << "Message: " << client_message << endl
			 << "Bytes received: " <<  read_size << endl;
    }

    if(read_size == 0)
    {
		cout << "Client disconnected." << endl;
    }
    else if(read_size == -1)
    {
        cerr << "Error: recv failed" << endl;
		return -1;
    }

    return 0;
}

