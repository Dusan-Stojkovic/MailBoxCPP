
#include"../include/TCPClient.hpp"
#include <signal.h>
#include <string.h>
void sigHandler(int sig_num)
{
	signal(SIGINT, sigHandler);
	cin.unget();
	cout << "Can't terminate this program using CTRL + C, try EXIT" << endl;
}

int main()
{
	string msg = "";
	string command;
	string response;
	int index;
	
	try
	{
		TCPClient client("127.0.0.1");
		signal(SIGINT, sigHandler);
		while(getline(cin, msg))
		{
			//signal(SIGINT, sigHandler);
			if(msg == "EXIT")
			{
				return 0;
			}
			client.Send(msg);
			cout << client.Receive() << endl;
			index=msg.find(" ");
			command=msg.substr(0,index);
			if(command=="RECEIVE"){
				response=client.Receive();
				cout<<response<<endl;
			}
			if(command=="CHECK"){
				response=client.Receive();
				cout<<response<<endl;
			}
			msg = "";
		}
	}
	catch(NetworkException& ex)
	{
		cout << "Network Error: " << ex.what() << endl;
		return 1;
	}
	catch(exception& ex)
	{
		cout << ex.what() << endl;
		return 2;
	}
	
	return 0;
}

