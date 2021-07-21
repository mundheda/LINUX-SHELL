#include "headers.h"

#define DELIM_TOKEN " \t\r\f\a\n"
#define BUF_SIZE 1024  //assuming max length of input is BUF_SIZE

int getInput(char *inputString, char **commands)
{
	int size = 0;
	fgets(inputString, BUF_SIZE, stdin);

	inputString[strlen(inputString)-1] = '\0';	//removing extra '\n'
	
	char *found = strtok(inputString, ";");
	while( found != NULL ){
		commands[size] = (char *)malloc(BUF_SIZE);
		strcpy(commands[size++], found);
		found = strtok(NULL, ";");
	}
	return size;
}

int getargV(char *inputString, char **argV)
{
	int size = 0;
	char *found;
	found = strtok(inputString, DELIM_TOKEN);
	while( found != NULL ){
		argV[size] = (char *)malloc(BUF_SIZE);
		strcpy(argV[size++], found);
		found = strtok(NULL, DELIM_TOKEN);
    }
    argV[size] = NULL;

    free(found);
	
	return size;
}

