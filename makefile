mitko_proxy: proxy_.c connectTCP.c passiveTCP.c errexit.c connectsock.c passivesock.c proxy_cache.c
	gcc -pthread -o mitko_proxy proxy_.c connectTCP.c passiveTCP.c errexit.c connectsock.c passivesock.c proxy_cache.h proxy_cache.c
