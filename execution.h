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


#define SIZEKB 1<<10 // 1 KB
#define SIZEMB 1<<20 // 1 KB
#define PIPE_INPUT 1
#define PIPE_OUTPUT 0
#define STD_INPUT 0
#define STD_OUTPUT 1
using namespace std;






//PIPED EXECUTE
void pipedExecution(char*** argumentMatrix){


	//CREATE PIPES
	int pipes[totalCommands][2];	
	
	for(int p=0;p<totalCommands;p++){
		//printf("pipe created\n");	
		pipe(pipes[p]);
		
	
	}	
	
	
	int parent=getpid();
	
	int  command;


	for(command=0;command<(totalCommands-1);command++){
	
		if(parent==getpid()){
		
		
			if(fork()==0){
				
				
				if(command==0){//first child
					//printf("First child %d\n",command);
				
				
					int nextPipe=command;
				
		
					for(int p=0;p<totalCommands;p++){
		
						if(p!=nextPipe){
				
							close(pipes[p][PIPE_INPUT]);
							close(pipes[p][PIPE_OUTPUT]);
			
						}
					else{
				
							close(pipes[p][PIPE_OUTPUT]);
			
			
						}
		
		
					}
		
		
		
				dup2(pipes[nextPipe][PIPE_INPUT],STD_OUTPUT);
				execvp(argumentMatrix[command][0],argumentMatrix[command]);
				/**/	
		
				
				
				}
				else{//middle child
					//printf("Second child %d\n",command);
				
				
						
					int previousPipe=command-1;
					int nextPipe=command;	
		
		
		
					for(int p=0;p<totalCommands;p++){
		
						if(p!=previousPipe && p!=nextPipe){
				
							close(pipes[p][PIPE_INPUT]);
							close(pipes[p][PIPE_OUTPUT]);	
				
			
						}
						else if(p==previousPipe){
				
							close(pipes[p][PIPE_INPUT]);
				
						}
						else if(p==nextPipe){
				
							close(pipes[p][PIPE_OUTPUT]);
				
						}
			
		
					}
		
		
					dup2(pipes[previousPipe][PIPE_OUTPUT],STD_INPUT);
					dup2(pipes[nextPipe][PIPE_INPUT],STD_OUTPUT);
		
					execvp(argumentMatrix[command][0],argumentMatrix[command]);
					/**/	
				
				
				
				
				}
			
			}
			
			
		
		}
	
	}
	
	
	
	//PARENT TAKES CARE OF LAST COMMAND	
	
	
	//printf("Parent %d\n",command);
	
	int previousPipe=command-1;
	
	
	for(int p=0;p<totalCommands;p++){
	
		if(p!=previousPipe){
			close(pipes[p][PIPE_INPUT]);
			close(pipes[p][PIPE_OUTPUT]);		
		
		
		}
		else{
			
			close(pipes[p][PIPE_INPUT]);	
		
		}
	
	}
	
	
	
	dup2(pipes[previousPipe][PIPE_OUTPUT],STD_INPUT);
	execvp(argumentMatrix[command][0],argumentMatrix[command]);
	/**/
	

}


//EXECUTE
int execute(char*** argumentMatrix){

	pid_t PID;
	pid_t waitPID;
	int status;
	const char* exitComparison="exit";
	

	
	
	PID=fork();
	if(PID==0){
	
		pipedExecution(argumentMatrix);
	

	}
	else if(PID<0){
		fprintf(stderr,"\nforking error\n");
			
		
	
	}
	else if (PID>0){
		
		wait(NULL);

	}


	return 1;
	
	/**/
}



