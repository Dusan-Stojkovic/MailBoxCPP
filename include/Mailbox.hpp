
#ifndef __MAILBOXSERVER__
#define __MAILBOXSERVER__

#include<algorithm>
#include<map>
#include<vector>
#include<mutex>
#include<functional>
#include<thread>

#include"TCPServer.hpp"

class Mailbox 
{
	private:
		vector<Socket> conn_clients;
		TCPServer server;

		static const map<string, function<int(Socket, string)>> operationTable;

		void logins_create();
		
		static void client_handler(Mailbox&, Socket, string);
	public:
		mutex mx;

		Mailbox();
		Mailbox(const Mailbox& m);
		Mailbox& operator=(const Mailbox& rhs) { return *this; };

		//vector<Socket> getClients() { return conn_clients; }
		//void addClient(const Socket& s) { conn_clients.push_back(s); }

		//TCPServer getServer() { return server; }

		static void Eval(Mailbox&);

		//Send opeartion, server side
		static int Forward(Socket, string);

		//Recieve operation, server side
		static int Update(Socket, string);

		//Check operation, server side
		static int Status(Socket, string);

		//Login operation, server side
		static int Login(Socket, string);

		//Logout operation, server side
		static int Logout(Socket, string);
		
		//Register operation, server side
		static int Register(Socket, string);
};


#endif
