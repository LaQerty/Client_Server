#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(){
    char msg='v';
    int server;
    struct sockaddr_in addr;
    //struct sockaddr_in addr1;
    int data_matr[140][4];
    size_t nread;
    FILE *in;
    FILE *out;
    FILE *snd;
    const char *ip="192.168.193.3";
    //const char *ip1="192.168.193.5";
    server = socket(AF_INET, SOCK_DGRAM, 0);
    int b_data[32768][5];
    if(server < 0)
    {
        perror("socket");
        exit(1);
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(3425);
    addr.sin_addr.s_addr =htonl(INADDR_LOOPBACK);
    //addr1.sin_family = AF_INET;
    //addr1.sin_port = htons(3425);
    //addr1.sin_addr.s_addr =inet_addr(ip1);

    /*if(bind(server, (struct sockaddr *)&addr, sizeof(addr)) < 0){
        perror("bind");
      exit(2);   
    }*/
    in = fopen("First_data/qam256.txt","r");
    out = fopen("First_data/qamres.txt","w");
    int count_cols =0;
    while(!feof(in)){
        for(int j=0; j<5;j++){
            fscanf(in, "%d", &b_data[count_cols][j]);
        }
        count_cols++;
    }
    for(int i=0;i<count_cols;i++){
        for(int j=1; j<5;j++){
            if(b_data[i][0]){
                fprintf(out,"%d\n" ,b_data[i][j]);
            } 
        }
    }
    fclose(out);
    fclose(in);
    snd = fopen("First_data/qamres.txt","r");
    sleep(10);
	    while(!feof(snd)){
    		for(int i=0;i<140;i++){
                for(int j=0; j<4;j++){
                    fscanf(snd,"%d" ,&data_matr[i][j]);
                }
            }
    		sendto(server, data_matr, sizeof(data_matr), 0, (struct sockaddr *)&addr, sizeof(addr));
		}
 	    sendto(server, &msg, sizeof(msg), 0, (struct sockaddr *)&addr, sizeof(addr));
            fclose(snd);
    close(server);  
    return 0;
}
