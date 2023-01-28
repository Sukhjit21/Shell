# Project1 - **Implementation of a simple shell**

# Development Process
### Phase 1 - Fork, Execvp, Wait

- For this first phase, we just created a simple while loop in main() that uses \
fork(), execvp(), and wait() instead of system().
- The shell then prints out the completed exit statys to stderr.

### Phase 2 - Arguments

- To handle command line arguments, we had to parse the input into multiple \
commands. Initially, we created a *char array to store the arguments, then ran \
through the list to execute each command. However, this approach proved to be \
suboptimal, and oftentimes we would run into errors when trying to implement \
multiple commands. 
- Later on, we switched to a struct to handle the input arguments. This is the \
design we are currently, which allows us to isolate each command on its own \
in order to handle more complex commands, such as multiple pipelines and \
background tasks.

### Phase 3 - Builtin Commands

- In this phase we just implemented the rest of the builtin commands. 
- We checked the input line for the builtin commands, and wrote a series of \
simple *if-else* statements to handle pwd, cd, etc. 
- Wrote a changedirec() function that takes the PATH as an input char array, \
 then implements the *cd* command and prints the status and path to *stderr*

### Phase 4 - Output Redirection

- To enable output redirection, we created the function int outputredirect() \
that takes the *struct Argument* as the input parameter. We first have to \
check whether the output file that the user wants to redirect the output to \
actually exists or not. If not, we print an error message and exit(1).
- If no error occurs, and the first argument in the command line is not NULL, \
then we can start to redirect the output. We append the output to the file \
if that file already exists, otherwise we create a new file. The user can \
also specify whether they want to overwrite the file, or to simply append \
the output to the end of the file.

### Phase 5 - Piping

- This was the phase we had the most trouble with. We tried to implement \
a loop that operated as a general piping function that could handle any number \
of piping commands, but ended up having numerous issues that led to the piping \
commands not being executed by the shell.
- In the end, we decided to write two functions, one that would run when there \
was only one piping command, and another one to handle multiple pipes.
- Both functions take *struct Argument* as the parameter, and create pipes \
between the different commands in the command line. However, in pipeline2(), \
there are two file descriptors instead of just one in order to handle \
multiple commands.

### Phase 6 - Extra Features

- We made the necessary changes so that the user can append to the end of \
the file when redirecting the output rather than truncate the content.
- We also needed to implement background processing, which is done through \
waitpid(). We call this in the parent process in main(), and wait until \
all background processes have terminated to print the completion message.

# Code Design Choices
### struct Arguments

We used a struct named *Arguments* with four arrays to store the command line. 
- We decided to parse the command line into a struct instead of just treating \
it like a std::string, in order to better split up and iterate through the \
commands. This way, we can also implement flag variables to specift modes.

1. char *firstarg[] stores the the command line commands before '>'
2. char *secondarg[] stores the the command line commands after '>'
3. char *thirdarg[] stores the second job that pipeline needs to do
4. char *fourtharg[] stores the third command in a pipe

Then there are three int variables that act as flags:

1. int outputre: if 1 then output redirection, if 2 then append to file
2. int background: stores 1 if its a background process
3. int pipe: if this variable is > 0, then executes pipe

### int main()

- We create a pointer to the struct Arguments called first
- Next we implement an infinite while loop to print out the prompt "sshell", \
keeps prompting the user for commands until they enter "exit". 
- Otherwise, when the user enters a command, we parse it and set the \
appropriate variables in the "Arguments" struct.
- *char *token* on line 71 checks if the command being parsed is valid, and if\
so proceeds to execute the corresponding command using execvp() in a while \
loop that will continue until we reach the end of the command line input.
- Background processes are handled by forking, and we place a waitpid() call in \
the parent process to suspend execution of the parent until the child returns.
- Will print completed process after returning, and also prints error message \
if error occurs. 

### int changedirec()
- Called from main when the command is *cd*.
- If a path is provided from the user, will implement the *chdir* command.
- Else prints error message, then returns.

### int outputredirect()
- Takes in the Argument struct as command line arguments.
- Returns error if there is no secondary argument provided.
- Will either call *O_TRUNC* or *O_APPEND* depending on usage mode, then \
calls dup2() on the file descriptor and closes the file.

### int pipeline() and int pipeline2()
- We created two pipeline functions in order to facilitate multiple piping calls
- Calls fork, creates the parent and child processes.
- pipeline() handles only one piping call, while pipeline2() handles more than 1.

# Sources
- [Multiple pipe implementation reference from stackoverflow](https://stackoverflow.com/questions/8389033/implementation-of-multiple-pipes-in-c)
- [GNU C Library](https://www.gnu.org/software/libc/manual/)
- Lecture slides
