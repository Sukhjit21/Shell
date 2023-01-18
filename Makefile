CC = gcc
CFLAGS = -Wall -Wextra -Werror

sshell: sshell.c
	$(CC) $(CFLAGS)  sshell.c -o sshell
clean :
	-rm -f *.o sshell

