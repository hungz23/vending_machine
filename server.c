/*
    C socket server example
*/
 
#include <stdio.h>
#include <string.h>    //strlen
#include <sys/socket.h>
#include <arpa/inet.h> //inet_addr
#include <unistd.h>    //write
typedef struct Inventory_t{
    char machineName[32];
    char branchName[32];
    int  amount;
}Inventory;

void decreaseInventory(FILE *inventFile, char* machineName, char* branchName);
void increaseInventory(FILE*inventFile, char *machineName, char* branchName);
Inventory searchInventory(FILE*inventFile, char* machineName, char* branchName);
void addHistory(FILE* historyFile, char* machineName, char* branchName);
int main(int argc , char *argv[])
{
    int socket_desc , client_sock , c , read_size;
    int counter;
    struct sockaddr_in server , client;
    char client_message[2000];
    char *branchName, *machineName;
    FILE* inventFile;
    // Create file pointer
    if((inventFile=fopen("inventory.txt","r+"))==NULL){
        printf("error!\n");
        exit(1);
    }
    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");
     
    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( 8888 );
     
    //Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        //print the error message
        perror("bind failed. Error");
        return 1;
    }
    puts("bind done");
     
    //Listen
    listen(socket_desc , 3);
     
    //Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
    int pid;
     
    //accept connection from an incoming client
    for(;;){
        client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
        pid=fork();
        if(pid<0){
            close(client_sock);
            continue;
        }else if(pid>0){
            close(client_sock);
            counter++;
            printf("here2\n");
            continue;
        }else{
                if (client_sock < 0){
                    perror("accept failed");
                    return 1;
                }
                puts("Connection accepted");
                read_size = recv(client_sock , client_message , 2000 , 0);
                write(client_sock , client_message , strlen(client_message));
                branchName=strtok(client_message,",");machineName=strtok(NULL,"\0");
                puts(branchName);puts(machineName);
                 
                //Receive a message from client
                //while( (read_size = recv(client_sock , client_message , 2000 , 0)) > 0 ){
                    //Send the message back to client
                  //  write(client_sock , client_message , strlen(client_message));
                //}
                 
                if(read_size == 0){
                    puts("Client disconnected");
                    fflush(stdout);
                }
                else if(read_size == -1){
                    perror("recv failed");
                }
                close(client_sock);
        }
    }
     
    return 0;
}