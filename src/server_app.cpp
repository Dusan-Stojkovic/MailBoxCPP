
#include "../include/TCPServer.hpp"

int main()
{
	try
	{
		TCPServer s;
		cout<< s.Recieve() << endl;
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

