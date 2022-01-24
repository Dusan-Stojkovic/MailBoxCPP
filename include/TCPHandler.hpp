
#ifndef __TCPHANDLER__
#define __TCPHANDLER__

#include<exception>
#include<iostream>
#include<sys/socket.h> //socket
#include<arpa/inet.h>  //inet_addr
#include<unistd.h>    //for close

#define LISTEN_MAX 5
#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT   27015

using namespace std;


typedef struct Socket_st
{
	int sock;
	struct sockaddr_in addr;
}Socket;

//TODO Move this into a separate utils folder
class NetworkException : public exception
{
	private:
		string description;

	public:
		NetworkException() { }
		NetworkException(string d) : description(d) { }
  		virtual const char* what() const throw()
  		{
			return description.c_str();
  		}
};

class TCPHandler 
{
	protected:
		Socket Socket_create(short, unsigned short, string);
		void Socket_close(int);

		//Server side
		int Bind(Socket);
		void Listen(int);
		int Accept(int, Socket*); 
		
		//Client side
		void Connect(Socket);

		//Dual channel
		int Send(int, string, int, int);
		int Recieve(int, char*, int);
};

#endif
