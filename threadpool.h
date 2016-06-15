class ThreadPool{
private:
	/*Private Data*/
	int pool_size;
	pthread_t** pool;
	pthread_mutex_t tpcount_lock;
	pthread_cond_t threads_all_idle;
	/*Private Functions*/
	void thread_init(pthread_t** th_init);
	void thread_function(void* args);

public:
	ThreadPool(int argsize);

};

void thread_function(void* f_args)
{
	printf("I am a little thread!\n");

}

void thread_init(pthread_t** pthread){

	*pthread = (pthread_t *)malloc(sizeof(pthread_t));
	pthread_create(pthread, NULL, thread_function, NULL);

}

void ThreadPool(int argsize):pool_size(argsize)
{
	pool = (pthread_t**)malloc(argsize*sizeof(pthread_t*));
	
	pthread_mutex_init(&tpcount_lock);
	pthread_cond_init(&thpool_p->threads_all_idle, NULL);

	for(int i=0;int<argsize;i++)
	{
		thread_init(&pool[i],i);
	}

}