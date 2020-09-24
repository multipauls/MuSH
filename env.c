#include <unistd.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h> 
#include <stdlib.h>
#include <sys/stat.h>
#include <dirent.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>



void setenvfunc(char **tokens, int i){
	if (i==2){
		setenv(tokens[1], "",0);
	}
	else if (i==3){
		setenv(tokens[1], tokens[2],0);
	}
	else{
		printf("Incorrect number of arguments\n");
	}

}

void unsetenvfunc(char **tokens, int i){
	if (i==2){
		unsetenv(tokens[1]);
	}
	else{
		printf("Incorrect number of arguments\n" );
	}
	/*if (getenv(tokens[1])){
		printf("works\n");
	}
	else{printf("no workey\n");}*/
}