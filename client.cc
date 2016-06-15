#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <ctype.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <netdb.h>
#include "bankclient.h"				/*bankserver functions*/
#include "helpfunctions.h"			/*error functions*/

BankClient::BankClient(char* hostname,char* port_number,char* com_file)
{
	int port = atoi(port_number);
	cfile.open(com_file);
	if((rem = gethostbyname(hostname))==NULL)
	{
		herror("gethostbyname");
		exit(EXIT_FAILURE);
	}
	if((server = (struct sockaddr_in *)malloc(sizeof(struct sockaddr_in))) == NULL)
		perror_exit("Malloc failed");
	serverptr = (struct sockaddr *)server;
	server->sin_family = AF_INET;
	server->sin_port = htons(port);
	memcpy(&(server->sin_addr),rem->h_addr,rem->h_length);
}

BankClient::~BankClient()
{
	free(server);
	/*close the command file*/
}

void BankClient::Execute(int &sock,bool& client_stop)
{
	std::string command_line;
	char *c_command;
	char *c_command_tok;
	int command_length;
	char* tok;

	std::cout<<"Give an input string"<<"\n";
	getline(std::cin,command_line);
	command_length = command_line.length()+1;		/*+1 for the null termination*/
	c_command = (char*)malloc(command_length*sizeof(char));
	bzero(c_command,command_length);
	strcpy(c_command,command_line.c_str());			/*C_command is a null terminated string*/
	c_command_tok = (char*)malloc(command_length*sizeof(char));
	bzero(c_command_tok,command_length);
	strcpy(c_command_tok,command_line.c_str());	

	tok = strtok(c_command_tok," \n");
	if(strcmp(tok,"exit"))					/*If the command is not the exit then wait for response*/
	{
		if(strcmp(tok,"sleep"))
		{
			/*Send until whole message is sent*/
			ChunkWrite(sock,c_command);
			
			char* acknowledge;

			ChunkRead(sock,&acknowledge);
			printf("%s\n",acknowledge);

			free(acknowledge);

		}
		else
		{
			//read the next int to sleep for
			tok = strtok(NULL," \n");
			usleep(atoi(tok));
		}
	}
	else											/*If the command is the exit bancklient should exit*/
	{
		/*Send until whole message is sent*/
		ChunkWrite(sock,c_command);
		client_stop = true;
	}
	free(c_command);
	free(c_command_tok);
}
void BankClient::FileExecute(int& sock,bool& client_stop)
{
	std::string command_line;
	char *c_command;
	char *c_command_tok;
	int command_length;
	char* tok;

	while(getline(cfile,command_line))
	{
		command_length = command_line.length()+1;		/*+1 for the null termination*/
		c_command = (char*)malloc(command_length*sizeof(char));
		bzero(c_command,command_length);
		strcpy(c_command,command_line.c_str());			/*C_command is a null terminated string*/
		c_command_tok = (char*)malloc(command_length*sizeof(char));
		bzero(c_command_tok,command_length);
		strcpy(c_command_tok,command_line.c_str());	

		tok = strtok(c_command_tok," \n");
		if(strcmp(tok, "exit"))
		{
			if(strcmp(tok,"sleep"))
			{
				/*Send until whole message is sent*/
				ChunkWrite(sock,c_command);
				
				char* acknowledge;

				ChunkRead(sock,&acknowledge);
				printf("%s\n",acknowledge);

				free(acknowledge);
			}
			else
			{
				tok=strtok(NULL," \n");
				usleep(atoi(tok));
			}
			free(c_command);
			free(c_command_tok);
		}
		else
		{
			/*Send until whole message is sent*/
			ChunkWrite(sock,c_command);
			client_stop = true;
			free(c_command);
			free(c_command_tok);
			break;
		}
	}
}

bool BankClient::run_client()
{
	int sock;
	bool client_stop1 = false;
	bool client_stop2 = false;
	if((sock = socket(PF_INET,SOCK_STREAM,0))< 0)
		perror_exit("socket");

	if(connect(sock,serverptr,sizeof(*server)) < 0)
		perror_exit("connect");

	//Execute for the file
	FileExecute(sock,client_stop1);
	if(!client_stop1)
	{
		while(1)
		{
			Execute(sock,client_stop2);
			if(client_stop2)
			{
				shutdown(sock,SHUT_RDWR);
				close(sock);
				break;
			}
		}
	}
	else
	{
		shutdown(sock,SHUT_RDWR);
		close(sock);
	}
	return true;
}