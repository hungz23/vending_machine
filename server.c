/*
    C socket server example
*/
 
#include <stdio.h>
#include <string.h>    //strlen
#include <sys/socket.h>
#include <arpa/inet.h> //inet_addr
#include <unistd.h>    //write
#include <time.h>
#include <stdlib.h>

typedef struct Inventory_t{
    char machineName[32];
    char branchName[32];
    int  amount;
}Inventory;


void initInventFile();
void waitFor (unsigned int secs);

int main(int argc , char *argv[]){
    int socket_desc , client_sock , c , read_size, confirm=0;
    int counter;
    int number;
    struct sockaddr_in server , client;
    char client_message[2000];
    char machineError[]="Wrong Machine"; char success[]="Successfully!";
    char *branchName, *machineName;
    FILE* inventFile, *historyFile;
    Inventory* inventory;
    char back=8,character;
    char invent[4];

    unsigned int secs=5;

    time_t t = time(NULL);
    struct tm tm;

    char info[1024];char*machine;char*branch;char*amount;
    // Create file pointer
    
    
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

    //test
    
    if((inventFile=fopen("inventory.txt","r+"))==NULL){
          perror("error:");
    }
   
        fclose(inventFile);
    //test
     
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
                
                //read from socket
                for(;;){
                    read_size = recv(client_sock , client_message , 32 , 0);
                    branchName=strtok(client_message,",");machineName=strtok(NULL,"\n");
                    puts(branchName);puts(machineName);
                    puts("hung");

                    if((inventFile=fopen("inventory.txt","r+"))==NULL){
                        perror("No database!\n");
                    }

                    while(!feof(inventFile)){
                        fgets(info,2000,inventFile);
                        machine=strtok(info,",");printf("machine:%s\n",machine);
                        branch=strtok(NULL,",");
                        amount=strtok(NULL,"\n ");number=atoi(amount);number--;
                        
                        if(strcmp(machine,machineName)==0 && strcmp(branch,branchName)==0){
                            write(client_sock,success,strlen(success)+1);
                            confirm=1;
                            break;
                        }
                    }
                    
                    
                    if(confirm==1){
                        //update database
                        sprintf(invent,"%d",number);
                        fseek(inventFile,-4,SEEK_CUR);
                        while((character=fgetc(inventFile))!='\n'){
                                if(isdigit((int)character)){
                                    fseek(inventFile,-1,SEEK_CUR);
                                    if(number==9) strcat(invent," ");
                                    fputs(invent,inventFile);
                                    break;
                                }
                        }
                        confirm=0;
                        
                    }else{
                        write(client_sock,machineError,strlen(machineError)+1);
                        close(client_sock);
                    }
                    
                    if((historyFile=fopen("history.txt","r+"))==NULL){
                        perror("can't write in file");
                    }
                    //write history
                    
                        tm = *localtime(&t);
                        fseek(historyFile,0,SEEK_END);
                        fprintf(historyFile,"%s\t%s\t%d-%d-%d %d:%d:%d\n",machineName,branchName,  tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
                        fclose(historyFile);
                        fseek(inventFile,0,SEEK_SET);
                     while(1){
                        fgets(info,2000,inventFile);
                        if(feof(inventFile)) break;
                        machine=strtok(info,",");printf("machine:%s\n",machine);
                        branch=strtok(NULL,",");
                        amount=strtok(NULL,"\n ");number=atoi(amount);
                        
                        if(number<=3){
                            sprintf(invent,"%d",number+10);
                            fseek(inventFile,-4,SEEK_CUR);
                            while((character=fgetc(inventFile))!='\n'){
                                    if(isdigit((int)character)){
                                        fseek(inventFile,-1,SEEK_CUR);
                                        fprintf(inventFile,"%s",invent);
                                        fseek(inventFile,1,SEEK_CUR);
                                        printf("Delivery....\n");
                                        sleep(5);
                                        printf("Done!......\n");
                                        break;
                                    }
                            }
                        }
                    }

                   
                    
                    fclose(inventFile);

                    if(read_size == 0){
                        puts("Client disconnected");
                        fflush(stdout);
                    }
                    else if(read_size == -1){
                        perror("recv failed");
                    }
                }
                close(client_sock);
        }

    }

    

    
    return 0;
}

void waitFor (unsigned int secs) {
    unsigned int retTime = time(0) + secs;   // Get finishing time.
    while (time(0) < retTime);               // Loop until it arrives.
}