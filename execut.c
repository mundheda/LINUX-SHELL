#include "headers.h"

struct processes {
	int jobNumber;
	int pid;
	char procName[1024];
	int curr_status;	//1 for working and 0 for stopped
	int bg;		//0 for foreground process and 1 for background process
} procs[100];		//currently only check upto 100 processes

int procsize = 0;
int fg_process;
int ctrlZ = 0;

void signalHandler2(int signum) 
{
	ctrlZ = 1;
	// printf("inside signal handler for process\n");
	kill(fg_process, SIGTSTP);
	return;
}

void checkFinish()
{
	int status;
	pid_t pid;
	while((pid = waitpid(-1, &status, WNOHANG)) > 0){
		for(int i = 1; i <= procsize; i++){
			if(procs[i].pid == pid) {
				procs[i].curr_status = 0;
				if(WIFEXITED(status)) {
					fprintf(stderr, "[%s with pid %d exited normally]\n", procs[i].procName, pid);
				}
				else  {
					fprintf(stderr, "[%s with pid %d exited abnormally]\n", procs[i].procName, pid);
				}
			}			
		}
		
	}
	int j = 1;
	for(int i = 1; i <= procsize; i++) {
		if(procs[i].curr_status) {
			procs[j++] = procs[i];
			procs[j-1].jobNumber = j-1;
			procs[j-1].curr_status = 1;
		}
	}
	procsize = j - 1;
}

void execJobs(int argC, char *argV[])
{
	for(int i = 1; i <= procsize; i++) {
		char *path = malloc(100);
		sprintf(path, "/proc/%d/status", procs[i].pid);

		char *procStatus = malloc(1024);
		FILE *procstat = fopen(path, "r");
		if(procstat) {
			char *buff = malloc(100);
			while(fgets(buff, 100, procstat)) {
				if(!strncmp("State", buff, 5)) {
					break;
				}
				
			}
			int check = 0, j = 0;
			for(int i = 0; i < strlen(buff); i++) {
				if(buff[i] == ')') {
					check = 0;
					break;
				}
				if(check) {
					procStatus[j++] = buff[i];
				}
				if(buff[i] == '(') {
					check = 1;
				}
			}
			procStatus[j] = '\0';

		}
		else {
			procs[i].curr_status = 0;
		}		
		if(procs[i].curr_status)
			printf("[%d] %s %s [%d]\n", procs[i].jobNumber, procStatus, procs[i].procName, procs[i].pid);
	}
}

void exec_kjob(int argC, char *argV[])
{
	if(argC != 3) {
		perror("invalid number of arguments");
		return;
	}
	int jobId = atoi(argV[1]);
	if(jobId > procsize || jobId < 1) {
		perror("invalid job number");
		return;
	}
	int killSignal = atoi(argV[2]);
	if(kill(procs[jobId].pid, killSignal) == -1) {
		perror("kjob");
		return;
	}
}

void exec_overkill(int argC, char *argV[])
{
	if(argC > 1) {
		perror("invalid number of arguments");
		return;
	}
	for(int i = 1; i <= procsize; i++) {
		if(procs[i].curr_status) {
			if(kill(procs[i].pid, SIGKILL) == -1) {
				perror("overkill");
				return;
			}
		}
	}
}

void exec_fg(int argC, char *argV[])
{
	int status;
	if(argC != 2) {
		perror("invalid number of arguments");
		return;
	}
	int jobId = atoi(argV[1]);
	if(jobId > procsize || jobId < 1) {
		perror("invalid job number");
		return;
	}
	if(getpgid(procs[jobId].pid) != getpid())
		tcsetpgrp(0,procs[jobId].pid);
	
	if(kill(procs[jobId].pid, SIGCONT) < 0){
		perror("no such process\n");
		return;
	}
	signal(SIGTTOU, SIG_IGN);
	waitpid(-1, &status, WUNTRACED);

	if(tcgetpgrp(STDIN_FILENO) == -1) {
		perror("tcgetpgrp for stdin\n");
	}
	else if(tcsetpgrp(STDIN_FILENO, getpid()) == -1) {
		perror("tcsetpgrp for stdin\n");
	}
	signal(SIGTTOU, SIG_DFL);
	return;
}

void exec_bg(int argC, char *argV[])
{
	if(argC != 2) {
		perror("invalid number of arguments");
		return;
	}
	int jobId = atoi(argV[1]);
	if(jobId > procsize || jobId < 1) {
		perror("invalid job number");
		return;
	}
	if(kill(procs[jobId].pid, SIGCONT) == -1) {
		perror("no such process");
	}

}

void execute(int argC, char *argV[], int bg)
{
	pid_t pid = fork();

	if(pid < 0) {
		perror("fork failed");
		return;
	}

	else if(pid == 0) {
		if(bg){
			setpgid(0,0);
		}
		execvp(argV[0], argV);
		perror("invalid command");
		_exit(1);
	}

	if(bg == 0) {
		int child_status = 0;
		fg_process = pid;
		signal(SIGTSTP, signalHandler2);
		waitpid(pid, &child_status, WUNTRACED);
		if(ctrlZ) {
			procsize++;
			procs[procsize].jobNumber = procsize;
			procs[procsize].pid = pid;
			strcpy(procs[procsize].procName, argV[0]);
			for(int i = 1; i < argC; i++) {
				strcat(procs[procsize].procName, " ");
				strcat(procs[procsize].procName, argV[i]);
			}
			procs[procsize].curr_status = 1;
			procs[procsize].bg = 1;
			ctrlZ = 0;
		}
	}
	else {
		procsize++;
		procs[procsize].jobNumber = procsize;
		procs[procsize].pid = pid;
		strcpy(procs[procsize].procName, argV[0]);
		for(int i = 1; i < argC; i++) {
			strcat(procs[procsize].procName, " ");
			strcat(procs[procsize].procName, argV[i]);
		}
		procs[procsize].curr_status = 1;
		procs[procsize].bg = 1;
		printf("[%d] %d\n", procsize, pid);
	}
	return;
}

