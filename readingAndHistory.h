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

int totalCommands=0;

//HISTORY
vector<string> history;
int historyIndex=0;


void addToHistory(char* argumentArray){
	
	if(history.size()!=0 && history[history.size()-1]==""){
		
		history.pop_back();
		
	}
	
	history.push_back(argumentArray);
	history.push_back("");
	historyIndex=history.size()-1;

}



int getch(void)
{
	struct termios oldt, newt;
	int ch;

	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	ch = getchar();
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

	return ch;
}









//READ LINE
char* read_line(){
	
	int bufferSize=1024;
	int position=0;
	char* buffer = (char*) malloc(sizeof(char)*bufferSize);
	int character;
	int arrow=0;
	
	if(!buffer){
		fprintf(stderr,"\nread_line memory allocation error\n");
		exit(EXIT_FAILURE);
	}
	
	while(1){
	
		character = getch();
		//GET UP or DOWN arrow 
		

		if (character == '\033') { // if the first value is esc
				
				
			    getch(); // skip the [
			    switch(getch()) { // the real value
        	case 'A'://UP
            	 	
            	
         	
            	//printf("up arrow");// code for arrow up
            	if(history.size()>0){
            	
		    		while(position!=0){
		    			printf("%s","\b \b");
		    			position--;            		
		    	
		    		}
		    		
		    		if(historyIndex!=0){
		    			
		    			historyIndex--;
		    			
		    		}
		    		
		    			strcpy(buffer,history[historyIndex].c_str());
		    			position=history[historyIndex].size();        			
		    			printf("%s",buffer);
        		}
        		
            	
            	break;
        	case 'B'://DOWN
        	
        		//printf("up arrow");// code for arrow up
        		
		    		if(history.size()>0){
		    		while(position!=0){
		    			printf("%s","\b \b");
		    			position--;            		
		    	
		    		}
		    		
		    		if(historyIndex!=history.size()-1){
		    			historyIndex++;
		    		}
		    		
		    			strcpy(buffer,history[historyIndex].c_str());
		    			position=history[historyIndex].size();        			
		    			printf("%s",buffer);        		
        		}
            		          	
            	
            	break;
        	case 'C'://RIGHT
            	// code for arrow right
            	break;
        	case 'D'://LEFT
            	// code for arrow left
            	break;
    			}
			}
			
		else if(character == 127){//backspace
		
			if(position!=0){
				printf("%s","\b \b");
			
				buffer[position]='\0';
				position--;
			}
		
		}
		


		
	
		else if(character==EOF || character=='\n'){
		
			buffer[position]='\0';
			return buffer;
		
		}
		
		else{
			printf("%c",character);
			buffer[position]=character;
			position++;	
		}
		
		
		if(position>=bufferSize){
			printf("invoked");
			bufferSize+=1024;
			buffer =  (char*) realloc(buffer,bufferSize);
			if(!buffer){
				fprintf(stderr,"\nread_line memory allocation error\n");
				exit(EXIT_FAILURE);					
			}			
		
		}	
	
	}

}








//SPLIT INTO COMMANDS

char** split_into_commands(char* initialString){
	
	int bufferSize = 64;
	char** commandArray = (char**) malloc(bufferSize*sizeof(char*));
	char* command;
	int position=0;
	totalCommands=0;
	
	//GRAB COMMAND FOR HISTORY
	addToHistory(initialString);
	
	if(!commandArray){
		fprintf(stderr,"\nsplit initialString memory allocation error\n");
		exit(EXIT_FAILURE);	
	}
	
	command = strtok(initialString,"|");
	while(command != NULL){
		totalCommands++;
		commandArray[position]=command;
		position++;	
	
		if(position>=bufferSize){
			bufferSize += 64;
			char** commandArray = (char**) realloc(commandArray,bufferSize*sizeof(char*));
			if(!commandArray){
				fprintf(stderr,"\nsplit initialString memory allocation error\n");
				exit(EXIT_FAILURE);	
			}		
		}
		
		command=strtok(NULL,"|");
		
	
	}

	return commandArray;







}
/**/




//SPLIT INTO ARGUMENTS
char** split_into_arguments(char* command){

	int bufferSize = 64;
	char** argumentArray = (char**) malloc(bufferSize*sizeof(char*));
	char* argument;
	int position=0;
	
	
	
	if(!argumentArray){
		fprintf(stderr,"\nsplit command memory allocation error\n");
		exit(EXIT_FAILURE);	
	}
	
	argument = strtok(command," \t\n");
	while(argument != NULL){
		argumentArray[position]=argument;
		position++;	
	
		if(position>=bufferSize){
			bufferSize += 64;
			char** argumentArray = (char**) realloc(argumentArray,bufferSize*sizeof(char*));
			if(!argumentArray){
				fprintf(stderr,"\nsplit command memory allocation error\n");
				exit(EXIT_FAILURE);	
			}		
		}
		
		argument=strtok(NULL," \t\n");
		
	
	}
	argumentArray[position] = NULL;
	return argumentArray;
	
}



