# Project1
Implementation of a simple shell 

adadadadadadadadadadadadadadadadadadadadadadadadadadadadadadadadadadadadadadadad

# Design Choices
### Arguments

We used a struct named <mark>Arguments</mark> with four arrays to store the 
command line. 

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
- Next we implement an infinite while loop to print out the prompt "sshell", \n
keeps prompting the user for commands until they enter "exit". 
- Otherwise, when the user enters a command, we parse it and set the 
appropriate variables in the "Arguments" struct.
