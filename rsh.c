#include <stdio.h>
#include <stdlib.h>
#include <spawn.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define N 12
#define MAX_ARGS 21

extern char **environ;

char *allowed[N] = {"cp","touch","mkdir","ls","pwd","cat","grep","chmod","diff","cd","exit","help"};

int isAllowed(const char*cmd) {
	// TODO
	// return 1 if cmd is one of the allowed commands
	// return 0 otherwise
	size_t allowed_size = sizeof(allowed) / sizeof(allowed[0]);

	for ( size_t i = 0 ; i < allowed_size ; i++ ) {
		if (strcmp(cmd,allowed[i]) == 0) return 1;  
	}//end for

	return 0;
}//end isAllowed

int main() {

    	// TODO
	// Add variables as needed

    	char line[256];
	char line_copy[256];
	char *argv[MAX_ARGS];
	int argc;
    	int run = 1;

	pid_t pid;
	int status;


    
    	while (run == 1) {

		fprintf(stderr,"rsh>");

		if (fgets(line,256,stdin)==NULL) continue;

		if (strcmp(line,"\n")==0) continue;

		line[strlen(line)-1]='\0';
		
		strcpy(line_copy, line);

		char *token = strtok(line_copy, " ");
		argc = 0;

		while (token != NULL && argc < MAX_ARGS - 1) {
			argv[argc++] = token;
			token = strtok(NULL, " ");
		}//end tokenizer while
		argv[argc] = NULL;

		if (argc == 0) continue;

		char *cmd = argv[0];

		if (isAllowed(cmd) == 1) {
			if(strcmp(cmd,"exit")==0) {
				run = 0;
				break;
			//end exit if
			} else if (strcmp(cmd,"help") == 0) {
				printf("The allowed commands are:\n");	
				size_t allowed_size = sizeof(allowed) / sizeof(allowed[0]);

        			for ( size_t i = 0 ; i < allowed_size ; i++ ) {
        				printf("%zu: %s\n", i+1, allowed[i]);
        			}//end print for
			//end help if
			} else if (strcmp(cmd,"cd") == 0) {
				if (argc > 2) {
					printf("-rsh: cd: too many arguments\n");
					//end check if
				} else if (argc == 1) {
					const char *home_dir = getenv("HOME");
					//if (home_dir && chdir(home_dir) != 0) {
					//}
				} else {
					if (chdir(argv[1]) != 0) {
						fprintf(stdout, "-rsh: cd: %s: %s\n", argv[1], strerror(errno));
					}//end error if
				}//end else
			//end cd if
			} else {
				int spawn_result = posix_spawnp(&pid, cmd, NULL, NULL, argv, environ);

				if (spawn_result != 0) {
					fprintf(stdout, "rsh: Error executing %s: %s\n", cmd, strerror(spawn_result));
					//end spawnfail if
				} else {
					if (waitpid(pid, &status, 0) == -1) {
						perror("waitpid failed");
					}//end wait if
				}//end wait else
			}//end spawn else

		} else {
			printf("NOT ALLOWED!\n");
		}//end isAllowed if

		// TODO
		// Add code to spawn processes for the first 9 commands
		// And add code to execute cd, exit, help commands
		// Use the example provided in myspawn.c

    	}//end rsh while
    	return 0;
}//end main
