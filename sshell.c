#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#define CMDLINE_MAX 512

// Function for built in command cd
int changedirec(char *path[]);
struct Arguments
{
        char *firstarg[CMDLINE_MAX];  // stores all the command line commands before >
        char *secondarg[CMDLINE_MAX]; // stores all command line commands after >
        int outputre;                 // if outputre = 1 then we do output redirection
                      //
};
int outputredirect(struct Arguments s);
int main(void)
{
        struct Arguments first;
        char cmd[CMDLINE_MAX];

        while (1)
        {
                char *nl;
                int status;
                int idfork;
                /* Print prompt */
                printf("sshell$ ");
                fflush(stdout);

                /* Get command line */
                fgets(cmd, CMDLINE_MAX, stdin);

                /* Print command line if stdin is not provided by terminal */
                if (!isatty(STDIN_FILENO))
                {
                        printf("%s", cmd);
                        fflush(stdout);
                }

                /* Remove trailing newline from command line */
                nl = strchr(cmd, '\n');
                if (nl)
                        *nl = '\0';

                /* Builtin command */
                if (!strcmp(cmd, "exit"))
                {
                        fprintf(stderr, "Bye...\n");
                        fprintf(stderr, "+ completed 'exit' [0]\n");
                        break;
                }
                char cwd[256];
                char *argv[CMDLINE_MAX];
                int argc = 0;
                char *token;
                int xz = 0;
                token = strtok(cmd, " ");
                while (token != NULL)
                {
                        if (!strcmp(token, ">"))
                        {
                                first.outputre = 1;
                                argc++;
                                token = strtok(NULL, " ");
                                continue;
                        }
                        if (!strcmp(token, ">>"))
                        {
                            first.outputre = 2;
                            argc++;
                            token = strtok(NULL, " ");
                            continue;                            
                        }
                        else if ((first.outputre == 1) || (first.outputre == 2))
                        {
                                first.secondarg[xz] = token;
                                //	printf("First arg %s\n", first.secondarg[0]);
                                xz++;
                                //	token = strtok(NULL, " ");
                        }
                        else
                        {
                                first.firstarg[argc] = token;
                                argv[argc] = token;
                        }
                        argc++;
                        token = strtok(NULL, " ");
                }
                // strcpy(first.firstarg, argv[3]);
                argv[argc] = NULL;
                first.firstarg[argc] = NULL;
                /*Builtin command for PWD */
                /* if (!strcmp(cmd, "pwd"))
                {
                        getcwd(cwd, sizeof(cwd));
                        printf("%s\n", cwd);
                        fprintf(stderr, "+ completed 'pwd' [0]\n");
                        continue;
                } */
                if (!strcmp(cmd, "cd"))
                {
                        changedirec(first.firstarg);
                        continue;
                }
               /* if (!strcmp(cmd, "pipe"))
                {

                        piping(first);
                } */
                idfork = fork();
                /*Child Process*/
                if (idfork == 0)
                {
                    if(!strcmp(cmd, "pwd")){
                        getcwd(cwd, sizeof(cwd));
                        printf("%s\n", cwd);
                        exit(0);
                }
                    if (first.outputre != 0)
                    {
                        outputredirect(first);
                        status = execvp(first.firstarg[0], first.firstarg);
                        exit(0);
                    }
                    else
                    {
                        status = execvp(first.firstarg[0], first.firstarg);
                        printf("Error executing command: %s\n", argv[0]);
                        exit(1);
                    }
                }
                /* Parent Process */
                else
                {
                        first.outputre = 0; // set to 0 so it does not run output redirection on next cmd
                        waitpid(idfork, &status, 0);
                        fprintf(stderr, "+ completed '%s' ['%d']\n",
                                cmd, status);
                }
        }

        return EXIT_SUCCESS;
}

int changedirec(char *path[])
{
        if (chdir(path[1]) == 0)
        {
            fprintf(stderr, "+ completed 'cd %s' [%d]\n", path[1], 0);
            return 0;
        }

        return -1;
}
int outputredirect(struct Arguments s)
{
        int fd;
        char filenam[70];
        strcpy(filenam, s.secondarg[0]);
        if(s.outputre == 2){
            fd = open(filenam, O_WRONLY|O_APPEND| O_CREAT, 0644);
            
        }
        else{
            fd = open(filenam, O_WRONLY| O_TRUNC | O_CREAT, 0644);
        }
        dup2(fd, STDOUT_FILENO);
        close(fd);
        return 0;
}
