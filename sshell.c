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
                        else if (first.outputre == 1)
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
                if (!strcmp(cmd, "pwd"))
                {
                        getcwd(cwd, sizeof(cwd));
                        printf("%s\n", cwd);
                        fprintf(stderr, "+ completed 'pwd' [0]\n");
                        continue;
                }
                if (!strcmp(cmd, "cd"))
                {
                        changedirec(first.firstarg);
                        continue;
                }
                idfork = fork();
                /*Child Process*/
                if (idfork == 0)
                {
                        printf("output value %d", first.outputre);
                        if (first.outputre == 1)
                        {
                                outputredirect(first);
                                status = execvp(first.firstarg[0], first.firstarg);
                                exit(0);
                        }
                        else
                        {
                                status = execvp(first.firstarg[0], first.firstarg);
                                //		status = execvp(argv[0], argv);
                                /* execvp returns only if error occurs */
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
        printf("Made it ot file name '%s' ", filenam);
        fd = open(filenam, O_WRONLY | O_CREAT, 0644);
        dup2(fd, STDOUT_FILENO);
        close(fd);
        return 0;
}

int piping(struct Arguments s)
{
        int fd[2];
        pid_t pid = fork();

        // create the pipe
        if (pipe(fd) == -1) {
                perror("pipe");
                exit(EXIT_FAILURE);
        }
        
        if (pid == -1) {
                perror("fork");
                exit(EXIT_FAILURE);
        }

        if (pid == 0) {
                /* child process */
                close(fd[0]); // close the read end of the pipe
                dup2(fd[1], STDOUT_FILENO); // redirect stdout to the write end of the pipe
                close(fd[1]);
                execlp("ls", "ls", "-l", NULL); // execute 'ls -l' command
                perror("execlp");
                exit(EXIT_FAILURE);
        } else {
                /* parent process */
                close(fd[1]); // close the write end of the pipe
                dup2(fd[0], STDIN_FILENO); // redirect stdin to the read end of the pipe
                close(fd[0]);
                execlp("grep", "grep", "^d", NULL); // execute 'grep ^d' command
                perror("execlp");
                exit(EXIT_FAILURE);
        }

        return 0;

}
