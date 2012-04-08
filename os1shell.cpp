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


char **history;
int histIndex = 0;
int histSize = 0;

void registerSignalAction(int sig, void (*func)(int)){
	struct sigaction act;
	act.sa_handler = func;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	sigaction(sig, &act, NULL);
}

void printSignal(int sig){
	cerr << "Signal #" << sig << " was handled" << endl;
	perror("OS1Shell");
}

void waitOnChildExit(int sig){
	wait(0);
}



void addToHistory(char* command, char** history, int histIndex){
	//add to history, overwriting oldest one if full
	if(history[histIndex] != NULL)
			delete[] history[histIndex];
	history[histIndex] = new char[65];
	strcpy(history[histIndex], command);

}

int handleArrows(){
	unsigned char c = getchar();
	int i = c;
	if (c == '['){
		c = getchar();
		if(c == 'A') return -5;
		else if(c == 'B') return -6;
		else return i;
	}
	return i;
}

int getKeyPress(){
	int c = getchar();
	if(c == 27) return handleArrows();
	else
		return c;

}

void clearLine(){
	cout << string(65, '\b');     //backspaces the line away
	cout << string(65, ' ');
	cout << string(65, '\b');
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

void handleCtrlC(int sig){
	cout << endl;
	printHistory(history, histIndex);
}

void sigHandleSetup(){

	struct sigaction act;
	act.sa_handler = printSignal;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	sigaction(1, &act, NULL);
	sigaction(4, &act, NULL);
	sigaction(5, &act, NULL);
	sigaction(6, &act, NULL);
	sigaction(7, &act, NULL);
	sigaction(8, &act, NULL);
	sigaction(10, &act, NULL);
	sigaction(11, &act, NULL);
	sigaction(12, &act, NULL);
	sigaction(13, &act, NULL);
	sigaction(14, &act, NULL);
	sigaction(15, &act, NULL);
	sigaction(20, &act, NULL);
	sigaction(21, &act, NULL);
	sigaction(22, &act, NULL);
	sigaction(23, &act, NULL);
	sigaction(24, &act, NULL);
	sigaction(25, &act, NULL);
	sigaction(26, &act, NULL);
	sigaction(27, &act, NULL);
	sigaction(28, &act, NULL);
	sigaction(29, &act, NULL);
	sigaction(30, &act, NULL);
	sigaction(31, &act, NULL);

	struct sigaction act2;
	act2.sa_handler = handleCtrlC;
	sigemptyset(&act2.sa_mask);
	act2.sa_flags = 0;
	sigaction(2, &act2, NULL);


	struct sigaction act3;
	act3.sa_handler = SIG_IGN;
	sigemptyset(&act3.sa_mask);
	act3.sa_flags = SA_NOCLDWAIT;
	sigaction(SIGCHLD, &act3, NULL);
}

int main() {


	history = new char *[20];
	for(int i = 0; i<20; i++){
		history[i]= new char[65];
	}
	initTerm();
	sigHandleSetup();
	while(1){
		cout << "OS1Shell -> ";
		char userin[65];
		bool runInBackground = false;
		initTerm();
		int count=0;
		int arrowHistoryPos = -1;
		int ch=getKeyPress();
		bool gotReturn = false;

		while(count<64 && ch!='\n' && ch!='\r'){
			if(ch == 4){
				cout << endl;
				resetTerm();
				return 0;
			}
			else if (ch == -5){
				if (arrowHistoryPos == -1){    //history mode has not been activated
					arrowHistoryPos = histIndex;
				}
			    arrowHistoryPos--;
			    if (arrowHistoryPos == -1){
					arrowHistoryPos = histSize-1;    //wrap around arrow index
				}
				if(arrowHistoryPos == histIndex){
					ch = getKeyPress();  //if there is no earlier history, do nothing and wait for next character
					continue;
				}
				else{
					if (history[arrowHistoryPos] != NULL){
						if(arrowHistoryPos >= 0){
							clearLine();
							strcpy(userin, history[arrowHistoryPos]);
							printf(userin, stdin);
							count = strlen(userin) -1;
							ch = getKeyPress();
							continue;
						}
					}


				}
			}

			else if (ch == -6){
			    if(arrowHistoryPos == -1){
					ch = getKeyPress();  //if the up arrow has not been pressed, we can't go down. ignore arrow.
					continue;
				}
				arrowHistoryPos++;
				if (arrowHistoryPos == histSize){
					arrowHistoryPos = 0;    //wrap around arrow index
				}
				else if(arrowHistoryPos == histIndex){
					clearLine();
					arrowHistoryPos = -1;
					ch = getKeyPress();
					continue;
				}
				else{
					clearLine();
					strcpy(userin, history[arrowHistoryPos]);
					printf(userin, stdin);
					count = strlen(userin) -1;
					ch = getKeyPress();
					continue;

				}
			}
			else{
				if(ch == 127){
					if (count > 0){
						printf("\b \b");
						count--;
					}
					ch = getKeyPress();
					continue;
				}
				else if(ch == -1) {
					ch = getKeyPress();
					continue;
				}
				else{
					userin[count]=ch;
					count++;
					cout << (unsigned char)(ch);
					ch=getKeyPress();
					continue;
				}
			}

		}
		cout << endl;

		if (count == 64 && userin[63] != '\n'){
			cerr << "Too many characters "<< endl;
			continue;
		}
		userin[count]='\0';


		if (strlen(userin)==0){
			continue;
		}

		char historyCommand[] = "history";
		if(strcmp(userin, historyCommand) == 0){
			printHistory(history, histIndex);
			continue;
		}
		else addToHistory(userin, history, histIndex);


		for(int i = 0; i<int(strlen(userin)); i++){
			if(userin[i] == '&'){
				runInBackground = true;
				userin[i] = 0;
			}
		}

		if (strlen(userin)==0){
			continue;
		}

		char* tokens[strlen(userin)];
		char* token;
		token = strtok(userin, " ");
		int i = 0;
		while (token != NULL){
			tokens[i] = token;
			i++;
			token = strtok(NULL, " ");
		}
		tokens[i] = NULL;

		char* progName = tokens[0];  			  // set the program name to the first argument

		if(strlen(progName)>0){

			histIndex = histIndex+1 % 20;
			if(histSize < 20) histSize++;
			int pid = fork();
			if (pid == 0){
				int status = execvp(progName, tokens);
				if(status == -1) perror("Execvp error");
			}
			else if (pid > 0){
				if(!runInBackground)
					waitpid(pid, 0, 0);
			}else{
				perror("Fork error");
			}
		}
	}

	for( int i = 0 ; i < 20 ; i++ )
	delete[] history[i];
	delete[] history;


	resetTerm();
	return EXIT_SUCCESS;
}











