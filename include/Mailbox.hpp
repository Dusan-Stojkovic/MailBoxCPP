
#ifndef __MAILBOXSERVER__
#define __MAILBOXSERVER__

#include<algorithm>
#include<map>
#include<vector>
#include<mutex>
#include<functional>
#include<thread>

#include"TCPServer.hpp"
//mutex mx;
class Mailbox 
{
	private:
		vector<Socket> conn_clients;
		TCPServer server;

		static const map<string, function<int(Mailbox&, Socket, string, string&)>> operationTable;
		
		void logins_create();

		static void client_handler(Mailbox&, Socket, string);
	public:
		//mutex mx;
		//static string BOX;
		//static int rbr;
		Mailbox();
		Mailbox(const Mailbox& m);
		Mailbox& operator=(const Mailbox& rhs) { return *this; };

		//vector<Socket> getClients() { return conn_clients; }
		//void addClient(const Socket& s) { conn_clients.push_back(s); }

		//TCPServer getServer() { return server; }

		static void Eval(Mailbox&);

		//Send opeartion, server side
		static int Forward(Mailbox&, Socket, string, string&);

		//Receive operation, server side
		static int Update(Mailbox&, Socket, string, string&);

		//Check operation, server side
		static int Status(Mailbox&, Socket, string, string&);

		//Login operation, server side
		static int Login(Mailbox&, Socket, string, string&);

		//Logout operation, server side
		static int Logout(Mailbox&, Socket, string, string&);
		
		//Register operation, server side
		static int Register(Mailbox&, Socket, string, string&);
};


#endif
