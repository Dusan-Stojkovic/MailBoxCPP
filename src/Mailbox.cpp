
#include "../include/Mailbox.hpp"

Mailbox::Mailbox() : server(AF_INET, DEFAULT_PORT, "0")
{
	cout << "Mailbox: Starting..." << endl;
	conn_clients = vector<Socket>();
	
	//Load logins 
}

Mailbox::Mailbox(const Mailbox& m)
{
	this->conn_clients = m.conn_clients;
	this->server = TCPServer(m.server);	
}

void Mailbox::logins_create()
{

}

void Mailbox::client_handler(Mailbox& m, Socket client, string id)
{
	string msg = "";
	while((msg = m.server.Recieve(client)) != "")
	{
		cout << "Mailbox: New message from " << id << ": " << msg << endl;

		//operation table test
		size_t delim = msg.find(" ");
		string command = msg.substr(0, delim);
		//cout << command << endl;

		auto f = operationTable.find(command)->second; 
		if(f == NULL)
			m.server.Send(client.sock, "Mailbox: Bad message.");	
		else
		{
			m.server.Send(client.sock, "Mailbox: Message recieved.");
			f(client, msg);
		}
		msg = "";
	}
	
	//our Client has to be erased now
	vector<Socket>::iterator it = m.conn_clients.begin();
	while(it != m.conn_clients.end())
	{
		if((*it).sock == client.sock)
		{
			m.conn_clients.erase(it);
			break;
		}

		++it;
	}
	cout << "Mailbox: " << id << " disconnected." << endl;
	cout << "\t number of online clients is now: " << m.conn_clients.size() << endl;
}

void Mailbox::Eval(Mailbox& m)
{
	string clientID;
	Socket* client;
	vector<thread> t;
	int i = 0;
	cout << "Mailbox: Now accepting new connections." << endl;
	while(m.conn_clients.size() < LISTEN_MAX)
	{
		client = new Socket();
		m.server.Accept(client);	
		m.conn_clients.push_back(*client);

		clientID = "Guest" + to_string(i++);
		t.push_back(thread(Mailbox::client_handler, ref(m), *client, ref(clientID)));
		t[t.size() - 1].detach();
	}
	throw NetworkException("Mailbox: Too many connections.");
}

int Mailbox::Forward(Socket client, string data)
{
	//TODO implement from specification
	cout << "Test I'm working!" << endl;
	return 0;
}

int Mailbox::Update(Socket client, string data)
{
	//TODO implement from specification
	return 0;
}

int Mailbox::Status(Socket client, string data)
{
	//TODO implement from specification
	return 0;
}

int Mailbox::Login(Socket client, string data)
{
	//TODO implement from specification
	return 0;
}

int Mailbox::Logout(Socket client, string data)
{
	//TODO implement from specification
	return 0;
}

int Mailbox::Register(Socket client, string data)
{
	//TODO implement from specification
	return 0;
}

const string operations[] = 
{
	"SEND",
	"RECIEVE",
	"CHECK",
	"LOGIN",
	"LOGOUT",
	"REGISTER",
	"EXIT"
};

const map<string, function<int(Socket, string)>> Mailbox::operationTable 
{
	{ operations[0], Mailbox::Forward }, 
	{ operations[1], Mailbox::Update }, 
	{ operations[2], Mailbox::Status }, 
	{ operations[3], Mailbox::Login }, 
	{ operations[4], Mailbox::Logout }, 
	{ operations[5], Mailbox::Register }, 
};
