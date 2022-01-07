
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
		int Socket_create();
		void Socket_close(int);
		struct sockaddr_in Gen_addr(short, unsigned short, string);
		int Bind(int, struct sockaddr_in);
		void Listen(int);
		int Accept(int, struct sockaddr_in); 
		void Connect(int, struct sockaddr_in);
		int Send(int, string, int, int);
		int Recieve(int, char*, int);
};

#endif
