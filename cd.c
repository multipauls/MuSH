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

int cd(char ** command, char home[1024], char cwd[1024])
{
	const char s[2]=" "; 
	char homecp[1024];

	strcpy(homecp,cwd);
	if ( command[1] == NULL)
		strcpy(homecp, home);
	else if ( *command[1] == '~')
		strcpy(homecp, home);

	else if (strcmp(command[1], homecp) != 0){
		strcat(homecp,"/");
		strcat(homecp, command[1]);
	}
	if (chdir(homecp) != 0)  
		perror("chdir() failed");
	return 1;

}