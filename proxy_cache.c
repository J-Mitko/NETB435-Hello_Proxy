#include "proxy_cache.h"

#define MAXBUF 8192 /* max buffer size*/
#define MULTIPLIER (97)//prime numbers
/*
struct cache
{
	long url;// hasted url address corresponding to name of file
	char *file_name;
	
	char *request_date;
	char *date_modified;
	long  *file_size;
	
	int N; // number of elements
	
	struct cache **next;
};



Cache_ createCache()
{
	Cache_ new;
	
	new = calloc(sizeof(*new),1);
	assert(new != 0);// used to verify assumptions made by the program and print a diagnostic message if this assumption fails
	
	new->N = 1;
	new->next = calloc(sizeof(*new),1);
	
	assert(new->next != 0);
	
	new->next[0] = 0;//first element 0
	
	/*int i;
	for(i = 0;i< new->N;i++)
	{
		new->next[i] = 0;
	}*/
	/*
	
	return new;	
}

void destroyCache(Cache_ cache)
{
	free(cache->next);//FIX
	free(cache);
}

Cache_ insert(Cache_ cache, char *URL,char *BUFFER,char *Modified_date,char *Date,long *Content_lenght)
{

	int i = cache->N;

	Cache_ new;
	new = calloc(sizeof(*new),1);
	
	
	
	new->url = hash_func(URL);
	new->file_name = URL;

	new->request_date = Date;
	new->date_modified = Modified_date;
	new->file_size = Content_lenght;
//-!-!-	
	new->next[i] = cache;
	
	new->N++;
			
	write_file(URL,BUFFER);
	
	return new;
}

char *cacheSearch(Cache_ cache,char *URL)
{
	Cache_ temp;
	int i = cache->N;
	long u = hash_func(URL);
	
	for(temp = cache->next[i]; temp != 0 ;temp=cache->next[i])
	{
		if((long)temp->url == u)
		{
			return *read_file(temp->file_name);
		}
		i--;
	}
	return 0;
}
*/

//-----------------------------------------------------
// URL HASHING

static unsigned long hash_func(const char *S)
{
	unsigned const char *str;
	unsigned long hash = 0;
	
	for(str = (unsigned const char *) S; *str; str++)
		hash = hash * MULTIPLIER + *str;
	
	return hash;
}

//-----------------------------------------------------

void write_file(char* file_name, char* buffer)
{
	file = fopen(file_name,"a+b");
	if(!file)
		exit(1);
	char *TEMP = calloc(strlen(buffer),1);

	if(TEMP = strstr(buffer,"\r\n\r\n"))
		fprintf(file, "%s",TEMP+4);
	else
		fprintf(file, "%s",buffer);
	fclose(file);
}

char read_file(char* file_name)
{
	char *buffer = calloc(MAXBUF,1);
	
	file = fopen(file_name,"rb");
	if(!file)
		exit(1);
	fscanf(file, "%s",buffer);
	
	fclose(file);
	
	return *buffer; 
}


int search_for_file(char* file_name)
{
	file = fopen(file_name,"r");
	if(file)
	{
		fclose(file);
		return 1;
	}
	else
		return 0;
	
}

void delete_file(char* file_name)
{
	if(remove(file_name))
		perror("Error deleting files" );
	else
		printf("Files successfully deleted");	
}
