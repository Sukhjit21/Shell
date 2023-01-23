#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define CMDLINE_MAX 512

int main(void)
{
        char cmd[CMDLINE_MAX];

        while (1) {
                char *nl;
                int status;
                int idfork;
                /* Print prompt */
                printf("sshell$ ");
                fflush(stdout);

                /* Get command line */
                fgets(cmd, CMDLINE_MAX, stdin);

                /* Print command line if stdin is not provided by terminal */
                if (!isatty(STDIN_FILENO)) {
                        printf("%s", cmd);
                        fflush(stdout);
                }

                /* Remove trailing newline from command line */
                nl = strchr(cmd, '\n');
                if (nl)
                        *nl = '\0';
                

                /* Builtin command */
                if (!strcmp(cmd, "exit")) {
                        fprintf(stderr, "Bye...\n");
                        break;
                }
                char cwd[256];
                char* argv[CMDLINE_MAX];
                int argc = 0;
                char *token;

                token = strtok(cmd, " ");
                while (token != NULL)
                {
                    argv[argc] = token;
                    argc++;
                    token = strtok(NULL, " ");
                }
                argv[argc] = NULL;
                /*Builtin command for PWD */
		if(!strcmp(cmd, "pwd")){
			getcwd(cwd,sizeof(cwd));
			printf("%s\n", cwd);
                        fprintf(stderr, "+ completed 'pwd' [0]\n");	
			continue;
		}
		if(!strcmp(cmd, "cd")){
                        chdir(argv[1]);
                        fprintf(stderr, "+ completed 'cd %s' [0]\n", argv[1]);
                        continue;
                }

                idfork = fork();
                /*Child Process*/
                if(idfork == 0){
                        status = execvp(argv[0], argv);
                        /* execvp returns only if error occurs */
                        printf("Error executing command: %s\n", argv[0]);
                        exit(1);
                }
                /* Parent Process */
                else{
                        waitpid(idfork, &status, 0);
                        fprintf(stderr, "+ completed '%s' ['%d']\n",
                        cmd,status);
                }
        }

        return EXIT_SUCCESS;
}
