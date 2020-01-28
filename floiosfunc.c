/*
============================================================================
onoma: Azelis Thomas

floiosfunc.c
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

/*================================welcome========================*/				
void welcome() {
        printf("\nWelcome to shell\n");
}
/*======================================printPrompt================*/
void printPrompt() {
        printf("%s :> ",getcwd(currentDirectory, 1024));
}
/*============================================lookupPath===================*/
/* 1.looks up path using environment variable PATH
   2.function looks up is command is actually in path
   3.if it finds command in that path, it will return path*/
char * lookupPath(char **argv, char **dir) {
        char *result;
	int i;
        char pName[MAX_PATH_LEN];
        if( *argv[0] == '/') {
                return argv[0];
        }
        else if( *argv[0] == '.') {
                if( *++argv[0] == '.') {
                         if(getcwd(pName,sizeof(pName)) == NULL)
                                 perror("getcwd(): error\n");
                         else {
                                 *--argv[0];
                                 asprintf(&result,"%s%s%s",pName,"/",argv[0]);
                         }
                         return result;
                }
                *--argv[0];
                if( *++argv[0] == '/') {
                        if(getcwd(pName,sizeof(pName)) == NULL)
                                perror("getcwd(): error\n");
                        else {
                                asprintf(&result,"%s%s",pName,argv[0]);
                        }
                        return result;
                }
        }

        /* look in PATH directories, use access() to see if the file is in the dir*/
        for(i = 0 ; i < MAX_PATHS ; i++ ) {
                if(dir[i] != NULL) {
                        asprintf(&result,"%s%s%s", dir[i],"/",argv[0]);
                        if(access(result, X_OK) == 0) {
                                return result;
                        }
                }
                else continue;
        }

        fprintf(stderr, "%s: command not found\n", argv[0]);
        return NULL;
}

/*===========================================readcommand===========================*/


int readCommand(char * buffer, char * commandInput) {
        int debug = 0;
        buf_chars = 0;


        while((*commandInput != '\n') && (buf_chars < LINE_LEN)) {
                buffer[buf_chars++] = *commandInput;
                *commandInput = getchar();
        }
        buffer[buf_chars] = '\0';
        return 0;
}


