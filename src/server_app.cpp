
#include "../include/Mailbox.hpp"

int main()
{
	try
	{
		Mailbox m;
		Mailbox::Eval(ref(m));
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

