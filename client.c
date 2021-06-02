#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

//pthread
pthread_t f_pthread;
pthread_t s_pthread;

pthread_mutex_t f_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t s_mutex = PTHREAD_MUTEX_INITIALIZER;

typedef struct{
	int client;
	int size;
	char buf[4][140];
	int bytes_read;
	FILE *out;
}func_args;


void* write_f(void *args){
	func_args *data=(func_args*)args;
	while(1){
		//pthread_mutex_lock(&f_mutex);
		data->bytes_read=recvfrom(data->client,data->buf, data->size,0,NULL,NULL);
		if(data->bytes_read<=1){
			pthread_cancel(f_pthread);
			pthread_cancel(s_pthread);
		}
		//pthread_mutex_unlock(&f_mutex);
		//pthread_mutex_lock(&s_mutex);
		int i,j;
		for(i=0;i<4;i++){
			for(j=0;j<140;j++){
				if(data->buf[i][j]!='v')
				putc(data->buf[i][j],data->out);
    		}
		}
    	memset(data->buf,'v',data->size);
    	//pthread_mutex_unlock(&s_mutex);
    }
    return NULL;
}

int main()
{ 	int client=socket(af_inet, sock_dgram, 0);
    struct sockaddr_in addr;
    char buf[4][140];
    int size=sizeof(buf);
    char buf1[4][140];
   // int status, status1;
    file *out=fopen("/media/sf_Share/Visual/build-qwe-Desktop-Debug/res.txt","w");
    const  char *ip="192.168.193.3";

    memset(buf, 'v',size);
    memset(buf1,'v',size);

    if(client < 0)
    {
        perror("socket");
        exit(1);
    }
    
    addr.sin_family = AF_INET;
    addr.sin_port = htons(3425);
    addr.sin_addr.s_addr = inet_addr(ip);
    if(bind(client, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("badbind");
        exit(2);
    }
	func_args *arr_args=(func_args*) malloc(2 * sizeof(func_args));
    //заполняем структуру данных
	arr_args[0].client= client;
	arr_args[0].size=size;
	arr_args[0].buf[4][140]=buf[4][140];
	arr_args[0].out=out;
	arr_args[0].bytes_read=2;
	arr_args[1].client=client;
	arr_args[1].size=size;
	arr_args[1].buf[4][140]=buf1[4][140];
	arr_args[1].out=out;
	arr_args[1].bytes_read=2;
	//
	

    pthread_create(&f_pthread, NULL, write_f , &arr_args[0]);
    /*if(status!=0){
    	perror("PTHREAD_1");
        exit(1);
    }*/

    pthread_create(&s_pthread, NULL, write_f , &arr_args[1]);
    /*if(status1!=0){
    	perror("PTHREAD_2");
        exit(1);
    }*/
    pthread_join(f_pthread, NULL);
    pthread_join(s_pthread, NULL);
    //free(f_pthread);
    //free(s_pthread);
	fclose(out);
	close(client);
   
    
    return 0;
}
