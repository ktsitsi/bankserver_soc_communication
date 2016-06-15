#include "task_queue.h"
#include <cstdlib>

TaskQueue::TaskQueue(int q_size)
{
	queue_size = q_size;
	//init mutexs and cond vals
	pthread_mutex_init(&mutex,NULL);
	pthread_cond_init(&cond_nonempty,NULL);
	pthread_cond_init(&cond_nonfull,NULL);
	task = (int*)malloc(q_size*sizeof(int));
	start = 0;
	end = -1;
	counter = 0;
}

TaskQueue::~TaskQueue(){
	free(task);
}

void TaskQueue::Enqueue(int newtask)
{
	pthread_mutex_lock(&mutex);
	while(counter >= queue_size){
		pthread_cond_wait(&cond_nonfull,&mutex);
	}
	end = (end + 1) % queue_size;
	task[end] = newtask;
	counter++;
	pthread_cond_signal(&cond_nonempty);
	pthread_mutex_unlock(&mutex);
}

int TaskQueue::Dequeue(){
	int deq_task = 0;
	pthread_mutex_lock(&mutex);
	while(counter <= 0){
		pthread_cond_wait(&cond_nonempty,&mutex);
	}
	deq_task = task[start];
	start = (start + 1)% queue_size;
	counter--;
	pthread_mutex_unlock(&mutex);
	return deq_task;
}