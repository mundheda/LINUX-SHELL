#include "headers.h"

void insertHistory(char *inputString, char *tilda)
{
	char path[1024];
	strcpy(path, tilda);
	strcat(path,"/history.txt");
	FILE *f = fopen(path, "r");

	char hdata[20][1024];
	int size = 0;

	if(f) {
		int i = 0;
		while(fgets(hdata[i], sizeof(hdata[i]), f)) {
			hdata[i][strlen(hdata[i])-1] = '\0';
			i++;
		}
		if(i == 20) {
			for(int i = 0;i < 19;i++) {
				strcpy(hdata[i], hdata[i+1]);
			}
			strcpy(hdata[19], inputString);
			size = 20;
		}
		else {
			strcpy(hdata[i], inputString);
			size = i+1;
		}
	}
	else {
		size = 1;
		strcpy(hdata[0], inputString);
	}
	
	f = fopen(path, "w");
	for(int i=0;i<size;i++){
		fprintf(f, "%s\n", hdata[i]);
	}
	fclose(f);
}

void execHistory(int argC, char *argV[], char *tilda)
{
	char path[1024];
	strcpy(path, tilda);
	strcat(path,"/history.txt");
	if(argC > 2){
		fprintf(stderr, "Invalid number of arguments\n");
	}
	FILE *f = fopen(path, "r");
	if(!f){
		fprintf(stderr, "nothing in history :(\n");
		return;
	}
	char hdata[20][1024];
	// fread(hdata, sizeof(char), sizeof(hdata), f);
	int end = 0;
	while(fgets(hdata[end], sizeof(hdata[end]), f)) {
		hdata[end][strlen(hdata[end])-1] = '\0';
		end++;
	}
	int start, arg = 10;
	if(argC == 2) {
		arg = atoi(argV[1]);
	}
	start = end - arg;
	if(start < 0) start = 0;
	for(int i = start; i < end; i++) {
		printf("%s\n", hdata[i]);
	}
	fclose(f);
}