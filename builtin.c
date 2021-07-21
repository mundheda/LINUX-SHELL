#include "headers.h"

#define BUF_SIZE 1024

int execBuiltin(int argC, char *argV[], char *tilda)
{
	if (!strcmp(argV[0], "cd")){
		execCD(argV, argC, tilda);
		return 0;
	}
	else if(!strcmp(argV[0], "echo")){
		execECHO(argV, argC);
		return 0;
	}
	else if(!strcmp(argV[0], "pwd")){
		execPWD(tilda);
		return 0;
	}
	return 1;
}

void execPWD(char *tilda)
{
	char *currWD = malloc(BUF_SIZE);
	getcwd(currWD, BUF_SIZE);
	printf("%s\n", currWD);

	free(currWD);
}

void execECHO(char **argV, int argC)
{
	for(int i = 1; i < argC; i++){
		for(int j = 0; argV[i][j] != '\0'; j++){
			if(argV[i][j] != '"'){
				printf("%c", argV[i][j]);
			}
		}
		printf(" ");
	}
	printf("\n");
}

void execCD(char **argV, int argC, char *tilda)
{
	if(argC > 2){
		printf("cd: string not in pwd: %s\n", argV[0]);
		return;
	}
	char *path = malloc(BUF_SIZE);
	
	if(argC == 1){
		strcpy(path, tilda);
		if(chdir(path) != 0){
			perror("cd");
		}
		return;
	}
	
	if(argV[1][0] == '~'){
		int i,j;
		for(i=0; i < strlen(tilda); i++){
			path[i] = tilda[i];
		}
		for(j = 1;j < strlen(argV[1]); j++){
			path[i++] = argV[1][j];
		}
	}
	else {
		strcpy(path, argV[1]);
	}

	if(chdir(path) != 0){
		perror("cd");
	}

	free(path);
}