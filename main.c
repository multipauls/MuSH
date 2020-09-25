
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


struct bgprog {
	char *prog;
	int bgpid;
};


char home[1024];
char cwd[1024];
struct bgprog bglist[1024];
int bgctr=0;
pid_t progid, curpid;
int shell=STDERR_FILENO;


/*

void printkill(int sig){
	pid_t myid;
	myid=getpid();
	printf("%d %d\n", myid, progid);
	//assert(sig == SIGQUIT);
	if (progid != myid) {
	printf("Exiting %d\n",myid );
	
	}
}
*/
void stphandler(int sig){
	pid_t myid;
	myid=getpid();
	printf(" %d %d\n",myid, progid );
	//printf("Comes here\n");
	//signal(SIGTSTP, SIG_DFL);
	if (myid != progid)
		printf("%d\n",kill(myid, SIGSTOP));
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
  	signal(SIGINT, SIG_DFL);
  	signal(SIGTSTP, stphandler);
  	

    if(execvp(tokens[0],arglist) == -1)
      {
        printf("Execution failed '%s'\n",tokens[0]);
        return;
      }
   	printf("%d", getpgrp());
  }   
  else {
  	
  		if (bg == 0)
        {	//printf("%d\n",pid );
          //setpgid(pid, 0);
    	//if(setpgid(pid, pid) == 0) perror("setpid");
          wpid = waitpid(pid, &status, WUNTRACED);
          //printf("%d parent\n",wpid );
          while (!WIFEXITED(status) && !WIFSIGNALED(status) && !WIFSTOPPED(status))
            {
              wpid = waitpid(pid, &status, WUNTRACED);
            }
          }
        else{
        printf("pid %d\n", pid);
        //snprintf(bglist[bgctr].prog,1023,"%s", tokens[0]);
    	//printf("here1\n" );
    	//bglist[bgctr].prog=tokens[0];
    	//printf("%d %d", bglist[bgctr].bgpid, bgctr);
    	//printf("here2" );
    	printf(" %d\n",bgctr);
    	//printf("here3\n" );
    	bgctr++;
        }

  }
}

void printexit(){
	int pid;
	int status;
	pid=waitpid(-1, &status, WNOHANG|WUNTRACED);
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
	//signal(SIGQUIT, printkill);
	signal(SIGINT, SIG_IGN);
    signal(SIGTSTP, SIG_IGN);
    
    //signal(SIGTSTP, stphandler);
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
		
		char *lineptr;
		lineptr=fgets(incommand, sizeof(incommand), stdin);
		if (lineptr == NULL)
    		break;

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
			
			killpg(getpgrp(), SIGINT);
		}
		else if (strcmp(tokens[0], "quit")==0){
			break;
		}
		else{

			execute(tokens,i);

		}
		printf("%d", getpgrp());
		
	}
return 0;
}
