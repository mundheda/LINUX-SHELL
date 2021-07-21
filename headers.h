#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <errno.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <signal.h>
#include <fcntl.h>
#include <termios.h>

void run(char *inputString, char *tilda);
void checkPiping(char *inputString, char *tilda, int pipes[][2], int pipenum); 

void execLS(int argC, char *argV[], char *tilda);
void sls(char *dir);
void slsa(char *dir);
void slsl(char *dir);
void slsal(char *dir);
void decideColor(int flag1,int flag2);
void resetColor();

int getargV(char inputString[], char **argV);
int getInput(char *inputString, char **commands);

char *getTilda();
void addTilda(char *currDir, char *tilda);
void disp_dir(char *tilda);	

int execBuiltin(int argC, char *argV[], char *tilda);
void execPWD(char *tilda);
void execECHO(char **argV, int argC);
void execCD(char **argV, int argC, char *tilda);

void execJobs(int argC, char *argV[]);
void exec_kjob(int argC, char *argV[]);
void exec_overkill(int argC, char *argV[]);
void exec_fg(int argC, char *argV[]);
void exec_bg(int argC, char *argV[]);
void execute(int argC, char *argV[], int bg);
void checkFinish();

void execPinfo(int argC, char *argV[], char *tilda);

void insertHistory(char *inputString, char *tilda);
void execHistory(int argC, char *argV[], char *tilda);

void execNight(int argC, char *argV[]);
void exec_dirty(int sec);
void exec_interrupt(int sec);

int checkRedirect(int *argC, char *argV[], char *tilda);

void execSetEnv(int argC, char *argV[]);
void execUnSetEnv(int argC, char *argV[]);
