/*
    C ECHO client example using sockets
*/
#include <stdio.h> //printf
#include <string.h>    //strlen
#include <sys/socket.h>    //socket
#include <arpa/inet.h> //inet_addr

char* getBranchName();
char* getMachineName();
 
int main(int argc , char *argv[]){
    int sock,equipMain;
    struct sockaddr_in server;
    char *branchName , *machineName; char *server_reply=(char*)malloc(2000*sizeof(char));
    char message[64];
    char reply[64];
    //Create socket
    sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");
     
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons( 8888 );
 
    //Connect to remote server
    if ((equipMain=connect(sock , (struct sockaddr *)&server , sizeof(server))) < 0){
        perror("connect failed. Error");
        return 1;
    }
     
    puts("Connected\n");
     
    //keep communicating with server
    for(;;){
        branchName=getBranchName();strcpy(message,branchName);
        machineName=getMachineName(); strcat(message,",");
        strcat(message,machineName);strcat(message,"\n");
        
        //Send some data
        if(send(sock,message,strlen(message)+1,0)<0){
            perror("Can't send");
        }
        //Receive a reply from the server
        if( recv(sock , reply , 2000 , 0) < 0)
        {
            puts("recv failed");
            //break;
        }
        puts(reply);
    }
        close(sock);
     
      return 0;
}

char* getBranchName(){
    char* branch = (char*)malloc(32*sizeof(char));
    printf("Enter Branch Name: ");
    scanf("%s" , branch);
    return branch;
}
char* getMachineName(){
    char* machineName = (char*)malloc(32*sizeof(char));
    printf("Enter Machine Name:");
    scanf("%s", machineName);
    return machineName;
}