//============================================================================
// Name        : os1shell.cpp
// Author      : Jeremy Williams
// Version     : 1.0
// Description : A custom shell program
// Sources	   : cplusplus.com (iostream/strtok information)
//============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cstring>
#include <cerrno>
#include <wait.h>
#include <termios.h>
#include <unistd.h>
using namespace std;

int sigHandleSetup(){
	//register all signals except Ctrl-/ using registerSignalAction

}


void registerSignalAction(int sig, void *func){
	//create the struct, fill it with data, and call sigaction
}

void printSignal(int sig){
	// print the signal associated with the number (use a switch)
}

void handleCtrlC(int sig){
	//print the history when ctrl-c is found
}

void waitOnChildExit(int sig){
	// call wait when the child exits to prevent zombie processes
}

void addToHistory(char* command, char** history, int histIndex){
	//add to history, overwriting oldest one if full
	if(history[histIndex] != NULL)
			delete[] history[histIndex];
	history[histIndex] = new char[65];
	strcpy(history[histIndex], command);

}

int handleArrows(){
	int c = getchar();
	if (c == '['){
		c = getchar();
		switch(c){
		case 'A': return -5;
		case 'B': return -6;
		default: return -4;
		}
	}
}

int getKeyPress(){
	int c = getchar();
	if(c == 27) return handleArrows();
	else
		return c;

}

void clearLine(){
	cout << string(65, '\b');     //backspaces the line away
	cout << "OS1Shell -> ";

}


void initTerm(){
	struct termios term;
	tcgetattr(STDIN_FILENO, &term);
	term.c_lflag &= ~ICANON;
	term.c_lflag &= ~ECHO;
	tcsetattr(0, TCSANOW, &term);
}

void resetTerm(){
	struct termios term;
	tcgetattr(STDIN_FILENO, &term);
	term.c_lflag |= ICANON;
	term.c_lflag |= ECHO;
	tcsetattr(0, TCSANOW, &term);
}

void printHistory(char** history, int histIndex){
	for(int i = histIndex; i<20; i++){
		if (history[i] != NULL && strlen(history[i]) != 0)
			cout << history[i] << endl;
	}
	for (int i = 0; i<histIndex; i++){
		if (history[i] != NULL && strlen(history[i]) != 0)
				cout << history[i] << endl;
	}
}

int main() {

	char **history;
	int histIndex = 0;
	int histSize = 0;
	history = new char *[20];
	for(int i = 0; i<20; i++){
		history[i]= new char[65];
	}
	initTerm();
	sigHandleSetup();
	while(1){
		cout << "OS1Shell -> ";
		char userin[65];
		char *command = NULL;
		bool runInBackground = false;
		initTerm();
		int count=0;
		int arrowHistoryPos = -1;
		int ch=getKeyPress();
		bool restart = false;
		bool gotReturn = false;

		while(count<64 && ch!='\n' && ch!='\r'){
			switch(ch){
			case 4:
				cout << endl;
				resetTerm();
				return 0;
			case -5:
				if (arrowHistoryPos == -1){    //history mode has not been activated
					arrowHistoryPos = histIndex;
				}
			    arrowHistoryPos--;
			    if (arrowHistoryPos == -1){
					arrowHistoryPos = histSize-1;    //wrap around arrow index
				}
				if(arrowHistoryPos == histIndex){
					ch = getKeyPress();  //if there is no earlier history, do nothing and wait for next character
					break;
				}
				else{
					clearLine();
					char histCommand[65];
					strcpy(histCommand, history[arrowHistoryPos]);
					printf(histCommand, stdin);
					command = history[arrowHistoryPos];
					count = strlen(history[arrowHistoryPos]) -1;


				}

			case -6:
			    if(arrowHistoryPos == -1){
					ch = getKeyPress();  //if the up arrow has not been pressed, we can't go down. ignore arrow.
					break;
				}
				arrowHistoryPos++;
				if (arrowHistoryPos == histSize){
					arrowHistoryPos = 0;    //wrap around arrow index
				}
				else if(arrowHistoryPos == histIndex){
					clearLine();
					arrowHistoryPos = -1;
					break;
				}
				else{
					clearLine();
					char histCommand[65];
					strcpy(histCommand, history[arrowHistoryPos]);
					printf(histCommand, stdin);
					command = history[arrowHistoryPos];
					count = strlen(history[arrowHistoryPos]) -1;

				}
			case -4:
				// print invalid input, set count to zero, break
			default:
			    userin[count]=ch;
			    count=count+1;
			    putchar(ch);
			    ch=getKeyPress();

			}

		}
		cout << endl;
		userin[count]='\0';

		char line[65];

		if(command == NULL){
			strcpy(line, userin);
		}
		else{
		    strcpy(line, command);
		}



		//getsReturn = fgets(userin, 65, stdin);  //get up to 64 characters from user
		for(int i = 0; i<int(strlen(line)); i++){
			if(line[i] == '&'){
				runInBackground = true;
				line[i] = 0;
			}
		}


		if (strlen(line)==0){
			continue;
		}


		char* tokens[strlen(line)];
		char* token;
		token = strtok(line, " ");
		int i = 0;
		while (token != NULL){
			tokens[i] = token;
			i++;
			token = strtok(NULL, " ");
		}
		tokens[i] = NULL;

		char* progName = tokens[0];  			  // set the program name to the first argument
	    //cout << progName << endl;
		//cout << runInBackground << endl;
		//for (int j = 0; j<i; j++){
		//	cout << tokens[j] << endl;
		//}

		if(strlen(progName)>0){
			char historyCommand[] = "history";
			if(strcmp(progName, historyCommand) == 0){
				printHistory(history, histIndex);
				continue;
			}
			addToHistory(progName, history, histIndex);
			histIndex = histIndex+1 % 20;
			if(histSize < 20) histSize++;
			int pid = fork();
			if (pid == 0){
				int status = execvp(progName, tokens);
				cout << status << endl;
				cout << strerror(errno) << endl;
			}
			else if (pid > 0){
				if(!runInBackground)
					waitpid(pid, 0, 0);
			}else;
		}
	}

	for( int i = 0 ; i < 20 ; i++ )
	delete [] history[i] ;
	delete [] history;


	resetTerm();
	return EXIT_SUCCESS;
}











