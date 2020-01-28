/*
============================================================================
onoma: Azelis Thomas

floios.h
============================================================================
*/




#ifndef FLOIOS_H
#define FLOIOS_H

#define FALSE 0
#define TRUE 1
#define LINE_LEN 80
#define MAX_ARGS 64
#define MAX_ARG_LEN 64
#define MAX_PATHS 64
#define MAX_PATH_LEN 96
#define WHITESPACE " .,\t&"
#define STD_INPUT 0
#define STD_OUTPUT 1
#ifndef NULL
#define NULL 0
#endif

static char* currentDirectory;
static char commandInput = '\0';
static int buf_chars = 0;
static char *pathv[MAX_PATHS];
static char commandLine[LINE_LEN];

struct command_t {
        char *name;
        int argc;
        char *argv[MAX_ARGS];
};

void printPrompt();
void welcome();
int readCommand(char *commandLine, char *commandInput);
int parseCommand(char *commandLine, struct command_t *command);
char * lookupPath(char **, char **);
int parsePath(char **);
int executeFileInCommand(char *, char **, char *);
int executeFileOutCommand(char *, char **, char *);
void executePipedCommand(char **, char **, char *, char *);

#endif
