/*
============================================================================
onoma: Azelis Thomas

floiosfunc1.c
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


/*===========================parsepath=================*/
/*populates "pathv" with environment variable PATH*/
int parsePath(char* dirs[]){
        int debug = 0;
        char* pathEnvVar;
        char* thePath;
        int i;
	char* pch;
	int j=0;

        for(i=0 ; i < MAX_ARGS ; i++ )
                dirs[i] = NULL;
        pathEnvVar = (char*) getenv("PATH");/* The  getenv()  function searches the environment list to find the environment variable  							name, and returns a pointer to the corresponding string.*/
        thePath = (char*) malloc(strlen(pathEnvVar) + 1);
        strcpy(thePath, pathEnvVar);

        
        pch = strtok(thePath, ":");
        while(pch != NULL) {
                pch = strtok(NULL, ":");
                dirs[j] = pch;
                j++;
        }

        /*===================== debug ===============*/
        // print the directories if debugging
        if(debug){
                printf("Directories in PATH variable\n");
                for(i=0; i<MAX_PATHS; i++)
                        if(dirs[i] != '\0')
                                printf(" Directory[%d]: %s\n", i, dirs[i]);
        }
}
/*====================================parsecommand=====================*/
/* parses commandLine into command.argv and command.argc*/
int parseCommand(char * commandLine, struct command_t * command) {
        char * pch;
        pch = strtok (commandLine," ");
        int i=0;
        while (pch != NULL) {
                command->argv[i] = pch;
                pch = strtok (NULL, " ");
                i++;
        }
        command->argc = i;
        command->argv[i++] = NULL;
        return 0;
}
