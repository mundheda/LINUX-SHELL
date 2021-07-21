#include "headers.h"

/*
**	execls executes the ls command
**	sls is used for simple ls
**  slsal is used for ls -al
**  slsa is used for ls -a
**  slsl is used for ls -l
*/

void execLS(int argC, char *argV[], char *tilda)
{
	char *dir = malloc(1024);
	strcpy(dir, ".");
	int flag_l = 0;
	int flag_a = 0;
	int valid = 0;
	for(int i = 1; i < argC; i++){
		if(argV[i][0] == '-'){
			for(int j = 1;j < strlen(argV[i]); j++){
				if(argV[i][j] == 'l'){
					flag_l = 1;
				}
				else if(argV[i][j] == 'a'){
					flag_a = 1;
				}
				else{
					printf("Only ls -[al] implemented\n");
				}
			}
		}
		else{
			valid++;
			if(valid > 1){
				printf("Too many argV / Presently only valid for one directory\n");
				return;
			}
			strcpy(dir, argV[i]);
		}
	}
	char *tildacpy = malloc(1024);
	strcpy(tildacpy, tilda);
	if(dir[0] == '~'){
		strcat(tildacpy, dir+1);
		strcpy(dir, tildacpy);
	}
	if(flag_a && flag_l){
		slsal(dir);
	}
	else if(flag_a){
		slsa(dir);
	}
	else if(flag_l){
		slsl(dir);
	}
	else{
		sls(dir);
	}

	free(dir);
	free(tildacpy);
}

void sls(char *dir)
{
	struct dirent **namelist;
	int n;
	n = scandir(dir, &namelist, NULL, alphasort);
	if(n == -1){
		perror("ls");
		return;
	}
	while(n--){
		if(namelist[n]->d_name[0] != '.')
			printf("%s\n", namelist[n]->d_name);
		free(namelist[n]);
	}
	free(namelist);
}

void slsa(char *dir)
{
	struct dirent **namelist;
	int n;
	n = scandir(dir, &namelist, NULL, alphasort);
	if(n == -1){
		perror("ls");
		return;
	}
	while(n--){
		printf("%s\n", namelist[n]->d_name);
		free(namelist[n]);
	}
	free(namelist);
}

void slsal(char *dir)
{
	struct dirent **namelist;
	struct stat st;
	if(stat(dir, &st) != 0){
		perror("ls");
		return;
	}
	printf("total %ld\n", st.st_size);
	int n;
	n = scandir(dir, &namelist, NULL, alphasort);
	if(n == -1){
		perror("ls");
		return;
	}
	int size_dir_string = strlen(dir);
	while(n--){
		dir[size_dir_string] = '/';
		dir[size_dir_string+1] = '\0';
		strcat(dir, namelist[n]->d_name);
		if(stat(dir, &st) != 0){
			perror("ls");
			return;
		}
		printf( (S_ISDIR(st.st_mode)) ? "d" : "-");
    	printf( (st.st_mode & S_IRUSR) ? "r" : "-");
    	printf( (st.st_mode & S_IWUSR) ? "w" : "-");
    	printf( (st.st_mode & S_IXUSR) ? "x" : "-");
    	printf( (st.st_mode & S_IRGRP) ? "r" : "-");
    	printf( (st.st_mode & S_IWGRP) ? "w" : "-");
    	printf( (st.st_mode & S_IXGRP) ? "x" : "-");
    	printf( (st.st_mode & S_IROTH) ? "r" : "-");
    	printf( (st.st_mode & S_IWOTH) ? "w" : "-");
    	printf( (st.st_mode & S_IXOTH) ? "x" : "-");
    	printf(" ");
    	printf("%2ld ", st.st_nlink);
    	struct passwd *pass = getpwuid(st.st_uid);
    	printf("%8s ", pass->pw_name);
    	struct group *grp = getgrgid(st.st_gid);
    	printf("%8s ", grp->gr_name);
    	printf("%8ld ", st.st_size);
    	char *date = malloc(50);
    	strftime(date, 50, "%b %d %R", localtime(&(st.st_mtime)));
    	printf("%s ", date);
    	decideColor(S_ISDIR(st.st_mode), st.st_mode & S_IXUSR);
    	printf("%s\n", namelist[n]->d_name);
    	resetColor();
    	
    	free(date);
		free(namelist[n]);
	}
	free(namelist);
}

void slsl(char *dir)
{
	struct dirent **namelist;
	struct stat st;
	if(stat(dir, &st) != 0){
		perror("ls");
		return;
	}
	printf("total %ld\n", st.st_size);
	int n;
	n = scandir(dir, &namelist, NULL, alphasort);
	if(n == -1){
		perror("ls");
		return;
	}
	int size_dir_string = strlen(dir);
	while(n--){
		if(namelist[n]->d_name[0] == '.'){
			free(namelist[n]);
			continue;
		}
		dir[size_dir_string] = '/';
		dir[size_dir_string+1] = '\0';
		strcat(dir, namelist[n]->d_name);
		if(stat(dir, &st) != 0){
			printf("%s\n", dir);
			perror("ls");
			return;
		}
		printf( (S_ISDIR(st.st_mode)) ? "d" : "-");
    	printf( (st.st_mode & S_IRUSR) ? "r" : "-");
    	printf( (st.st_mode & S_IWUSR) ? "w" : "-");
    	printf( (st.st_mode & S_IXUSR) ? "x" : "-");
    	printf( (st.st_mode & S_IRGRP) ? "r" : "-");
    	printf( (st.st_mode & S_IWGRP) ? "w" : "-");
    	printf( (st.st_mode & S_IXGRP) ? "x" : "-");
    	printf( (st.st_mode & S_IROTH) ? "r" : "-");
    	printf( (st.st_mode & S_IWOTH) ? "w" : "-");
    	printf( (st.st_mode & S_IXOTH) ? "x" : "-");
    	printf(" ");
    	printf("%2ld ", st.st_nlink);
    	struct passwd *pass = getpwuid(st.st_uid);
    	printf("%8s ", pass->pw_name);
    	struct group *grp = getgrgid(st.st_gid);
    	printf("%8s ", grp->gr_name);
    	printf("%8ld ", st.st_size);
    	char *date = malloc(50);
    	strftime(date, 50, "%b %d %R", localtime(&(st.st_mtime)));
    	printf("%s ", date);
    	decideColor(S_ISDIR(st.st_mode), st.st_mode & S_IXUSR);
    	printf("%s\n", namelist[n]->d_name);
    	resetColor();
		
		free(date);
		free(namelist[n]);
	}
	free(namelist);
}

void decideColor(int flag1, int flag2)
{
	if(flag1){
		printf("\033[1;34m");
	}
	else if(flag2){
		printf("\033[1;32m");
	}
}

void resetColor()
{
	printf("\033[0m");
}