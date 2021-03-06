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
#include "jobs.h"
#include "execute.h"
#include "parser.h"
#include "minishell_input.h"

void show_command(command * p_cmd);
void Ejecutar_Comando(command * p_cmd);

//Creo que con sigaction() podemos evitar que al apretar intro se fastidie la ejecucion del programa	

int main (int argc, char *argv[])
{
    command* cmds;
    char* raw_command;
    int n_cmds=0;
    int n, i;

    while (1)
    {
        // We print the prompt 
        print_prompt(); 
 
        raw_command = NULL;
        n = read_line (&raw_command, stdin);
        if (n==-1) 
        {
            break;
        }

        n_cmds = parse_commands(&cmds, raw_command); 

        for (i = 0; i < n_cmds; i++)
        { 
            //show_command(&cmds[i]);
					  execute_piped_command_line(&cmds[i], n);
            //Ejecutar_Comando(&cmds[i]);
        }

        free_commands(cmds, n_cmds); 
        free(raw_command);
    }

    exit(0);
}

void show_command(command * p_cmd)
{
    int i;

    printf ("\tRaw command: \"%s\"\n", p_cmd->raw_command);
    printf ("\tNumber of arguments: %d\n", p_cmd->argc);

    for (i=0; i<=p_cmd->argc; i++)
        if (p_cmd->argv[i] != NULL)
            printf ("\t\targv[%d]: \"%s\"\n", i, p_cmd->argv[i]);
        else
            printf ("\t\targv[%d]: NULL\n", i);

    if (p_cmd->input)
        printf ("\tInput: \"%s\"\n", p_cmd->input);

    if (p_cmd->output)
        printf ("\tOutput: \"%s\"\n", p_cmd->output);

    if (p_cmd->output_err)
        printf ("\tError output: \"%s\"\n", p_cmd->output_err);

    printf ("\tExecute in the background: %s\n",
            p_cmd->background ? "Yes" : "No");
}
										//Ya lo he conseguido pasar a execute.c
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
/*void Ejecutar_Comando(command * p_cmd){
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
						else if(p_cmd->background==1){
							if(waitpid(ident,&status,WNOHANG)==-1) //supuestamente el WNOHANG lo hace no bloqueante
																										 //es decir que no interrumpe, y vuelve a poner el promptt
								{
									perror("PADRE: Se ha producido un error \n");
									exit(EXIT_FAILURE);
								}
						}
    }
}*/