#include <iostream>
#include <stdio.h>
#include <cstring>
#include "bankserver.h"

int main(int argc,char* argv[])
{
	/*The argument check and the bankserver creation*/
	if(argc<7)
	{
		std::cerr<<"Insuficient number of arguments"<<std::endl;
		exit(EXIT_FAILURE);
	}
	else
	{
		if(!strcmp(argv[1],"-p"))
		{
			if(!strcmp(argv[3],"-s"))
			{
				if(!strcmp(argv[5],"-q"))
				{
					BankServer bankserver(argv[2],argv[4],argv[6]);
					bankserver.run_server();
				}
				else
				{
					std::cerr<<"Unknown flag for third argument"<<std::endl;
					exit(EXIT_FAILURE);
				}
			}
			else if(!strcmp(argv[3],"-q"))
			{
				if(!strcmp(argv[5],"-s"))
				{
					BankServer bankserver(argv[2],argv[6],argv[4]);
					bankserver.run_server();
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
		else if(!strcmp(argv[1],"-s"))
		{
			if(!strcmp(argv[3],"-p"))
			{
				if(!strcmp(argv[5],"-q"))
				{
					BankServer bankserver(argv[4],argv[2],argv[6]);
					bankserver.run_server();
				}
				else
				{
					std::cerr<<"Unknown flag for third argument"<<std::endl;
					exit(EXIT_FAILURE);
				}
			}
			else if(!strcmp(argv[3],"-q"))
			{
				if(!strcmp(argv[5],"-p"))
				{
					BankServer bankserver(argv[6],argv[2],argv[4]);
					bankserver.run_server();
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
		else if(!strcmp(argv[1],"-q"))
		{
			if(!strcmp(argv[3],"-p"))
			{
				if(!strcmp(argv[5],"-s"))
				{
					BankServer bankserver(argv[2],argv[6],argv[2]);
					bankserver.run_server();
				}
				else
				{
					std::cerr<<"Unknown flag for third argument"<<std::endl;
					exit(EXIT_FAILURE);
				}
			}
			else if(!strcmp(argv[3],"-s"))
			{
				if(!strcmp(argv[5],"-p"))
				{
					BankServer bankserver(argv[6],argv[4],argv[2]);
					bankserver.run_server();
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
	/*Bankserver run*/
	printf("Termination of Server Function.\n");
}