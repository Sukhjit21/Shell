#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define CMDLINE_MAX 512

int main(int argc, char *argv[])
{
        char cmd[CMDLINE_MAX];

        while (1) {
                char *nl;
                //int retval;
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

                if(argc > 1) {
                        strcpy(cmd,argv[1]);
                        for(int i=2;i<argc;i++){
                                strcat(cmd," ");
                                strcat(cmd,argv[i]);
                        }
                        char* argument_list[] = {NULL};
                        idfork = fork();
                        wait(&status);
                        /* Child Process */
                        if (idfork == 0) {
                                status = execvp(cmd, argument_list);
                        }
                        /* Parent Process */
                        else {
                                fprintf(stderr, "+ completed '%s' ['%d']\n", cmd, status);
                        }
                        break;
                }
        }

        return EXIT_SUCCESS;
}

