#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>

//error handling.
//if the input is longer than 512 character, ignored.


//this function reads the input from user and store each character in an array
char *read_char(void){

	int position = 0;
	//max is 512 character + 1 for null terminator;
	char *buffer = malloc(sizeof(char) * 513);
	int c;
	
	while (1) {
		c = getchar();
		//check every character reads;
		//EOF = control + d
		if (c == EOF || c == '\n') {
			buffer[position] = '\0';
			return buffer;
		} else {
			buffer[position] = c;
		}
		position++;
	}
}

//this function take the character array as an in put and tokenize
char **tokenize(char *line) {
	int size = 100;
	int position = 0;
	char *token;
	char **tokens = malloc(size * sizeof(char*));
	
	token = strtok(line, " ");
	while (token != NULL) {
		tokens[position] = token;
		position++;
		
		//if it exceeds the size;
		if (position >= size) {
			size = size * 2;
			tokens = realloc (tokens, size * sizeof(char*));
		}
		
		token = strtok(NULL, " ");
	}
	
	tokens[position] = NULL;
	return tokens;	
}


//buit-in functions: setenv, unsentenv, cd, exit,

//set environment variable USER to Z -> setenv("USER", "Z", 1)
//terminal input is: setenv var value
//not done. "If environment variable var does not exist, then kapish should create it."
int kapish_setenv (char **args) {
	
	if (args[2] == NULL) {
		setenv(args[1], " ", 1); 
	} else {
		setenv(args[1], args[2], 1);
		//printf("%s %s %s\n", args[0], args[1], args[2]);
	}
	
	
	return 1;
}

//destroy the environment variable var
//terminal input is: unsetenv var
int kapish_unsetenv(char **args) {
	unsetenv(args[1]);
	return 1;
}

//change directory to dir, if dir is missing, then change to home
int kapish_cd (char **args){
	
	if (args[1] == NULL){
		chdir(getenv("HOME"));
	} else {
		chdir(args[1]);
	}
	
	return 1;
}


//kapish should exit if the terminal input is exit;
int kapish_exit(char **args){
	return 0;
}


//fork(), execvp()
int kapish_init(char **args){

	pid_t pid, w;
	int status; 
	//fork a children process
	pid = fork();
	
	if (pid == 0) {
		//child process
		if (execvp(args[0], args) == -1) {
			printf("ERROR\n");
		}
		exit(EXIT_FAILURE);
	} else if (pid < 0) {
		//unsuccessful fork
		perror("kapish");
	} else {
		//parent process
		typedef void (*sighandler_t)(int);
		
		//signal(SIGINT, SIG_IGN);
		w = wait(&status);
	}
	
	return 1;
}



void kapish_loop (void) {
	char *line;
	char **args;
	int status;
	
	do {
	printf("? ");
	line = read_char();
	args = tokenize(line); 
	
	
	//execute the built in command first;
	if (strcmp(args[0] , "setenv") == 0) {
	
		status = kapish_setenv(args);
		
	} else if (strcmp(args[0], "unsetenv") == 0) {
	
		status = kapish_unsetenv(args);
		
	} else if (strcmp(args[0], "cd") == 0) {
	
		status = kapish_cd(args);
		
	} else if (strcmp(args[0], "exit") == 0){
	
		status = kapish_exit(args);
		
	} else {
	//fork(), exec()
		status = kapish_init(args);
	}
	
	
	free(line);
	free(args);
	
	} while (status);
}




int main (int argc, char **argv){
	//open kapishrc first;
	FILE *fp;
	fp = fopen(strcat(getenv("HOME"), ".kapishrc"), "r");
	
	kapish_loop();
	
	return EXIT_SUCCESS;
}



