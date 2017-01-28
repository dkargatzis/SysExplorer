#include <stdio.h>
#include <pthread.h>
#include "threadpool.h"
#include "sysexplorer.h"

int init_ThreadPool(thpool_t *pool, int max_idle_threads){
	if((pool->threadArray = malloc(max_idle_threads*sizeof(thread_data *))) == NULL){
		printf("Unable to allocate memory for threadpool\n");
		return -1;
	}else{
		pool->max_idle_threads = max_idle_threads;
		pool->threadsN = 0;
		if(pthread_mutex_init(&pool->mutex, NULL) !=0){
			printf("Unable to init mutex for threadpool\n");
			free(pool->threadArray);
			return -1;
		}else{
			return 0;
		}

	}
}


pthread_t ThreadPool_spawn(thpool_t *this, void (*func)(void *),void *arg){
	pthread_t p_id;
	pthread_mutex_lock(&this->mutex);
	#ifndef DEBUG
			printf("Thread pool state  \n");
		#endif

	if(this->threadsN == 0){			//create a new thread
		thread_data *new;
		if((new = malloc(sizeof(thread_data *))) == NULL)
			printf("Unable to allocate memory for threadpool\n");

		
		new->tp_p = this;
		new->state = BUSY;
		new->arg = arg;
		new->func = func; 
		puts(func);
		if( new->state == BUSY)
			printf("state is BUSY\n");
		if( new->state == IDLE)
			printf("state is IDLE\n");
		#ifndef DEBUG
			printf("Thread pool state 1 \n");
		#endif
		
		if(pthread_create(&p_id, NULL, wait_Thread, arg) == -1)
			pthread_detach(p_id);
		
				
		
	}else{							//else use an existing thread
		#ifndef DEBUG
		printf("Thread pool Idle threads: %d\n", this->threadsN);
		#endif
		this->threadsN--;
		this->threadArray[this->threadsN]->state = BUSY;
		this->threadArray[this->threadsN]->func = func;
		p_id = this->threadArray[this->threadsN]->p_id;
		pthread_cond_signal(&this->threadArray[this->threadsN]->cond_var);
	}

	pthread_mutex_unlock(&this->mutex);
	return p_id;
}

int wait_Thread(void *new_thread){
	thread_data *new = (thread_data *)new_thread;
	pthread_mutex_t mt;

	if(pthread_mutex_init(&mt, NULL) !=0){
		printf("Unable to init mutex for threadpool\n");
		return 0;
	}
	pthread_mutex_lock(&mt);
	if(pthread_cond_init(&new->cond_var, NULL) != 0){
		printf("Unable to init comdition variable\n");
	}
	#ifndef DEBUG
		printf("Thread pool state  wait thread\n");
	#endif
	while(new->state == IDLE){
		printf("state is = IDLE\n");
		pthread_cond_wait(&new->cond_var, &mt);
	}
	#ifndef DEBUG
		printf("Thread pool state  %s\n", new->state);
	#endif
	new->func(new->arg);		/*	if(new->state == BUSY) run func()		*/
	if(new->tp_p->threadsN >= new->tp_p->max_idle_threads){
		pthread_mutex_unlock(&new->tp_p->mutex);
		return -1;
	}else{
		new->tp_p->threadArray[new->tp_p->threadsN++] = new;
		new->state = IDLE;
		pthread_mutex_unlock(&new->tp_p->mutex);
	}
	

	pthread_mutex_unlock(&mt);
	pthread_mutex_destroy(&mt);
	pthread_cond_destroy(&new->cond_var);
	return 0;

}
