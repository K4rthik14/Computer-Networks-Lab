#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netdb.h>

char buff[32];
int main(){
    struct sockaddr_in servaddr,cliaddr;
    int cid,sid,r;
    unsigned int n;
    char buf[32],buff[32],*tim;
    int i,j,k,l;

   sid=socket(AF_INET,SOCK_STREAM,0);
   if(sid==0){
        perror("socket  cant be created..");
   } 
  memset(&servaddr,0,sizeof(servaddr));
  servaddr.sin_family=AF_INET;
  servaddr.sin_port=htons(8000);
  servaddr.sin_addr.s_addr=inet_addr("127.0.0.1");
  if(bind(sid,(struct sockaddr*)&servaddr,sizeof(servaddr))<0){
      perror("socket connection...");
   }
  if(listen(sid,5)<0){
      perror("error in listening..");
  }
  n=sizeof(cliaddr);
  if((cid=accept(sid,(struct sockaddr *)&cliaddr,&n))<0)
     perror("\n Acccept failed");

 while(1){
 
  strncpy(buf,"",32);
  if(l=(recv(cid,buf,31,0))<=0)
   perror("\n receive failed\n");

  l=strlen(buf);

  printf("%s",buf);
if(strcmp(buf,"stop")==0){
   // printf("stopped");
    break;
}

  strncpy(buff,"",32);
  printf("\n enter a message to client:");

  scanf("%s",buff);
  send(cid,buff,strlen(buff),0);
if(strcmp(buff,"stop")==0){
    printf("stopped");
    break;
 }
}
  close(sid);

return 0;
}
