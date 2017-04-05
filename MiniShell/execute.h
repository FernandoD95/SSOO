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

int execute_piped_command_line(command* cmds, int n);

#endif __EXECUTE_H__
