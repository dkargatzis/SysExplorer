#ifndef _threadpool_h
#define _threadpool_h
#include <pthread.h>

/* Declarations */

enum ThreadState{
	IDLE, BUSY
};

/* Container for all things that each thread is going to need */
typedef struct thread_data{       
	pthread_t        p_id;                       
	pthread_mutex_t *mutex_p;
	struct thpool_t        *tp_p;
	void*			 (*func)(void *);
	void*			 arg;
	enum ThreadState state; 
	pthread_cond_t cond_var;
}thread_data;

/* The threadpool */
typedef struct thpool_t{
	int 			 max_idle_threads;					/**< max number of idle Threads 	  */
	int              threadsN;                         /**< cuurent amount of threads    	  */
	pthread_mutex_t  mutex;								
	struct thread_data **threadArray;                  /**< array witch contains idle threads */
}thpool_t;

/*
	Create threadpool for the number of max idle threads 
*/
int init_ThreadPool(thpool_t *pool, int max_idle_threads);

/*
	Use one idle thread for the task func if number of idle threads > 10,
	else create a new one for this task	
*/
pthread_t ThreadPool_spawn(thpool_t *this, void (*func)(void *), void *arg);

int wait_Thread(void *new_thread);

#endif 
