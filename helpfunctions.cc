#include <stdio.h>
#include <cstdlib>
#include <ctype.h>	/*isdigit*/
#include <cstring>	/*strerror*/
#include <unistd.h>
#include "helpfunctions.h"

int num_digits(int check)			/*Number of digits is a simple function that calculate the number of integer's digits*/
{
	int length = 0;					//Multiple divistion with 10
	while(check /= 10)
	{
		length++;
	}
	return length;
}
bool isnumber(char* target)			//a function that takes a string as input and decides if this string is integer or not
{
	if(target == NULL){				//argument checking
		return false;
	}
	int len = strlen(target);		//calculate the length of the string
	bool ret_val = true;
	for(int i=0;i<len;i++){			//for the charachters of the string
		if(!isdigit(target[i])){	//checks if they are digits if sth is not the immediately return false
			ret_val = false;
		}
	}
	return ret_val;
}
void perror_exit(const char* errormessage)			//Function that prints an error message in stderr and then exits the callable
{
	perror(errormessage);
	exit(EXIT_FAILURE);
}

void perror2_exit(const char* errormessage, int err)
{
	fprintf(stderr,"%s: %s\n",errormessage,strerror(err));
	exit(EXIT_FAILURE);
}

int ChunkWrite(int sock,char* const c_command){
	int temp;
	int length = 0;
	length = strlen(c_command)+1;
	temp = length;
	if(write(sock,&temp,sizeof(int))<0)
		perror_exit("Error sending the length");
	//printf("The length is identified %d:\n",length);
	if(write(sock,c_command,length)<0)
		perror_exit("Error sending the command");
	//printf("The command is send\n");
	return length;
}

int ChunkRead(int sock,char** arrived_command)
{
	int temp;
	int i = 0;
	int length = 0;
	if(read(sock,&temp,sizeof(int))<0)
		perror_exit("Error in reading the size of message");
	length = temp;
	//printf("Length read\n");
	(*arrived_command) = (char*)malloc(length*sizeof(char));
	bzero(*arrived_command,length);
	while(i < length){
		i += read(sock,&(*arrived_command)[i],length-i);
	}
	//printf("Command/Acknowledge arrived\n");
	return length;

}