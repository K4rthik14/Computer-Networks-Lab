#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netdb.h>

int main(){
   struct sockaddr_in addr;
   int cid,r;
   unsigned int l;
   char buf[32],msg[32],temp[32];
   
  cid=socket(AF_INET,SOCK_STREAM,0);
  if(cid==-1){
     perror("socket cant be created..");
  }

  memset(&addr,0,sizeof(addr));
  addr.sin_family=AF_INET;
  addr.sin_port=htons(8000);
  addr.sin_addr.s_addr=inet_addr("127.0.0.1");

  if(connect(cid,(struct sockaddr*)&addr,sizeof(addr))<0){
      perror("socket connection..");
  }
while(1){
  printf("enter the message:");
  scanf("%s",temp);
  strncpy(msg,"",32);
  strcpy(msg,temp);
  l=strlen(msg);
  send(cid,msg,l,0);
if(strcmp(msg,"stop")==0){
   // printf("stopped");
    break;
}

  strncpy(buf,"",32);
  if(r=recv(cid,buf,31,0)<0){
     perror("error in  receiving...");
  }
  if(strcmp(buf,"stop")==0){
   // printf("stopped");
    break;
}
  printf("message received from server is %s \n",buf);
  
}
  close(cid);
  return 0;
}
