#include "headers.h"

void checkPiping(char *inputString, char *tilda, int pipes[][2], int pipenum) 
{
	int pipecount = 0;
	for(int i = 0; i < strlen(inputString); i++) {
		if(inputString[i] == '|') {
			pipecount++;
		}
	}

	if(pipecount == 0) {
		run(inputString, tilda);
		return;
	}

	pipe(pipes[pipenum]);
	int i;
	for(i = 0; inputString[i] != '|'; i++);
	inputString[i] = '\0';
	
	int spid = fork();
	if(spid == 0) {
		close(pipes[pipenum][0]);
		dup2(pipes[pipenum][1], 1); //duplicate write end to stdout
		close(pipes[pipenum][1]);
		run(inputString, tilda);
		exit(0);
	} 
	else if (spid < 0) {
		perror("fork failed\n");
	} 
	else {
		int dpid = fork();
		if(dpid == 0) {
			close(pipes[pipenum][1]); //close write end
			dup2(pipes[pipenum][0], 0); //duplicate read end to stdin
			close(pipes[pipenum][0]);
			checkPiping(inputString + i + 1, tilda, pipes, pipenum + 1);
			exit(0);
		} 
		else if (dpid < 0) {
			perror("fork failed");
		} 
		else {
			close(pipes[pipenum][0]);
			close(pipes[pipenum][1]);
			wait(NULL);
			wait(NULL);
			return;
		}
	}
}

void run(char *inputString, char *tilda)
{
	char **argV = (char **)malloc(10*sizeof(char *)); //Assuming max 10 argV
	int argC = getargV(inputString, argV);

	int fstdin, fstdout;
	if((fstdin = dup(0)) < 0) {
		perror("dup failed");
		return;
	}
	if((fstdout = dup(1)) < 0) {
		perror("dup failed");
		return;
	}

	if(checkRedirect(&argC, argV, tilda) != 0) {
		return;
	}
	
	int bg = 0;
	if(!strcmp(argV[argC-1], "&")) {
		argV[argC-1] = NULL;
		argC--;
		bg = 1;
	}

	if(!strcmp(argV[0],"exit") || !strcmp(argV[0],"quit")) {
		_exit(0);
	}

	if(execBuiltin(argC, argV, tilda) != 0) {
		if(!strcmp(argV[0], "ls")) {
			execLS(argC, argV, tilda);
		}
		else if(!strcmp(argV[0], "pinfo")) {
			execPinfo(argC, argV, tilda);
		}
		else if(!strcmp(argV[0], "history")) {
			execHistory(argC, argV, tilda);
		}
		else if(!strcmp(argV[0], "nightswatch")) {
			execNight(argC, argV);
		}
		else if(!strcmp(argV[0], "setenv")) {
			execSetEnv(argC, argV);
		}
		else if(!strcmp(argV[0], "unsetenv")) {
			execUnSetEnv(argC, argV);
		}
		else if(!strcmp(argV[0], "jobs")) {
			execJobs(argC, argV);
		}
		else if(!strcmp(argV[0], "kjob")) {
			exec_kjob(argC, argV);
		}
		else if(!strcmp(argV[0], "overkill")) {
			exec_overkill(argC, argV);
		}
		else if(!strcmp(argV[0], "fg")) {
			exec_fg(argC, argV);
		}
		else if(!strcmp(argV[0], "bg")) {
			exec_bg(argC, argV);
		}
		else {
			execute(argC, argV, bg);
		}
		
	}
	fflush(stdout);
	fflush(stdin);
	dup2(fstdin, 0);
	dup2(fstdout, 1);
	close(fstdin);
	close(fstdout);
}