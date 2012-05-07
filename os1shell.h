/*
 * os1shell.h
 *
 *  Created on: Apr 7, 2012
 *      Author: jeremy
 */

#ifndef OS1SHELL_H_
#define OS1SHELL_H_

/*
 * Globals
 */
char **history;
int histIndex = 0;
int histSize = 0;

/*
 * Prints the signal number and errno information
 */
void printSignal(int sig);

/*
 * Adds the command to the specified history array at position histIndex
 * If the history is full, the oldest item will be overwritten
 */
void addToHistory(char* command, char** history, int histIndex);

/*
 * Deals with input that may be an arrow key. Returns special value to main
 */
int handleArrows();

/*
 * gets the key presses, calling handleArrows when escape key is received
 */
int getKeyPress();

/*
 * Clears the current line in the terminal
 */
void clearLine();

/*
 * Initializes the termina for proper program operation
 */
void initTerm();

/*
 * Undoes the actions performed in initTerm()
 */
void resetTerm();

/*
 * Prints the history, one item per line. histIndex points to the oldest item
 */
void printHistory(char** history, int histIndex);

/*
 * Handles Ctrl-C and prints the history
 */
void handleCtrlC(int sig);

/*
 * Called by main to set up signal handlers for all required signals.
 */
void sigHandleSetup();


#endif /* OS1SHELL_H_ */
