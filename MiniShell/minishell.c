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

#include "execute.h"
#include "parser.h"
#include "minishell_input.h"

void show_command(command * p_cmd);
void Ejecutar_Comando(command * p_cmd);


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
            //printf("%s\n",cmds[i].raw_command);
            //show_command(&cmds[i]);
            Ejecutar_Comando(&cmds[i]);
            
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


void Ejecutar_Comando(command * p_cmd){
    pid_t ident, pid;
    int status;
    //printf("%s\n",p_cmd->argv[0]);
    ident = fork();
    if(0==strcmp(p_cmd->argv[0],"exit")){
        exit(EXIT_SUCCESS);
    }
    
    switch(ident){
        case -1:
            exit(EXIT_FAILURE);
        
        case 0:
        if (execvp(p_cmd->argv[0], p_cmd->argv) < 0)
            //if (execl("/bin/ls", "ls", "-a", NULL) < 0)
            perror("HIJO: Error al ejecutar execvp \n");
	        exit(EXIT_FAILURE);
        
        default:
            pid=wait(&status);
            if (pid == -1)
			{
				perror("PADRE: Se ha producido un error \n");
				exit(EXIT_FAILURE);
			}
			//else if (pid == ident)                 
			    //printf("PADRE: El hijo ha finalizado \n"); 
        
    }

   
}