#include <stdint.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "hash.h"
#include "task_queue.h"

#define HASHTABLE_SIZE 5

struct struct_data{
	TaskQueue* task_queue;
	HashTable* hash_memory;
	char *commands[6];
};

class BankServer{
private:
	/*Private Data*/
	int pool_size;
	struct sockaddr_in *sa_in;
	struct sockaddr *sa;
	/*Private functions*/
	struct struct_data data;
	static void Tokenizer(int sock,char* const arrived_command,void* arguments,bool& client_stop);
	static void* thread_function(void* args);

public:
	BankServer(char* port,char* pl_size,char* q_size);
	~BankServer();
	void Execute(int &socket);
	bool run_server();
};