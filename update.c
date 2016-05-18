 
#include <stdio.h>
#include <string.h>    //strlen
#include <sys/socket.h>
#include <arpa/inet.h> //inet_addr
#include <unistd.h>    //write
#include <time.h>
#include <stdlib.h>

void waitFor (unsigned int secs) {
    unsigned int retTime = time(0) + secs;   // Get finishing time.
    while (time(0) < retTime);               // Loop until it arrives.
}



int main(){
	char *branch, *machine,*amount;
	char character;
	char info[1024],invent[4];
	int number;
	FILE*inventFile;
	unsigned int secs=5;
	if((inventFile=fopen("inventory.txt","r+"))==NULL){
          perror("error:");
    }
	//for(;;){
		void waitFor (secs);
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
	                            break;
	                        }
	                }
	            }
	        }
    //}
	 fclose(inventFile);
}