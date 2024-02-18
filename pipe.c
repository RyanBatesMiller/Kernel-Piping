#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

void execute_program(char *program)
{
	if (execlp(program, program, NULL) < 0) {
        perror("bogus");
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char *argv[]) 
{
	if (argc < 2) { //invalid or no programs to run
		exit(EINVAL);
	}

	int num_programs = argc - 1;

	if (num_programs == 1) { //only one program to run
		execute_program(argv[1]);
	}

	int fds[2];

	for (int i = 1; i <= num_programs; i++) {

		if (i != num_programs) //last program skip this, dont need to create pipe :)
		{
			if (pipe(fds) == -1) {
				perror("pipe");
				exit(EXIT_FAILURE);
			}
		}

		int status;
		int pid = fork();

		if (pid == -1) {
			perror("fork");
			exit(EXIT_FAILURE);
		}

		if (pid == 0) { 										//child process
			if (i != num_programs) { 							//last program skips this, dont need to redirect STDOUT :)
				dup2(fds[1], STDOUT_FILENO);
			}
			close(fds[0]);
			close(fds[1]);
			execute_program(argv[i]);							//execute child program
		}

		else if (pid > 0) { 									//parent process
			dup2(fds[0], STDIN_FILENO); 						//redirect STDIN
			close(fds[1]);
			close(fds[0]);

			waitpid(pid, &status, 0);							//wait for child to finish up
			if (WIFEXITED(status) && WEXITSTATUS(status) != 0) 	
			{
				exit(EXIT_FAILURE);
			}
		}
		
	}

	return 0;
}