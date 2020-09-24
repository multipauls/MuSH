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
#include <signal.h>
#include<sys/wait.h> 

int pinfo(char **tokens, char *procpath);
int cd(char ** command, char home[1024], char cwd[1024]);
int echo(char ** command);
int lsdo( char* tokens,int a, int l);
int ls (char* tokens[1024], int i, char*home);