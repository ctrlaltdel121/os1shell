//============================================================================
// Name        : os1shell.cpp
// Author      : Jeremy Williams
// Version     :
// Copyright   : 
// Description : A custom shell program
//============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cstring>
#include <cerrno>
#include <wait.h>
using namespace std;


void execute(char* progName, char** args, bool bg){

	return;

}

int main() {
	while(1){
		printf("OS1Shell -> ");
		char userin[64], *getsReturn;
		bool runInBackground = false;
		getsReturn = fgets(userin, 64, stdin);  //get up to 64 characters from user
		for(int i = 0; i<int(strlen(userin)); i++){
			if(userin[i] == '&'){
				runInBackground = true;
				userin[i] = 0;
			}
			else if(userin[i] == '\n'){
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
	    //cout << progName << endl;
		//cout << runInBackground << endl;
		//for (int j = 0; j<i; j++){
		//	cout << tokens[j] << endl;
		//}

		if(strlen(progName)>0){
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
	return EXIT_SUCCESS;
}











