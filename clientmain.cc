#include <iostream>
#include <stdio.h>
#include <cstring>
#include <cstdlib>
#include "bankclient.h"

int main(int argc,char* argv[])
{
	if(argc<7)
	{
		std::cerr<<"Insuficient number of arguments"<<std::endl;
		exit(EXIT_FAILURE);
	}
	else
	{
		if(!strcmp(argv[1],"-h"))
		{
			if(!strcmp(argv[3],"-p"))
			{
				if(!strcmp(argv[5],"-i"))
				{
					BankClient bankclient(argv[2],argv[4],argv[6]);
					bankclient.run_client();
				}
				else
				{
					std::cerr<<"Unknown flag for third argument"<<std::endl;
					exit(EXIT_FAILURE);
				}
			}
			else if(!strcmp(argv[3],"-i"))
			{
				if(!strcmp(argv[5],"-p"))
				{
					BankClient bankclient(argv[2],argv[6],argv[4]);
					bankclient.run_client();
				}
				else
				{
					std::cerr<<"Unknown flag for third argument"<<std::endl;
					exit(EXIT_FAILURE);
				}
			}
			else
			{
				std::cerr<<"Unknown flag for second argument"<<std::endl;
				exit(EXIT_FAILURE);
			}
		}
		else if(!strcmp(argv[1],"-p"))
		{
			if(!strcmp(argv[3],"-h"))
			{
				if(!strcmp(argv[5],"-i"))
				{
					BankClient bankclient(argv[4],argv[2],argv[6]);
					bankclient.run_client();
				}
				else
				{
					std::cerr<<"Unknown flag for third argument"<<std::endl;
					exit(EXIT_FAILURE);
				}
			}
			else if(!strcmp(argv[3],"-i"))
			{
				if(!strcmp(argv[5],"-h"))
				{
					BankClient bankclient(argv[6],argv[2],argv[4]);
					bankclient.run_client();
				}
				else
				{
					std::cerr<<"Unknown flag for third argument"<<std::endl;
					exit(EXIT_FAILURE);
				}
			}
			else
			{
				std::cerr<<"Unknown flag for second argument"<<std::endl;
				exit(EXIT_FAILURE);
			}
		}
		else if(!strcmp(argv[1],"-i"))
		{
			if(!strcmp(argv[3],"-p"))
			{
				if(!strcmp(argv[5],"-h"))
				{
					BankClient bankclient(argv[6],argv[4],argv[2]);
					bankclient.run_client();
				}
				else
				{
					std::cerr<<"Unknown flag for third argument"<<std::endl;
					exit(EXIT_FAILURE);
				}
			}
			else if(!strcmp(argv[3],"-h"))
			{
				if(!strcmp(argv[5],"-p"))
				{
					BankClient bankclient(argv[4],argv[6],argv[2]);
					bankclient.run_client();
				}
				else
				{
					std::cerr<<"Unknown flag for third argument"<<std::endl;
					exit(EXIT_FAILURE);
				}
			}
			else
			{
				std::cerr<<"Unknown flag for second argument"<<std::endl;
				exit(EXIT_FAILURE);
			}
		}
		else
		{
			std::cerr<<"Unknown flag for first argument"<<std::endl;
			exit(EXIT_FAILURE);
		}
	}
	printf("Client Function Termination\n");
}