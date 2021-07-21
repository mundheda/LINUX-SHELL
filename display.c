#include "headers.h"

char *getTilda()
{
	char *tilda = (char *) malloc(1024);
	
	int pid = getpid();
	char path[1024];
	sprintf(path, "/proc/%d/exe", pid);
	readlink(path, tilda, 1024);
	for(int i=strlen(tilda);;i--){
		if(tilda[i] != '/'){
			tilda[i] = '\0';
		}
		else{
			tilda[i] = '\0';
			break;
		}
	}
	return tilda;
}

void addTilda(char *currDir, char *tilda)
{
	if(strlen(currDir) < strlen(tilda)){
		return;
	}
	int i = 0;
	for(i=0; i<strlen(tilda); i++){
		if(currDir[i] != tilda[i]){
			return;
		}
	}
	currDir[0] = '~';
	for(int j = i; j < strlen(currDir); j++){
		currDir[j-i+1] = currDir[j];
	}
	currDir[strlen(currDir) - i + 1] = '\0';
}

void disp_dir(char *tilda)
{
	struct passwd *pass = getpwuid(geteuid());
	char *Host = malloc(50);
	gethostname(Host,50);
	
	char *currDir = malloc(1024);
	getcwd(currDir,1024);
	addTilda(currDir, tilda);
	
	printf("\033[1;31m");
	printf("<");
	printf("\033[1;32m");
	printf("%s@%s:", pass->pw_name, Host);
	printf("\033[1;34m");
	printf("%s",currDir);
	printf("\033[1;31m");
	printf("> ");
	printf("\033[0m");
	
	// free(pass);
	free(Host);
	free(currDir);
}

