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

int echo(char ** command){
	for(int i=1;i<sizeof(command) && command[i]!=NULL;i++){
		printf("%s ",command[i]);
	}
	printf("\n");
}