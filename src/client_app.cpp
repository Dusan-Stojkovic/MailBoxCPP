
#include"../include/TCPClient.hpp"

int main()
{
	try
	{
		TCPClient client("127.0.0.1");
		client.Send("this is a test");
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

