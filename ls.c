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

int ls (char* tokens[1024], int i, char *home){
	int  l=0, a=0, ctr=0;
	char count[1024][1024];
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

				printf("%s\n",tokens[j] );
				strcpy(count[ctr], tokens[j]);
				ctr++;
			}			
		}
		
		else{
			if (tokens[j][0]=='~'){
					snprintf(tokens[j],1023,"%s",home );
				}
			printf("%s\n",tokens[j] );
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
				//printf("break here" );
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