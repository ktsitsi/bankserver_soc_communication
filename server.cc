#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>					/*strcpy,strcmp...*/
#include <ctype.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <netdb.h>
#include <pthread.h>				/*thread utilities maybe comment that*/
#include "bankserver.h"				/*bankserver functions*/
#include "helpfunctions.h"			/*error functions*/

static bool server_up = true;

BankServer::BankServer(char* argport,char* pl_size,char* q_size)
{
	/*Connection Segment*/
	server_up = true;
	int port = atoi(argport);
	int queue_size = atoi(q_size);
	pool_size = atoi(pl_size);

	if((sa_in = (struct sockaddr_in *)malloc(sizeof(struct sockaddr_in)))==NULL)
		perror_exit("Malloc failed");
	sa = (struct sockaddr *)sa_in;

	sa_in->sin_family = AF_INET;
	(sa_in->sin_addr).s_addr = htonl(INADDR_ANY);
	sa_in->sin_port = htons(port);

	data.task_queue = new TaskQueue(queue_size);
	data.hash_memory = new HashTable(HASHTABLE_SIZE);

	data.commands[0] = (char*)malloc((strlen("add_account")+1)*sizeof(char));
	bzero(data.commands[0],strlen("add_account")+1);
	data.commands[1] = (char*)malloc((strlen("add_transfer")+1)*sizeof(char));
	bzero(data.commands[1],strlen("add_transfer")+1);
	data.commands[2] = (char*)malloc((strlen("add_multi_transfer")+1)*sizeof(char));
	bzero(data.commands[2],strlen("add_multi_transfer")+1);
	data.commands[3] = (char*)malloc((strlen("print_balance")+1)*sizeof(char));
	bzero(data.commands[3],strlen("print_balance")+1);
	data.commands[4] = (char*)malloc((strlen("add_print_multi_balance")+1)*sizeof(char));
	bzero(data.commands[4],strlen("add_print_multi_balance")+1);
	data.commands[5] = (char*)malloc((strlen("exit")+1)*sizeof(char));
	bzero(data.commands[5],strlen("exit")+1);
	
	memcpy(data.commands[0],"add_account",strlen("add_account"));
	memcpy(data.commands[1],"add_transfer",strlen("add_transfer"));
	memcpy(data.commands[2],"add_multi_transfer",strlen("add_multi_transfer"));
	memcpy(data.commands[3],"print_balance",strlen("print_balance"));
	memcpy(data.commands[4],"print_multi_balance",strlen("add_print_multi_balance"));
	memcpy(data.commands[5],"exit",strlen("exit"));
}

BankServer::~BankServer()
{
	free(sa_in);
	for(int i=0;i<6;i++){
		free(data.commands[i]);
	}
	delete data.hash_memory;
	delete data.task_queue;
}

void BankServer::Tokenizer(int sock,char* const arrived_command,void* arguments,bool &client_stop)
{
	struct struct_data *data = (struct struct_data *)arguments;
	char* command_tokens;
	char* init_amount;
	char* source_name;
	char* dest_name;
	char* extra_token = NULL;
	int  delay = -1;
	char* acknowledgement = NULL;
	bool found = false;
	
	command_tokens = strtok(arrived_command," \n");					/*Token of the command*/
	if(command_tokens != NULL)
	{
		for(int iter=0; iter<6; iter++)								/*Check on the available commands of the server*/
		{
			if(!strcmp(command_tokens,data->commands[iter]))		/*If arrived belongs to one of the available*/
			{
				if(iter == 0)//Add account is ready
				{
					found = true;
					init_amount = strtok(NULL," \n");
					source_name = strtok(NULL," \n");
					if((extra_token = strtok(NULL," \n"))!=NULL){
						delay = atoi(extra_token);
					}
					if(data->hash_memory->Insert(std::string(source_name),atoi(init_amount),delay))
					{
						int ack_length;
						if(extra_token!=NULL)
							ack_length = strlen("Success. Account Creation (::)") + strlen(init_amount) + strlen(source_name)+ strlen(extra_token); 
						else
							ack_length = strlen("Success. Account Creation (:)") + strlen(init_amount) + strlen(source_name); 
						acknowledgement = (char*)malloc((ack_length+1)*sizeof(char));
						bzero(acknowledgement,ack_length+1);
						if(extra_token!=NULL)
							sprintf(acknowledgement,"Success. Account Creation (%s:%s:%s)",source_name,init_amount,extra_token);
						else
							sprintf(acknowledgement,"Success. Account Creation (%s:%s)",source_name,init_amount);
						ChunkWrite(sock,acknowledgement);
						free(acknowledgement);
					}
					else
					{
						//Failure of add_account
						int ack_length;
						if(extra_token!=NULL)
							ack_length = strlen("Error. Account Creation failed (::)") + strlen(init_amount) + strlen(source_name)+strlen(extra_token); 
						else
							ack_length = strlen("Error. Account Creation failed (:)") + strlen(init_amount) + strlen(source_name); 
						acknowledgement = (char*)malloc((ack_length+1)*sizeof(char));
						bzero(acknowledgement,ack_length+1);
						if(extra_token!=NULL)
							sprintf(acknowledgement,"Error. Account Creation failed (%s:%s:%s)",source_name,init_amount,extra_token);
						else
							sprintf(acknowledgement,"Error. Account Creation failed (%s:%s)",source_name,init_amount);
						ChunkWrite(sock,acknowledgement);
						free(acknowledgement);
					}
				}
				else if(iter == 1)//Add transfer is ready
				{
					//add_transfer
					found = true;
					init_amount = strtok(NULL," \n");
					source_name = strtok(NULL," \n");
					dest_name = strtok(NULL," \n");
					if((extra_token = strtok(NULL," \n"))!=NULL){
						delay = atoi(extra_token);
					}
					if(data->hash_memory->AddTransfer(std::string(source_name),std::string(dest_name),atoi(init_amount),delay))
					{
						int ack_length;
						if(extra_token!=NULL)
							ack_length = strlen("Success. Transfer addition (:::)") + strlen(init_amount) + strlen(source_name) + strlen(dest_name) +strlen(extra_token);
						else
							ack_length = strlen("Success. Transfer addition (::)") + strlen(init_amount) + strlen(source_name) + strlen(dest_name); 
						acknowledgement = (char*)malloc((ack_length+1)*sizeof(char));
						bzero(acknowledgement,ack_length+1);
						if(extra_token!=NULL)
							sprintf(acknowledgement,"Success. Transfer addition (%s:%s:%s:%s)",source_name,dest_name,init_amount,extra_token);
						else
							sprintf(acknowledgement,"Success. Transfer addition (%s:%s:%s)",source_name,dest_name,init_amount);
						ChunkWrite(sock,acknowledgement);
						free(acknowledgement);
					}
					else
					{
						int ack_length;
						if(extra_token!=NULL)
							ack_length = strlen("Error. Transfer addition (:::)") + strlen(init_amount) + strlen(source_name) + strlen(dest_name)+strlen(extra_token);
						else
							ack_length = strlen("Error. Transfer addition (::)") + strlen(init_amount) + strlen(source_name) + strlen(dest_name); 
						acknowledgement = (char*)malloc((ack_length+1)*sizeof(char));
						bzero(acknowledgement,ack_length+1);
						if(extra_token!=NULL)
							sprintf(acknowledgement,"Error. Transfer addition (%s:%s:%s:%s)",source_name,dest_name,init_amount,extra_token);
						else
							sprintf(acknowledgement,"Error. Transfer addition (%s:%s:%s)",source_name,dest_name,init_amount);
						ChunkWrite(sock,acknowledgement);
						free(acknowledgement);
					}
				}
				else if(iter == 2)
				{
					//Multiple add_transfer in case of multiple names
					found = true;
					List<ClientNode> alldest;
					init_amount = strtok(NULL," \n");
					source_name = strtok(NULL," \n");
					dest_name = strtok(NULL," \n");
					while(dest_name != NULL){
						if(isnumber(dest_name)){
							extra_token = dest_name;
							delay = atoi(dest_name);
							break;
						}
						ClientNode current(dest_name);
						alldest.Push(current);
						dest_name = strtok(NULL," \n");
					}
					if(data->hash_memory->MultiAddTransfer(std::string(source_name),alldest,atoi(init_amount),delay))
					{
						//Send a success message
						int ack_length;
						if(extra_token!=NULL)
							ack_length = strlen("Success. Multi-Transfer addition (::)") + strlen(init_amount) + strlen(source_name)+strlen(extra_token);
						else
							ack_length = strlen("Success. Multi-Transfer addition (:)") + strlen(init_amount) + strlen(source_name); 
						acknowledgement = (char*)malloc((ack_length+1)*sizeof(char));
						bzero(acknowledgement,ack_length+1);
						if(extra_token!=NULL)
							sprintf(acknowledgement,"Success. Multi-Transfer addition (%s:%s:%s)",source_name,init_amount,extra_token);
						else
							sprintf(acknowledgement,"Success. Multi-Transfer addition (%s:%s)",source_name,init_amount);
						ChunkWrite(sock,acknowledgement);
						free(acknowledgement);
					}
					else
					{
						//Send error message
						int ack_length;
						if(extra_token!=NULL)
							ack_length = strlen("Error. Multi-Transfer addition (::)") + strlen(init_amount) + strlen(source_name) + strlen(extra_token);
						else
							ack_length = strlen("Error. Multi-Transfer addition (:)") + strlen(init_amount) + strlen(source_name); 
						acknowledgement = (char*)malloc((ack_length+1)*sizeof(char));
						bzero(acknowledgement,ack_length+1);
						if(extra_token!=NULL)
							sprintf(acknowledgement,"Error. Multi-Transfer addition (%s:%s:%s)",source_name,init_amount,extra_token);
						else
							sprintf(acknowledgement,"Error. Multi-Transfer addition (%s:%s)",source_name,init_amount);
						ChunkWrite(sock,acknowledgement);
						free(acknowledgement);
					}
				}
				else if(iter == 3)	//Print Balance Ready
				{
					//print_balance
					found = true;
					source_name = strtok(NULL," \n");
					int amount;
					if((amount = data->hash_memory->PrintBalance(source_name))>=0)
					{
						int int_length = snprintf(NULL,0,"%d",amount);
						char* camount = (char*)malloc(int_length+1);				
						bzero(camount,int_length+1);
						snprintf(camount,int_length+1,"%d",amount);
						int ack_length = strlen("Success. Balance (:)") + strlen(source_name) + int_length;//1 is for the balance 
						acknowledgement = (char*)malloc((ack_length+1)*sizeof(char));
						bzero(acknowledgement,ack_length+1);
						snprintf(acknowledgement,(ack_length+1),"Success. Balance (%s:%s)",source_name,camount);
						ChunkWrite(sock,acknowledgement);
						free(acknowledgement);
						free(camount);
					}
					else
					{
						int ack_length = strlen("Error. Balance ()") + strlen(source_name); 
						acknowledgement = (char*)malloc((ack_length+1)*sizeof(char));
						bzero(acknowledgement,ack_length+1);
						sprintf(acknowledgement,"Error. Balance (%s)",source_name);
						ChunkWrite(sock,acknowledgement);
						free(acknowledgement);
					}
				}
				else if(iter == 4)
				{
					//print multi_balance
					List<ClientNode> alldest;
					dest_name = strtok(NULL," \n");
					while(dest_name != NULL){
						ClientNode current(dest_name);
						alldest.Push(current);
						dest_name = strtok(NULL," \n");
					}
					std::string name_list;
					if((data->hash_memory->MultiPrint(alldest,name_list))){
						int ack_length = strlen("Success. Multi-Balance ()") + name_list.length();
						acknowledgement = (char*)malloc((ack_length+1)*sizeof(char));
						bzero(acknowledgement,ack_length+1);
						sprintf(acknowledgement,"Success. Multi-Balance (%s)",name_list.c_str());
						ChunkWrite(sock,acknowledgement);
						free(acknowledgement);
					}
					else
					{
						//Something in the print is not in the memory
						data->hash_memory->ErrorMultiPrint(alldest,name_list);
						int ack_length = strlen("Error. Balance ()") + name_list.length(); 
						acknowledgement = (char*)malloc((ack_length+1)*sizeof(char));
						bzero(acknowledgement,ack_length+1);
						sprintf(acknowledgement,"Error. Balance (%s)",name_list.c_str());
						ChunkWrite(sock,acknowledgement);
						free(acknowledgement);
					}
				}
				else
				{
					//Exit client from the server
					found = true;
					client_stop = true;
				}
			}
		}	
		if(!found)
		{
			//Unknown command
			int ack_length = strlen("Εrror. Unknown Command");
			acknowledgement = (char*)malloc((ack_length+1)*sizeof(char));
			bzero(acknowledgement,ack_length+1);
			sprintf(acknowledgement,"Εrror. Unknown Command");
			ChunkWrite(sock,acknowledgement);
			free(acknowledgement);
		}
	}
}

void* BankServer::thread_function(void* arguments)
{
	struct struct_data *data = (struct struct_data *)arguments;
	while(server_up)
	{
		int sock;
		char* arrived_command;
		bool client_stop = false;

		sock = data->task_queue->Dequeue();								/*Get first available task from the task queue*/
		
		while(!client_stop){											/*Run until the client sends an exit command*/
		
			/*Mechanism for reading arbitrary long commands with constant size buffer*/
			//printf("ChunkRead\n");
			
			ChunkRead(sock,&arrived_command);

			/*Tokenize the arrived command for execution*/
			/*If client_stop becomes true inside the Tokenizer then means exit*/											/*If gets an exit*/
			Tokenizer(sock,arrived_command,arguments,client_stop);
			
			free(arrived_command);
		}

		/*Close Thread*/
		shutdown(sock,SHUT_RDWR);
		close(sock);
	}
	pthread_detach(pthread_self());
	pthread_exit(NULL);
}

bool BankServer::run_server()
{
	int sock,newconn;

	/*struct sockaddr_in client;
	socklen_t clientlen;
	struct sockaddr* clientptr = (struct sockaddr *)&client;
	*/
	/*Create Socket*/
	if((sock = socket(PF_INET,SOCK_STREAM,0))<0)
		perror_exit("socket");
	
	if(bind(sock,sa,sizeof(*sa)) == -1)
		perror_exit("Error in socket binding");
	
	if(listen(sock,SOMAXCONN) == -1)
		perror_exit("Error in Listening Socket");
	
	printf("Listening for connections!\n");
	
	pthread_t pool[pool_size];
	for(int p=0; p<pool_size; p++)
	{
		pthread_create(&pool[p],NULL,thread_function,(void*)&data);
	}

	while(1)
	{
		if((newconn = accept(sock,NULL,NULL))<0)
			perror_exit("Accept Error");
		data.task_queue->Enqueue(newconn);								/*If the queue of the sockets(task queue) is full then blocks*/
		printf("New connection in the queue\n");
	}
	return true;
};