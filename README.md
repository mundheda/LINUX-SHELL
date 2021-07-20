# LINUX-SHELL

A basic terminal based shell written in C with I/O redirection and piping support. 

## Running:
* Go to the working directory
* Run `make dash`
* Execute `./dash`
* Use `exit` to exit the shell

## Features:
* Display - When you execute the code a shell prompt of `<username@system_name:curr_dir>` appears.

* Tilda(~) is replaced by current working directory and works for all commands listed below.

* Following Commands have been implemented seperately: 
	* cd

	* pwd
 
	* echo

	* ls -[al] <Directory_Name>

	* pinfo Command

	* history

	* nightswatch

	* jobs

	* kjob

	* setenv/unsetenv

	* fg/bg

	* overkill

* Rest all commands are executed using execvp system call.

* Background Processes are also supported if ended with '&'.

* Exit status of finished background processes is displayed.

* Implemented piping and redirection

## Files:
* main.c
	* main file which runs the program.

* input.c
	* getInput function takes the input and divides the input into semi-colon seperated commands.
	* getArgV function trims the command and divide it into necessary arguments.

* display.c
	* get the current username, hostname and working directory and display it.

* builtin.c
	* commands echo, cd, pwd are implemented.

* ls.c
	* ls along with its options is implemented.

* pinfo.c
	* pinfo 

* execut.c
	* rest all commands are implemented.
	* checkfinish checks for all background processes that have stopped at start of loop.

* history.c
	* history

* nightswatch.c
	* nightswatch

* pipe_and_run.c
	* implemented piping and redirection
