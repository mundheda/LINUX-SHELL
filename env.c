#include "headers.h"

void execSetEnv(int argC, char *argV[])
{
	if(argC == 2) {
		if(setenv(argV[1], "", 1) == -1) {
			perror("setenv");
		}
		return;
	}
	if(argC == 3) {
		if(setenv(argV[1], argV[2], 1) == -1) {
			perror("setenv");
		}
		return;
	}
	perror("invalid number of arguments");
}

void execUnSetEnv(int argC, char *argV[]) 
{
	if(argC == 2) {
		if(unsetenv(argV[1]) == -1) {
			perror("unsetenv");
		}
		return;
	}
	perror("invalid number of arguments");
}