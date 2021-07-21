#include "headers.h"

//ONlY ONE OUTPUT AND ONE INPUT FILE SUPPORTED
int checkRedirect(int *argC, char *argV[], char *tilda) 
{
	int in = 0, out = 0, outtype = 0, newargC = -1;
	char *infile = malloc(1024);
	char *outfile = malloc(1024);

	for(int i = 0; i < *argC; i++) {
		if(!strcmp(argV[i], ">") || !strcmp(argV[i], ">>")) {
			out++;
			if(!strcmp(argV[i], ">>")) {
				outtype = 1;
			}
			if(out > 1) {
				perror("only one output redirection allowed");
				return 1;
			}
			if(i == *argC - 1) {
				perror("no file given");
				return 1;
			}
			strcpy(outfile, argV[i+1]);
			if(newargC == -1) {
				newargC = i;
			}
		}
		if(!strcmp(argV[i], "<")) {
			in++;
			if(in > 1) {
				perror("only one input redirection allowed");
				return 1;
			}
			if(i == *argC - 1) {
				perror("no file given");
				return 1;
			}
			strcpy(infile, argV[i+1]);
			if(newargC == -1) {
				newargC = i;
			}
		}
	}
	if(in) {
		if(infile[0] == '~') {
			char *temp = malloc(1024);
			strcpy(temp, infile);
			strcpy(infile, tilda);
			strcat(infile, temp+1);
			free(temp);
		}

		int fd1 = open(infile, O_RDONLY);
		if(fd1 < 0) {
			perror("Failed to open file / no such file exists");
			return 1;
		}
		close(0);
		if(dup2(fd1, 0) != 0) {
			perror("input dup2 failed");
			return 1;
		}
		close(fd1);
	}
	if(out) {
		if(outfile[0] == '~') {
			char *temp = malloc(1024);
			strcpy(temp, outfile);
			strcpy(outfile, tilda);
			strcat(outfile, temp+1);
			free(temp);
		} 

		int fd2;
		if(outtype) fd2 = open(outfile, O_WRONLY | O_APPEND | O_CREAT, 0644);
		else fd2 = open(outfile, O_WRONLY | O_TRUNC | O_CREAT, 0644);
		if(fd2 < 0) {
			perror("Failed to open file");
			return 1;
		} 
		close(1);
		if(dup2(fd2, 1) != 1) {
			perror("output dup2 failed");
			return 1;
		}
		close(fd2);
	}
	if(in || out) {
		*argC = newargC;
		argV[*argC] = NULL;
	}
	return 0;
}