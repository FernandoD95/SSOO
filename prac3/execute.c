

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include "parser.h"
#include "jobs.h"
#include "internals.h"
#include "minishell_input.h"

//void jobs();

/*
Tenemos que hacer que si detecta el "&" se ejecute en segundo plano, para saber que
esta el "&" tenemos que mirar en la estructura command.background, si hay un 1 en background
y si hay un 0 en primer plano.

Todo esto hay que hacerlo en un execute.c y creo que hay que colocarlo en el make

EXIT_SUCCESS y EXIT_FAILURE son dos constantes globales 0 y 1 respectivamente
printf("%d",EXIT_SUCCESS);
printf("%d",EXIT_FAILURE);
*/


/*Tambien tenemos que meter esto en un .c que se llame execute.c  ademas solo tenemos que hacerlo para
ordenes simples, es decir, sin > ni >> ni <
*/

int execute_piped_command_line(command* p_cmd, int n){
    pid_t ident;
    int status,i;
		
		//http://voyager.deanza.edu/~perry/sigchld.html           http://stackoverflow.com/questions/2377811/tracking-the-death-of-a-child-process

 
    //printf("%s\n",p_cmd->argv[0]);
		
    for (i = 0; i < n; i++) {
			printf("%d - %d",i,n);
   		if(0==strcmp(p_cmd[i].argv[0],"exit")){
				jobs_new(ident,p_cmd[i].argv[0]);
        exit(EXIT_SUCCESS);
				jobs_finished(getpid());
			}
			else if(0==strcmp(p_cmd[i].argv[0],"jobs")){
					jobs_new(ident,p_cmd[i].argv[0]);
      	  execute_internal_command(&p_cmd[i]);
					jobs_finished(getpid());
				
			}
			else{
			
				ident = fork(); //He movido el fork para que se cree solo si no se meten ordenes preestablecidas como exit o jobs
    		switch(ident){
        /*case -1:
            exit(EXIT_FAILURE);Esto lo podremos borrar por que no hace falta, es mas el exit sale de la terminal
				*/
        	case 0:
        		if (execvp(p_cmd[i].argv[0], p_cmd[i].argv) == -1){
            	perror("Error ");      //perror("HIJO: Error al ejecutar execvp \n");
							//printf("No reconocido %s\n",p_cmd->argv[0]);    Esto me imprimiria lo que da el error
							exit(EXIT_FAILURE);
						}
						break;
					
					default:
						jobs_new(ident,p_cmd[i].argv[0]);
						if(p_cmd[i].background==0)
							{
            		if(wait(&status)== -1){  //esto es pid == -1		Si status fuese !=0 querria decir que el proceso termino anormalmente
									perror("Se ha producido un error");
							}
							jobs_finished(ident);
							}
						
					}
			}
		}
		return 0;
}

/*void jobs(){
	int i;
	for(i=0;i<jobs_count;i++){
		printf("PID del proceso: %d - Nombre del programa: %s - ",jobs_array[i].pid, jobs_array[i].name);
		if(jobs_array[i].finished==1)
			printf("El proceso fue finalizado.\n");
		else
			printf("El proceso aun no fue finalizado.\n");
	}
}*/

/*
Para el for y los pipes -> pipe(fdpipe)
esto es para el primer proceso dup2(fdpipe,cmd[i].input)  (no hace falta cerrar el std_out el dup2 lo hace, con el dup no lo hariamos)
if(ident==0)
close(fdpipe[i-1])   Mirarse el close
dup2(fdpipe,cmd[i].input)
execvp(cmd[i],(el std_out del pipe anterior))

*/