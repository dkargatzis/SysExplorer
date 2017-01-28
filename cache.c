#include <stdio.h>
#include <pthread.h>
#include <sys/stat.h>
#include "cache.h"
#include "hashtable.h"
#include "linkedlist.h"

void init_cache(int max_pages){
	List_init(&Cache);
	pthread_mutex_init(&mutex,NULL);
	HashTable_init(&hashTable, max_pages, (HashFunc)strhash, (EqPred)hash_compare);
}

int hash_compare(char* s1, char* s2){
	return(strcmp(s1,s2));
}

int find_Page(char* path, void* fd){
	int *FD=(int*)fd;
	struct Page *old_page,*page;
	page = (struct Page *) malloc (sizeof(struct Page));

	int found=0;
	struct stat bufc;

	lstat(path,&bufc);    
	page->version = bufc.st_ctime;
	pthread_mutex_lock(&mutex);
	if (HashTable_find(&hashTable,path) != HashTable_end(&hashTable)){
		old_page = (struct Page *) HashTable_find(&hashTable,path)->value;

		if (old_page->version != page->version){
			found = 0;		
		}else{
			printf("Found in cache..\n");
			found = 1;
			write((int)FD, old_page->response, strlen(old_page->response));
		}
	}else{
		found = 0;
	}
	pthread_mutex_unlock(&mutex);
	//printf("\tEnd of find page function\n");
	return(found);
}

void add_Page(char* path, char* response){
	struct  Page *new_page;
	
	new_page = (struct Page *) malloc(sizeof(struct Page));
	new_page->path = malloc(strlen(path)*sizeof(char));
	strcpy(new_page->path, path);

	new_page->response = malloc(strlen(response)*sizeof(char));
	strcpy(new_page->response, response);
	new_page->responselength = strlen(response);

	HashTable_insert(&hashTable, path, (void*)new_page);
	List_push_front(&Cache, path);
	printf("LIst size : %d\n", List_size(&Cache) );
	//printf("\tEnd of add page function\n");

}

void delete_LRU_page(){
	char *temp;
	if(HashTable_size(&hashTable) == 100){
		temp = List_pop_back(&Cache);
		HashTable_remove(&hashTable, temp);
	}
}
