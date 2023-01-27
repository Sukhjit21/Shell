# Project1
Implementation of a simple shell 

adadadadadadadadadadadadadadadadadadadadadadadadadadadadadadadadadadadadadadadad

# Design Choices
### struct Arguments

We used a struct named **Arguments** with four arrays to store the command line. 

1. char *firstarg[] stores the the command line commands before '>'
2. char *secondarg[] stores the the command line commands after '>'
3. char *thirdarg[] stores the second job that pipeline needs to do
4. char *fourtharg[] stores the third command in a pipe

Then there are three int variables that act as flags:

1. int outputre: if 1 then output redirection, if 2 then append to file
2. int background: stores 1 if its a background process
3. int pipe: if this variable is > 0, then executes pipe

### Main()

- We create a pointer to the struct Arguments called first
- Next we implement an infinite while loop to print out the prompt "sshell", \
keeps prompting the user for commands until they enter "exit". 
- Otherwise, when the user enters a command, we parse it and set the \
appropriate variables in the "Arguments" struct.
- **char *token** on line 71 checks if the command being parsed is valid, and if\
so proceeds to execute the corresponding command using execvp() in a while \
loop that will continue until we reach the end of the command line input.
- Background processes are handled by forking, and we place a waitpid() call in \
the parent process to suspend execution of the parent until the child returns.
