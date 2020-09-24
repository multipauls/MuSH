
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
#include <assert.h> 
#include "shell.h"
//#include "ls.h"
//#include "echo.h"
//#include "cd.h"

struct bgprog {
	char *prog;
	int bgpid;
};


char home[1024];
char cwd[1024];
struct bgprog bglist[1000];
int bgctr=0;
pid_t progid, curpid;

void siginthandler(int sig){

}


void printkill(int sig){
	pid_t myid;
	myid=getpid();
	printf("%d %d\n", myid, progid);
	//assert(sig == SIGQUIT);
	if (progid != myid) {
	printf("Exiting %d\n",myid );
	
	}
}

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

void execute(char **tokens, int i){
	int bg=0,wpid=0,pid=0, endlist, status;
	if (strcmp(tokens[i-1],"&")==0){
		bg=1;
	}
	char **arglist;
	if (bg==0){
		endlist=i;
	}
	else{
		endlist=i-1;
	}
	int j=0;
	for ( j=0;j<endlist;j++){
		arglist[i]=tokens[i];
	}
	arglist[j]=NULL;


  	pid = fork();
  if (pid < 0)
    {
      printf("Forking failed\n");
    }


  else if(pid == 0){
  	//printf("I am child" );
  	setpgid(0,0);
  	int selfpid=getpid();
  	printf("%d", selfpid);
    if (bg==1){
    	//snprintf(bglist[bgctr].prog,1023,"%s", tokens[0]);
    	//printf("here1\n" );
    	//bglist[bgctr].bgpid=selfpid;
    	//printf("%s %d %d", bglist[bgctr].prog,bglist[bgctr].bgpid, bgctr);
    	//printf("%d %d", bglist[bgctr].bgpid, bgctr);
    	//printf("here2" );
    	//printf(" %d\n",bgctr);
    	//printf("here3\n" );
    	//bgctr++;
    }
    if(execvp(tokens[0],arglist) == -1)
      {
        printf("Execution failed '%s'\n",tokens[0]);
        return;
      }
   
  }   
  else {
  		if (bg == 0)
        {	//printf("%d\n",pid );
          setpgid(pid, 0);
          wpid = waitpid(pid, &status, WUNTRACED);
          //printf("%d parent\n",wpid );
          while (!WIFEXITED(status) && !WIFSIGNALED(status))
            {
              wpid = waitpid(pid, &status, WUNTRACED);
            }
          }
        else{
        	//printf("here1\n" );
        }
  }
}

void printexit(){
	int pid;
	int status;
	pid=waitpid(-1, &status, WNOHANG);
	if (pid<=0){
		return;
	}
	else if (WIFEXITED(status)==1 && WEXITSTATUS(status)==EXIT_SUCCESS){
		printf("\nProcess with pid %d exited normally \n", pid);
	}
	else{
		printf("\nProcess with pid %d exited abnormally \n", pid);	
	}
	
}

/*int pinfo(char **tokens, char *procpath){
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
		sprintf(path, "/proc/%d/exe", procid);
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


int lsdo( char* tokens,int a, int l)
{
	struct dirent **namelist;
	int n;
	char dirname[1024];
	char fname[1024];
	struct stat mystat;
	strcpy(dirname,".");
	if (tokens!=NULL)
		sprintf(dirname, "%s", tokens);
	n = scandir(dirname, &namelist, NULL, alphasort);

	char stor[1024];    
	if (n < 0 && stat(dirname,&mystat))
		perror("scandir");
	else if (n < 0 && !stat(dirname,&mystat))
	{    	if (tokens[1] != '.' || a==1){
						     if (l==1){	
							     printf( (S_ISDIR(mystat.st_mode)) ? "d" : "-");
							     printf( (mystat.st_mode & S_IRUSR) ? "r" : "-");
							     printf( (mystat.st_mode & S_IWUSR) ? "w" : "-");
							     printf( (mystat.st_mode & S_IXUSR) ? "x" : "-");
							     printf( (mystat.st_mode & S_IRGRP) ? "r" : "-");
							     printf( (mystat.st_mode & S_IWGRP) ? "w" : "-");
							     printf( (mystat.st_mode & S_IXGRP) ? "x" : "-");
							     printf( (mystat.st_mode & S_IROTH) ? "r" : "-");
							     printf( (mystat.st_mode & S_IWOTH) ? "w" : "-");
							     printf( (mystat.st_mode & S_IXOTH) ? "x" : "-");
							     printf(" ");
							     printf("%ld ", mystat.st_nlink);
							     struct passwd *pw = getpwuid(mystat.st_uid);
							     struct group  *gr = getgrgid(mystat.st_gid);
							     printf("%s ",gr->gr_name );
							     printf("%s ",pw->pw_name );    
							     printf("%ld ",mystat.st_size);}
						     printf(" %s\n", tokens);
					     }
	}
	else {

		while (n--) {
			strcpy(fname,namelist[n]->d_name);
			if (tokens!=NULL)
				sprintf(fname, "%s/%s", tokens, namelist[n]->d_name);
			if (namelist[n]->d_name[0] != '.' || a==1){
				if (l==1){
					stat(fname, &mystat);  	
					printf( (S_ISDIR(mystat.st_mode)) ? "d" : "-");
					printf( (mystat.st_mode & S_IRUSR) ? "r" : "-");
					printf( (mystat.st_mode & S_IWUSR) ? "w" : "-");
					printf( (mystat.st_mode & S_IXUSR) ? "x" : "-");
					printf( (mystat.st_mode & S_IRGRP) ? "r" : "-");
					printf( (mystat.st_mode & S_IWGRP) ? "w" : "-");
					printf( (mystat.st_mode & S_IXGRP) ? "x" : "-");
					printf( (mystat.st_mode & S_IROTH) ? "r" : "-");
					printf( (mystat.st_mode & S_IWOTH) ? "w" : "-");
					printf( (mystat.st_mode & S_IXOTH) ? "x" : "-");
					printf(" ");
					printf("%ld ", mystat.st_nlink);
					struct passwd *pw = getpwuid(mystat.st_uid);
					struct group  *gr = getgrgid(mystat.st_gid);
					printf("%s ",pw->pw_name );
					printf("%s ",gr->gr_name );     
					printf("%ld ",mystat.st_size);}
				printf(" %s\n", namelist[n]->d_name);
				free(namelist[n]);
			}

		}
	}

}

int ls (char* tokens[30], int i){
	int  l=0, a=0, ctr=0;
	char count[30][1024];
	//int j;
	for (int j=1; j<i; j++){
		if (tokens[j][0]=='-'){
			//printf("if here" );
			if(tokens[j][1]=='a'||tokens[j][2]=='a')
			{
				if(tokens[j][1]=='l'||tokens[j][2]=='l'){
					l=1;
					a=1;
				}
				else
					a=1;
			}

			else if(tokens[j][1]=='l'||tokens[j][2]=='l')
			{
				if(tokens[j][1]=='a'||tokens[j][2]=='a'){
					l=1;
					a=1;
				}
				else
					l=1;
			}

			else{
				strcpy(count[ctr], tokens[j]);
				ctr++;
			}			
		}

		else{

			//printf("else here %s", count[ctr]);
			snprintf(count[ctr],1023,"%s", tokens[j]);
			ctr++;
			//printf("%d %d %d\n",ctr, i, j);
		}

		//printf("%s\n",tokens[j+1] );
		//printf("%d %d\n",j, i );
	}

	if (ctr==0){
		for (int j=1; j<=i ; j++){
			if (tokens[j]==NULL){
				
				break;
			}
			if (tokens[j][0]=='-'){
				//printf("if here" );
				if(tokens[j][1]=='a'||tokens[j][2]=='a')
				{
					if(tokens[j][1]=='l'||tokens[j][2]=='l'){
						l=1;
						a=1;
					}
					else
						a=1;
				}
				else if(tokens[j][1]=='l'||tokens[j][2]=='l')
				{
					if(tokens[j][1]=='a'||tokens[j][2]=='a'){
						l=1;
						a=1;
					}
					else
						l=1;
				}
			}

		}

		lsdo(".", a, l);
	}

	else{

		for (int j=0; j<ctr; j++){
			if (count[j]==NULL && j!=1){

				break;
			}
			lsdo(count[j],a, l);

		}
	}

}

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

int echo(char ** command){
	for(int i=1;i<sizeof(command) && command[i]!=NULL;i++){
		printf("%s ",command[i]);
	}
	printf("\n");
}*/


int main(int argc,char* argv[]){
	char hostname[HOST_NAME_MAX];
	char username[LOGIN_NAME_MAX];
	char incommand[1024];
	char command[1024];
	char *tokens[1024];
	int pipelist[1024];
	int index;
	gethostname(hostname, HOST_NAME_MAX);
	getlogin_r(username, LOGIN_NAME_MAX);
	getcwd(home, sizeof(home));
	signal(SIGCHLD, printexit);
	signal(SIGQUIT, printkill);
	progid= getpid();


	while (1){
		getcwd(cwd, sizeof(cwd));
		//printf("%s %ld %ld\n", cwd, strlen(cwd), strlen(home));

		printf("<%s@%s:",username, hostname);
		if ( strlen(home) > strlen(cwd) )
			printf("%s> ",cwd);
		else if (strlen(home) == strlen(cwd))
			printf("~> ");
		else if (strlen(home) < strlen(cwd))
		{
			printf("~");
			for(int i=strlen(home);;i++)
			{
				if(cwd[i] != '\0')
					printf("%c", cwd[i]);
				else if (cwd[i] == '\0')
					break;
			}
			printf("> " );
		}
		fgets(incommand, sizeof(incommand), stdin);
		strcpy(command,incommand);
		tokens[0] = strtok(incommand, " \n\t");
		int i =0, j=0;

		while (tokens[i]!=NULL){
			i++;
			tokens[i] = strtok(NULL, " \n\t");
				

		}
		int k=0;
		while (k<i){
			if (strcmp(tokens[k],"|")==0){
				//printf("here\n");
				pipelist[j]=k;
				
				printf("%d %d \n",k, j );
				j++;
				}

			k++;
		}
		
		if (i==0){
		}
		else if (strcmp(tokens[0],"cd")==0){
			cd(tokens, home, cwd);

		}
		else if (strcmp(tokens[0],"echo")==0){
			echo(tokens);
		}
		else if (strcmp(tokens[0],"pwd")==0){
			char val[1024];
			getcwd(val, sizeof(val));
			printf("%s\n", val );
		}
		else if (strcmp(tokens[0],"ls")==0){
			//if (i==1)
			//	i++;
			//sprintf(tokens[1],".");
			ls(tokens,i, home);
			//lsdo(tokens[1], 0, 0);
			//printf("%d",i );
		}
		else if (strcmp(tokens[0],"pinfo")==0){

			pinfo(tokens, argv[0]);
		}

		else if (strcmp(tokens[0], "setenv")==0){
			setenvfunc(tokens, i);
		}

		else if (strcmp(tokens[0], "unsetenv")==0){
			unsetenvfunc(tokens, i);
		}

		else if (strcmp(tokens[0], "overkill")==0){
			signal(SIGQUIT, SIG_IGN);
			kill(-progid, SIGQUIT);
		}
		else{

			execute(tokens,i);

		}

		
	}

return 0;
}
