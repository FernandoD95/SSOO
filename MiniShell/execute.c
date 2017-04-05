#ifndef __EXECUTE_H__
#define __EXECUTE_H__
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
#endif __EXECUTE_H__
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
    int status;
    //printf("%s\n",p_cmd->argv[0]);
    ident = fork();
    if(0==strcmp(p_cmd->argv[0],"exit")){
        exit(EXIT_SUCCESS);
		}
		if(0==strcmp(p_cmd->argv[0],"jobs")){
       
        //printf("%d",jobs_count);
			}
	
    switch(ident){
        case -1:
            exit(EXIT_FAILURE);
        
        case 0:
        if (execvp(p_cmd->argv[0], p_cmd->argv) < 0){
            //if (execl("/bin/ls", "ls", "-a", NULL) < 0)
            perror("Error ");      //perror("HIJO: Error al ejecutar execvp \n");
						//printf("No reconocido %s\n",p_cmd->argv[0]);    Esto me imprimiria lo que da el error 
						exit(EXIT_FAILURE);
				}
				
        
        default:
						jobs_new(ident,p_cmd->argv[0]);
						if(p_cmd->background==0){
            	if (wait(&status)== -1)  //esto es pid == -1
																				//Si status fuese !=0 querria decir que el proceso termino anormalmente 
								{
									perror("PADRE: Se ha producido un error \n");
									
								}
								//else if (pid == ident)                 
			    				//printf("PADRE: El hijo ha finalizado \n"); 
							}
						
    }
	return 0;
}

