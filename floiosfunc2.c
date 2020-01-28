/*
============================================================================
onoma: Azelis Thomas

floiosfunc2.c
============================================================================
*/




#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/types.h>
#include "floios.h"

/*=====================executefileinCommand==============*/
/*rediraction "<"*/


int executeFileInCommand(char * commandName, char * argv[], char * filename) {
        int pipefd[2];
	FILE * myFile;
        char string;

        if(pipe(pipefd)) {
                perror("pipe");
                exit(127);//127:command not found
        }

        switch(fork()) {
        case -1:
                perror("fork()");
                exit(127); //127: command not found
        case 0:
        close(pipefd[0]); 
        dup2(pipefd[1], 1); 
        close(pipefd[1]); 
          

          myFile = fopen (filename , "r");
          if (myFile == NULL) perror ("Error opening file");
          else {

                 while ((string=fgetc(myFile)) != EOF) {
                                 putchar(string); 
                         }
          fclose (myFile);
          }
          exit(EXIT_SUCCESS);

    default:

        close(pipefd[1]); 
        dup2(pipefd[0], 0); 
        close(pipefd[0]); 

        execve(commandName, argv, 0);
        perror(commandName);
        exit(125);//125:failure

        }

        return 0;
}
/*============================executefileoutcommand==================*/
/*redirection ">" */



int executeFileOutCommand(char * commandName, char * argv[], char * filename) {
	int def = dup(1);
	int pid;
    //open a file
	int file = open(filename, O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU | S_IRGRP | S_IROTH);
    if(file < 0) return 1;

    //redirect stdout to the file using dup2
    if(dup2(file,1) < 0) return 1;
    if( pid = fork() == 0) {
            close(file);
            close(def);
            execve(commandName, argv,0);
            return 0;
    }
    dup2(def, 1);
    close(file);
    close(def);
    wait(NULL);
         close(file);
        return 0;
}
/*=============================executepipecommand======*/
/*pipe with "|"*/



void executePipedCommand(char *argvA[], char *argvB[], char * nameA, char * nameB) {
        int pipefd[2];

        if(pipe(pipefd)) {
                perror("pipe");
                exit(127);
        }

        switch(fork()) {
        case -1:
                perror("fork()");
                exit(127);
        case 0:
        close(pipefd[0]); 
        dup2(pipefd[1], 1); 
        close(pipefd[1]);   

        execve(nameA, argvA, 0);
 
        perror(nameA);
        exit(126);
    default:
        /* parent */

        
        close(pipefd[1]); 
        dup2(pipefd[0], 0); 
        close(pipefd[0]); 
        
        execve(nameB, argvB, 0);
        perror(nameB);
        exit(125);

        }
}
