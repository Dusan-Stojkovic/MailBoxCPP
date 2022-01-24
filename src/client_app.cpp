
#include"../include/TCPClient.hpp"
#include <signal.h>

void sigHandler(int sig_num)
{
	signal(SIGINT, sigHandler);
	cin.unget();
	cout << "Can't terminate this program using CTRL + C, try EXIT" << endl;
}

int main()
{
	string msg = "";
	try
	{
		TCPClient client("127.0.0.1");
		signal(SIGINT, sigHandler);
		while(getline(cin, msg))
		{
			if(msg == "EXIT")
			{
				return 0;
			}
			client.Send(msg);
			cout << client.Recieve() << endl;
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

