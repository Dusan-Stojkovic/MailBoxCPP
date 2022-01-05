
#include<iostream>      //printf
#include<string>     //strlen
#include<sys/socket.h> //socket
#include<arpa/inet.h>  //inet_addr
#include<unistd.h>    //for close

using namespace std;

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT   27015

int main(int argc , char *argv[])
{
    int sock;
    struct sockaddr_in server;
    string message = "this is a test";

    //Create socket
    sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock == -1)
    {
		cerr << "Erro: could not create socket" << endl;
    }
	cout << "Socket created." << endl;

    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons(DEFAULT_PORT);

    //Connect to remote server
    if(connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
		cerr << "Error: connection failed." << endl;
        return 1;
    }

	cout << "Connected." << endl;

    //Send some data
    if(send(sock , message.c_str() , message.length(), 0) < 0)
    {
		cerr << "Error: sending failed." << endl;
        return 1;
    }

    cout << "Message: " << message << endl;

    close(sock);

    return 0;
}

