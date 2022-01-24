
#include "../include/Mailbox.hpp"
#include <fstream>

static int guestID=0;
map <string,string> MailBox;
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
	while((msg = m.server.Receive(client)) != "")
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
			m.server.Send(client.sock, "Mailbox: Message received.");
			//this_thread::sleep_for(chrono::milliseconds(10) );
			usleep(10000);//bez sleep-a se desi da zaglavi pri primanju poruka jer ovu i sledecu poruku primi kao jednu
			f(ref(m), client, msg,ref(id));
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
	//int i = 0;
	cout << "Mailbox: Now accepting new connections." << endl;
	while(m.conn_clients.size() < LISTEN_MAX)
	{
		client = new Socket();
		m.server.Accept(client);	
		m.conn_clients.push_back(*client);

		clientID = "Guest" + to_string(guestID++);
		t.push_back(thread(Mailbox::client_handler, ref(m), *client, ref(clientID)));
		t[t.size() - 1].detach();
	}
	throw NetworkException("Mailbox: Too many connections.");
}

int Mailbox::Forward(Mailbox& m, Socket client, string data, string& id)//SEND user_id data
{
	//TODO implement from specification
	//mx.lock();
	//cout << "Test I'm working!" << endl;
	//cout <<data<<endl;
	string temp_box;
	string id_chc=id.substr(0,5);
	if(id_chc=="Guest"){
		cout<<"You must login first to send data!"<<endl;
		return 1;
	}
	int space=data.find(" ");
	string pom=data.substr(space+1);//ostaje _user, data_
	space=pom.find(" ");
	string receiver=pom.substr(0,space);//ovde se nalazi id primaoca
	pom=pom.substr(space+1);//ovde se nalaze podaci
	if(pom=="" || pom=="SEND"){
		cout<<"Bad entry!"<<endl;
		return 2;
	}
	pom=pom+" Sent_By:"+id;
	//BOX=BOX+"msg "+to_string(rbr)+": "+pom+" ";
	auto user=MailBox.find(receiver);
	if(user != MailBox.end()){
		//ima napravljen inbox
		temp_box=user->second;
		temp_box=temp_box+"\n"+pom;
		user->second=temp_box;
		cout<<"Mailbox updated!"<<endl;
	}else{
		MailBox.insert(pair<string, string>(receiver, pom));
		cout<<"New user, mailbox added."<<endl;
	}
	//mx.unlock();
	return 0;
}

int Mailbox::Update(Mailbox& m, Socket client, string data, string& id)//RECEIVE 
{
	//TODO implement from specification
	string id_chc=id.substr(0,5);
	if(id_chc=="Guest"){
		cout<<"You must login first to receive data!"<<endl;
		return 1;
	}
	string temp_box;
	auto user=MailBox.find(id);
	if(user != MailBox.end()){
		//ima napravljen inbox
		temp_box=user->second;
		if(temp_box==""){
			m.server.Send(client.sock, "Mailbox is empty!");
			return 10;
		}
		m.server.Send(client.sock, temp_box);
		user->second="";
		cout<<"Mailbox returned to the client and its now empty!"<<endl;
	}else{
		cout<<"Mailbox for this user not found, it didn't receive any messages!"<<endl;
		m.server.Send(client.sock, "Mailbox for this user not found, it didn't receive any messages!");
	}
	return 0;
}

int Mailbox::Status(Mailbox& m, Socket client, string data, string& id)//CHECK 
{
	//TODO implement from specification
	string id_chc=id.substr(0,5);
	if(id_chc=="Guest"){
		cout<<"You must login first to check your data!"<<endl;
		return 1;
	}
	string temp_box="";
	auto user=MailBox.find(id);
	if(user != MailBox.end()){
		//ima napravljen inbox
		temp_box=user->second;
	}else{
		cout<<"CHECK din't find mailbox since it has not been created yet!"<<endl;
	}

	if(temp_box==""){
		m.server.Send(client.sock, "Inbox empty!");
	}else{
		m.server.Send(client.sock, "Inbox contains data!");
	}
	return 0;
}

int Mailbox::Login(Mailbox& m, Socket client, string data, string& id)//LOGIN username password
{
	//TODO implement from specification
	string username,password;
	string user_file,pass_file;
	string id_chc;
	int space=data.find(" ");
	data.erase(0,space+1);//uklanjam komandu jer nam je visak
	space=data.find(" ");
	username=data.substr(0,space);
	password=data.substr(space+1);
	//cout<<username<<" "<<password<<endl;//debug line
	if(username=="" || password=="" || username=="LOGIN" || password=="LOGIN"){
		cout<<"You can't hack this :-P"<<endl;
		return 2;
	}
	id_chc=id.substr(0,5);
	if(id_chc!="Guest"){
		cout<<"Already logged in, logout first!"<<endl;
		return 1;
	}
	
	string line;
  	ifstream user ("users.txt");
  	if (user.is_open())
  	{
    	while ( getline (user,line) )
    	{
			space=line.find(" ");
			user_file=line.substr(0,space);
			pass_file=line.substr(space+1);
      		//cout << line << '\n';
			if(user_file==username){
				cout<<"User found!"<<endl;
				if(pass_file==password){
					//cout<< password<<" "<<pass_file<<endl;
					id=username;
					cout<<"User is logged in!"<<endl;
					return 0;
				}else{
					cout<<"Incorrect password!"<<endl;
					return 1;
				}
			}
    	}
		if(username!=id){
			cout<<"Username not found!"<<endl;
		}
    user.close();
  	}
	else {
		cout << "Unable to open file";
	} 
	
	return 0;
}

int Mailbox::Logout(Mailbox& m, Socket client, string data, string& id)//LOGOUT
{
	//TODO implement from specification
	string id_chc=id.substr(0,5);
	if(id_chc=="Guest"){
		cout<<"You are not logged in!"<<endl;
		return 1;
	}
	id="Guest"+to_string(guestID++);
	cout<<"Client disconnected, now it's a guest: "<<id<<endl;
	return 0;
}

int Mailbox::Register(Mailbox& m, Socket client, string data, string& id)//REGISTER username password
{
	//TODO implement from specification
	string username,password;
	string id_chc;
	int space=data.find(" ");
	data.erase(0,space+1);//uklanjam komandu jer nam je visak
	space=data.find(" ");
	username=data.substr(0,space);
	password=data.substr(space+1);
	if(username=="" || password=="" || username=="REGISTER" || password=="REGISTER"){
		cout<<"Wrong format!"<<endl;
		return 2;
	}
	id_chc=id.substr(0,5);
	if(id_chc!="Guest"){
		cout<<"Already logged in into current account, logout first!"<<endl;
		return 1;
	}

	string line;
	fstream users ("users.txt", fstream::app);
  	if (users.is_open())
  	{
    	users << username<<" "<<password <<"\n";
    	users.close();
		cout<<"New user added!"<<endl;
  	}else{ 
		cout << "Unable to open file";
	}
	return 0;
}

const string operations[] = 
{
	"SEND",
	"RECEIVE",
	"CHECK",
	"LOGIN",
	"LOGOUT",
	"REGISTER",
	"EXIT"
};

const map<string, function<int(Mailbox&, Socket, string, string&)>> Mailbox::operationTable 
{
	{ operations[0], Mailbox::Forward }, 
	{ operations[1], Mailbox::Update }, 
	{ operations[2], Mailbox::Status }, 
	{ operations[3], Mailbox::Login }, 
	{ operations[4], Mailbox::Logout }, 
	{ operations[5], Mailbox::Register }, 
};
