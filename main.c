#include "headers.h"

#define BUF_SIZE 1024

void siginthandler(int signum)
{
	return;
}

void sigtstphandler(int signum)
{
	return;
}

int main()
{

	char *tilda, **commands, *inputString;
	inputString = (char *) malloc(BUF_SIZE);
	commands = (char **) malloc(10*sizeof(char *));

	tilda = getTilda();
	
	while (1) {
		disp_dir(tilda);
		
		signal(SIGINT, siginthandler);
		signal(SIGTSTP, sigtstphandler);

		int size = getInput(inputString, commands);
		insertHistory(inputString, tilda);
		for (int i=0; i < size; i++) {
			int pipes[100][2];
			checkPiping(commands[i], tilda, pipes, 0);
		}

		checkFinish();
	}
	return 0;
}
