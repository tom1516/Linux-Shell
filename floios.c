/*
============================================================================
onoma: Azelis Thomas

floios.c
============================================================================
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include "floios.h"
struct command_t command;

/* ================clear screan================*/
void clear_the_Screen() {
        printf("\033[2J\033[1;1H");
}

/*=======================change directory========*/
void change_Dir() {
        if (command.argv[1] == NULL) {
                chdir(getenv("HOME"));
        } else {
                if (chdir(command.argv[1]) == -1) {
                        printf(" %s: no such directory\n", command.argv[1]);
                }
        }
}

/*======check internal commands=================*/
int check_Inter_Command() {

        if(strcmp("cd", command.argv[0]) == 0) {
                change_Dir();
                return 1;
        }
        if(strcmp("clear", command.argv[0]) == 0) {
                clear_the_Screen();
                return 1;
        }

        return 0;
}


/* =========commands without < > | &==========*/
int execute_Command() {

        int ch_pid;
        int status;
        pid_t nextPID;


        ch_pid = fork();
        if(ch_pid < 0 ) {
                fprintf(stderr, "Fork fails: \n");
                return 1;
        }
        else if(ch_pid==0) {
                /* child */
                execve(command.name, command.argv,0);
                printf("Child process failed\n");
                return 1;
        }
        else if(ch_pid > 0) {
                /* parent */

                do {
                        nextPID = waitpid(ch_pid, &status, WUNTRACED | WCONTINUED);
            if (nextPID == -1) {
                perror("waitpid");
                exit(EXIT_FAILURE);
            }

            if (WIFEXITED(status)) {
                //printf("exited, status=%d\n", WEXITSTATUS(status));
                return 0;
            } else if (WIFSIGNALED(status)) {
                printf("killed by signal %d\n", WTERMSIG(status));
            } else if (WIFSTOPPED(status)) {
                printf("stopped by signal %d\n", WSTOPSIG(status));
            } else if (WIFCONTINUED(status)) {
                printf("continued\n");
            }
                } while (!WIFEXITED(status) && !WIFSIGNALED(status));
                return 0;
        }

}

/*===========commands with | is called from processcommand=======================*/
int pro_Pipe_Command(int i) {
        char *A[5];
        char *B[5];
        char *nameA, *nameB;
	int pid;
	int status;
        int p;
	int j,k=0;
        for(p=0;p<i;p++) {
                A[p]= command.argv[p];
        }
        A[p]=NULL;
        nameA = lookupPath(A, pathv);

        for(j=i+1; j<command.argc; j++) {
                B[k] = command.argv[j];
                k++;
        }
        B[k]=NULL;
        nameB = lookupPath(B, pathv);

        
        fflush(stdout);

    switch ((pid = fork())) {
    case -1:
        perror("fork");
        break;
    case 0:
        /* child */
            executePipedCommand(A, B, nameA, nameB);
        break;
    default:
        /* parent */
        pid = wait(&status);
        return 0;
    }
    return 1;
}

/*=======command with ">" rediraction =========================*/
int pro_FileOut_Command(int i) {

        char *argv[5];
        char *commandName;
        int j;
        for(j=0; j<i; j++) {
                argv[j] = command.argv[j];
        }
        argv[j] = NULL;
        commandName = lookupPath(argv, pathv);

        return executeFileOutCommand(commandName, argv, command.argv[i+1]);
}

/*============ command with  "<" rediraction ===========*/
int pro_FileIn_Command(int i) {
        char *argv[5];
        char *commandName;

        int j;
        for(j=0; j<i; j++) {
                argv[j] = command.argv[j];
        }
        argv[j] = NULL;
        commandName = lookupPath(argv, pathv);

        int pid, status;
        fflush(stdout);

    switch ((pid = fork())) {
    case -1:
        perror("fork");
        break;
    case 0:
        /* child */
            executeFileInCommand(commandName, argv, command.argv[i+1]);
        break;
    default:
        /* parent;*/
        pid = wait(&status);
        return 0;
    }

        return 0;
}
/*=========================backgroundCommand with "&"===============*/

int background_Command(int i) {
	int pid;
	char *argv[5];
        char* filename = command.name;
	int j;
	int status;
        if (filename == NULL) {
                printf("Command not found.\n");
                return 1;
        }
	for(j=0;j<i;j++){
		argv[j]=command.argv[j];
	}
	argv[j]=NULL;
        pid = fork();
        /*check for error*/
        if (pid < 0) {
                printf("Error in the fork process.\n");
                return 1;
        } 
	if (pid==0) {
                 /*Child code*/
		execve(command.name,argv,0);
		printf("the command finished\n");
		return 1;
        } 


	if (pid>0){
			/*parent doesn't wait*/
			/*nothing to do*/	
        }
	return 1;
	}

/*============pro_command searches for ">""<""|""&"=================================================*/
int pro_Command() {
        int i;
        for(i=0;i<command.argc; i++) {
                if(strcmp(command.argv[i], ">") == 0) {
                        return pro_FileOut_Command(i);
                }
                else if(strcmp(command.argv[i], "<") == 0) {
                        return pro_FileIn_Command(i);

                }
                else if(strcmp(command.argv[i], "|") == 0) {
                 	return pro_Pipe_Command(i);
                }
		else if(strcmp(command.argv[i],"&")==0){
			return background_Command(i);
        }
	}
        return execute_Command();
}

/*
============================================================================

                                  Main method of the programm

============================================================================
*/
int main(int argc, char* argv[]) {
        int i;
        int debug = 0;

        parsePath(pathv);
        welcome();
        while(TRUE) {
                printPrompt();

                commandInput = getchar(); 
                if(commandInput == '\n') {
                        continue;
                }
                else {
                        readCommand(commandLine, &commandInput); 

                        if((strcmp(commandLine, "exit") == 0))
                                break;

                        parseCommand(commandLine, &command);

                        if(check_Inter_Command() == 0) {
                                command.name = lookupPath(command.argv, pathv);

                                if(command.name == NULL) {
                                        printf("Stub: error\n");
                                        continue;
                                }

                                pro_Command();
                        }
                }
        }

        printf("\n");
        exit(EXIT_SUCCESS);
}
