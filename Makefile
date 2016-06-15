FLAGS = -Wall
THREADLIB = -lpthread
DEBUG = -g 
OBJ = bankserver bankclient
CC = g++

all: bankserver bankclient

bankserver: servermain.o server.o datatypes.o helpfunctions.o hash.o task_queue.o
	$(CC) $(DEBUG) $(FLAGS) -o bankserver servermain.o server.o datatypes.o helpfunctions.o hash.o task_queue.o $(THREADLIB)
	
bankclient: clientmain.o client.o datatypes.o helpfunctions.o hash.o task_queue.o
	$(CC) $(DEBUG) $(FLAGS) -o bankclient clientmain.o client.o datatypes.o helpfunctions.o hash.o task_queue.o

clientmain.o: clientmain.cc
	$(CC) $(DEBUG) $(FLAGS) -c clientmain.cc

servermain.o: servermain.cc
	$(CC) $(DEBUG) $(FLAGS) -c servermain.cc 
	
server.o: server.cc
	$(CC) $(DEBUG) $(FLAGS) -c server.cc
	
client.o: client.cc
	$(CC) $(DEBUG) $(FLAGS) -c client.cc

helpfunctions.o: helpfunctions.cc
	$(CC) $(DEBUG) $(FLAGS) -c helpfunctions.cc

datatypes.o: datatypes.cc
	$(CC) $(DEBUG) $(FLAGS) -c datatypes.cc

hash.o: hash.cc
	$(CC) $(DEBUG) $(FLAGS) -c hash.cc

task_queue.o: task_queue.cc
	$(CC) $(DEBUG) $(FLAGS) -c task_queue.cc


.PHONY : clean
clean:
	rm *.o $(OBJ)
