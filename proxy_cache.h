#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

FILE* file;

/*
/****************************************

	typedef struct cache *Cache_;
	

/****************************************	

Cache_ createCache();

void destroyCache(Cache_ cache);

Cache_ insert(Cache_ cache, char *URL,char *BUFFER,char *Modified_date,char *Date,long *Content_lenght);

char *cacheSearch(Cache_ cache,char *URL);
*/

static unsigned long hash_func(const char *S);

void write_file(char* file_name, char* buffer);

char read_file(char* file_name);

int search_for_file(char* file_name);

void delete_file(char* file_name);
