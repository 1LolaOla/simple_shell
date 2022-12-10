#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define BUFFER_SIZE 1024

int main(int argc, char **argv)
{
    char *input;
    char buffer[BUFFER_SIZE];

    while (1)
    {
        printf("%s$ ", argv[0]);
        input = fgets(buffer, BUFFER_SIZE, stdin);

        // If the user entered a command
        if (input != NULL)
        {
            // Remove the newline character from the input
            input[strcspn(input, "\n")] = 0;

            // Check if the user entered the "exit" command
            if (strcmp(input, "exit") == 0)
            {
                break;
            }

            // Create a child process to execute the command
            pid_t child_pid = fork();
            if (child_pid == 0)
            {
                // Child process: execute the command using the "execve()" system call
                char *args[] = {input, NULL};
                int status = execve(input, args, NULL);

                // If the command was not successful, print an error message
                if (status == -1)
                {
                    perror(argv[0]);
                    return 1;
                }
            }
            else if (child_pid > 0)
            {
                // Parent process: wait for the child process to complete
                int status;
                waitpid(child_pid, &status, 0);
            }
            else
            {
                // Error: fork() failed
                perror(argv[0]);
                return 1;
            }
        }
        else
        {
            break;
        }
    }

    return 0;
}
