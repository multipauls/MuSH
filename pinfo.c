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
#include <errno.h>
int pinfo(char **tokens, char *procpath){
	pid_t procid;
	char ppath[1024], c;
	if (tokens[1]==NULL){
		procid=getpid();
		strcpy(ppath,procpath);
	}
	else{

		long long int p = strtoll(tokens[1], NULL, 0);
		procid= (pid_t) p;
		char path[1024];
		char buf[1024];
		int bufsize=1024;
		struct stat sts;
		sprintf(path, "/proc/%d/exe", procid);
		if (stat(path, &sts) == -1 && errno == ENOENT) {
  				printf("Error: Cannot find process \n" );
  				return 0;
		}
		readlink(path, buf, bufsize);
		sprintf(ppath,"%s",buf );

	}
	char strid[50];
	char pathstr[1024];
	sprintf(pathstr, "/proc/%d/status", procid);
	FILE* status = fopen( pathstr, "r" );
	int ctr=0;
	char line[256];
	while (fgets(line, 256, status) != NULL){
		if (ctr==2){
			printf("Process Status-- %c\n", line[7]);
		}
		ctr++;
	}

	fclose(status);
	sprintf(pathstr, "/proc/%d/statm", procid);
	FILE* statm = fopen( pathstr, "r" );
	char *linem;
	char line2[256];
	fgets(line2, 256, statm);
	linem = strtok(line2, " \t");
	printf("memory-- %s\n", linem);

	fclose(statm);
	sprintf(strid,"%d",procid );
	printf("pid-- %s\n",strid );
	printf("Executable Path-- %s\n",ppath );

}