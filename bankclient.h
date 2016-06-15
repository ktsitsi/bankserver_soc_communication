#include <fstream>
class BankClient{
	struct sockaddr_in *server;
	struct sockaddr *serverptr;
	struct hostent *rem;
	char* commands[7];
	std::ifstream cfile;
	void FileExecute(int& sock,bool &client_stop);
	void Execute(int &socket,bool &client_stop);
public:
	BankClient(char* hostname,char* port_number,char* com_file);
	~BankClient();
	bool run_client();
};