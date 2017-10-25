#include <stdio.h>
#include <stdlib.h> //exit()
#include <string.h>
#include <unistd.h> //exec family
#include <sys/types.h> //wait
#include <sys/wait.h>  //wait
#define MaxNUM 1000
#include <errno.h>
#include <signal.h>
#include <iostream>


using namespace std;

void signal_handler(){
    while(waitpid(-1,NULL,WNOHANG)>0);
    /* send fg job/ related process group signal
	    if (sig != 0) {
		kill(-sig, SIGINT);
		/*if (sig < 0){
			printf("Job [%d] (%d) terminated by signal %d\n", jid, pid, (-sig));
			deletejob(jobs, pid);
		}*/
}

int main(int argc, const char* argv[]){

	signal(SIGCHLD, SIG_IGN);		//for linux
	char *str_t;
	str_t = (char *)malloc(MaxNUM*sizeof(char));
	char **arg_t;


	char c;			//getchar
	int i;			//index of str_t
	char *p;		//for strtok
	int arg_i;		//index of arg_t
    int count;
    int err;
    int nowait;     //for &



    printf("\x1B[32mWelcome to mysh by 0316083!\n");
	while(1){
		/* prepare */
		i=0;
        nowait=0;
		strcpy(str_t,"");

        printf("\x1B[33myicchen in %s\n", getcwd(NULL, NULL));
        printf("\x1B[35mmysh>");
		/* getline */
		c=getchar();
        count=1;
		while(c!='\n'){
			str_t[i]=c;
            if(str_t[i]==' ') count++;
            //if(str_t[i]=='&') count--;
			i++;
			c=getchar();
		}
        if(i==0) continue;
		str_t[i]='\0';
		//printf("%s\n",str_t);

        /* Counting Arguement & AllocMem Start */
        count++;
        arg_t = (char**)malloc(count*sizeof(char*));
        int k=0;
		for(k=0;k<count;k++){
            //printf("Size %d: %lu\n",k,sizeof(arg_t[k]));
			arg_t[k] = (char*)malloc(MaxNUM*sizeof(char));

        }
        /* Counting Arguement & AllocMem Finish */

		/* Passing Arguement Start */
		arg_i=0;
        p=strtok(str_t," ");
		strcat(p,"\0");
        arg_t[arg_i++]=p;
		p=strtok(NULL," ");
		while(p!=NULL){
            if(strcmp(p,"&")!=0){
                //printf("Old Arguement %d: %s\n",arg_i,arg_t[arg_i]);
                strcat(p,"\0");
                arg_t[arg_i]=p;
                //printf("Arguement %d: %s\n",arg_i,arg_t[arg_i]);
                arg_i++;
                p=strtok(NULL," ");
            }
            else{
                nowait=1;
                p=strtok(NULL," ");
            }
 		}
        arg_t[arg_i]=NULL; //the last one of the argv in execvp must be NULL
		/* Passing Arguement Finish */

        if(strcmp(arg_t[0],"exit")==0 && arg_t[1]==NULL){ //Build-in Exit
            free(str_t);
			return 0;
		}
		if(strcmp(arg_t[0],"cd")==0 && arg_t[2]==NULL){
            chdir(arg_t[1]);
		}
		//else if(arg_t[0],"cd")==0 && arg_t[1]==NULL)
		if(strcmp(arg_t[0],"fg")==0 && arg_t[2]==NULL){

		}
        if(strcmp(arg_t[0],"bg")==0 && arg_t[2]==NULL){

        }
        if(strcmp(arg_t[0],"kill")==0 && arg_t[2]==NULL){
            kill(323,SIGCONT);
        }



        if(arg_t[0]==NULL) continue;

		/* Fork */
		pid_t pid;
		pid=fork();
		if(pid<0){ //error occurred
            perror("fork"); //print error in standard lib
			//fprintf(stderr,"Fork failed!\n");
			exit(-1);
		}
		else if(pid==0){ //child process
            err=execvp(arg_t[0],arg_t);

            if(err < 0) {
//11           ///perror(arg_t[0]); //print error in standard lib
                //fprintf(stderr,"ERROR : (%s) %s\n",arg_t[0],strerror(errno));
                exit(-1);
            }
		}
		else{ //parent
			if(nowait==0){ //wait(NULL);
				//printf("Waiting for %d!\n",pid);
				waitpid(pid,NULL,0);
			}
            else{
                //printf("No wait!\n");
            }
			//printf("Child Complete!\n");
		}
		free(arg_t);
        arg_t=NULL;
	}
	free(str_t);
	return 0;
}

