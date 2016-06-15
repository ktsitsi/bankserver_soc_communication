#include <pthread.h>
#include <unistd.h>

class TaskQueue{			//Cyclic list representing the queue of tasks/connections	
private:
	int queue_size;			//queue magnitude
	int* task;				//array of int sockets/tasks
	int start;				//points to start of the cyclic list
	int end;				//points at the end of cyclic list
	int counter;			//counter of the elements that are int he cycli list
	pthread_mutex_t mutex;	//mutex of the cyclic list for multithreading 
	pthread_cond_t cond_nonempty;		//condition variable true when at least one element in the list
	pthread_cond_t cond_nonfull;		//condtion variable true when at least one space in the list for element
public:
	TaskQueue(int q_size);			//constructor of this cyclic list
	~TaskQueue();					//destructor of this cyclic list
	void Enqueue(int newtask);		//put a connection/socket in the cyclic list
	int Dequeue();					//take the first connection/ socket out of the list
};