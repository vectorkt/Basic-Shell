#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/wait.h>
#include<signal.h>
#include<pthread.h>
#include<math.h>
#include<time.h>
#include<vector>
#include<string.h>
#include<termios.h>
#include"readingAndHistory.h"
#include"execution.h"

#define SIZEKB 1<<10 // 1 KB
#define SIZEMB 1<<20 // 1 KB

using namespace std;







//LOOP
void repl(){
	

	int status=1;
	
	do{
	
	
		//READ
		char* initialString;
		char** commandArray;
		char*** argumentMatrix=(char***) malloc(totalCommands*sizeof(char**));
		
		printf("-{@ ");
		initialString = read_line();
		if(initialString[0]=='\0'){
			
			printf("\n");
			continue;			

		}
		commandArray=split_into_commands(initialString);
		for(int c=0;c<totalCommands;c++){
 	
 			
 			argumentMatrix[c]=split_into_arguments(commandArray[c]);
 	
 		}
		
		
		printf("\n");
		
		const char* exitComparison="exit";
		if(strcmp("exit\0",argumentMatrix[0][0])==0){
			
			exit(0);
		}
		/**/
		
		//EXECUTE
		status = execute(argumentMatrix);
		
		free(initialString);
		free(commandArray);
		free(argumentMatrix);
		
	}while(status);//LOOP
	
	
	
	
}





int main(int argc,char** argv){
	
	repl();
		
	return 0;

}
