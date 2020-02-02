/*---------------------------------------------------------------------------------------
--	SOURCE FILE:		main.c -   A simple Linux terminal keystroke manipulator
--
--	PROGRAM:			main
--
--	FUNCTIONS:			main
--
--	DATE:				January 21, 2020
--
--	REVISIONS:			January 22, 2020 - Commented around the code
--
--	DESIGNERS:			Nicole Jingco
--
--	PROGRAMMERS:		Nicole Jingco
--
--	NOTES:
--	This program will process read characters froma terminal keyboard and echo the
--  character tot th terminal screen process each line of character and write the
--  process line to the screen which will translate certain keystrokes to a different 
--  function or output.
---------------------------------------------------------------------------------------*/

#include "main.h"

pid_t pid[PROCESS_COUNT];

int main(int argc, char* argv[]) {
	
	// DISABLE KEYBOARD TERMINAL FUNCITONS
	system("stty raw igncr -echo");

	/* Array to store pipes
	 * 0 - INPUT | TRANSLATE
	 * 1 - INPUT | OUTPUT
	 * 2 - TRANSLATE | OUTPUT
	 */
	int pipe_arr[3][2];
	int i;

	// CREATE PI[ES
	for (i = 0; i < PIPE_COUNT; i++) {
		if (pipe(pipe_arr[i]) == -1) {
			fprintf(stderr, "Error opening pipe\r\n");
		} 
	}

	// SET NON-BLOCKING FLAGS (OUTPUT|TRANSLATE)
	if (fcntl(pipe_arr[1][0],F_SETFL, O_NDELAY) < 0) {
		fprintf(stderr, "Error on fcntl\r\n");
	}
	if (fcntl(pipe_arr[2][0], F_SETFL, O_NDELAY) < 0) {
		fprintf(stderr, "Error on fcntl\r\n");
	}

    // CREATE CHILD
	for (i = 0; i < 2; i++) {
		if ((pid[i] = fork()) == -1) {
			fprintf(stderr, "Error creating process\r\n");
		} else if (pid[i] == 0) {
            break;
		}
	}
    
	// OUTPUT PROCESS
	if (i == 0) {
		output(pipe_arr[1],pipe_arr[2]);

	// TRANSLATE PROCESS
	} else if (i == 1) {
		translate(pipe_arr[0],pipe_arr[2]);

	// INPUT PROCESS
	} else if (i == 2) {
		fprintf(stdout, "\r\nNormal Output: \r\n");
		input(pipe_arr[1],pipe_arr[0]);
		
        // ENABLE KEYBOARD TERMINAL FUNCITONS
        system("stty -raw -igncr echo");
        
        // KILL PROCESS
		kill(pid[0],SIGKILL);
		kill(pid[1],SIGKILL);

		// CLOSING PIPES
		for (i = 0; i < PIPE_COUNT; i++) {
			close(pipe_arr[i][0]);
			close(pipe_arr[i][1]);
		}

	}

	fprintf(stdout, "Keyboard Normal\r\n");
}
