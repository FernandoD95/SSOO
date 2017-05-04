
.PHONY: clean 

all: minishell

MINISHELL_VERSION=\"2017.1\"
ARCH_LIB_DIR=lib/$(shell uname -ms | tr ' ' '/')

minishell: execute.o minishell.o minishell_input.o jobs.h execute.h internals.h
	gcc -ggdb -Wall -o minishell minishell.o minishell_input.o execute.o -L${ARCH_LIB_DIR} -lshell

minishell.o: minishell.c minishell_input.h internals.h jobs.h execute.h parser.h
	gcc -ggdb -Wall -c -o minishell.o minishell.c

minishell_input.o: minishell_input.c minishell_input.h
	gcc -ggdb -Wall -c -o minishell_input.o minishell_input.c

jobs.o: jobs.c jobs.h
	gcc -ggdb -Wall -DMINISHELL_VERSION=$(MINISHELL_VERSION) -c -o jobs.o jobs.c

execute.o: execute.c parser.h execute.h
	gcc -ggdb -Wall -DMINISHELL_VERSION=$(MINISHELL_VERSION) -c -o execute.o execute.c

internals.o: internals.c internals.h
	gcc -ggdb -Wall -DMINISHELL_VERSION=$(MINISHELL_VERSION) -c -o internals.o internals.c

clean:
	rm -rf minishell *.o

