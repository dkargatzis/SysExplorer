#ifndef _cache_h
#define _cache_h
#include "hashtable.h"
#include "linkedlist.h"

struct Page
{
	char *path; 
	char *response;
	int responselength;
	time_t version;
};


List Cache;
HashTable hashTable;

pthread_mutex_t mutex;

int hash_compare(char* s1, char* s2);

void init_cache(int max_pages);

void add_Page(char* path, char* response);

void detele_LRU_page();

int find_Page(char* path,void* fd);

#endif 
