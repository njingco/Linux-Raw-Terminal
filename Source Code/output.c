#include <unistd.h>
#include "output.h"

/*---------------------------------------------------------------------------------------
--	FUNCTION:			output
--
--	DATE:				January 21, 2020
--
--	REVISIONS:			January 22, 2020 - Fixed the X functionality
--
--	DESIGNERS:			Nicole Jingco
--
--	PROGRAMMERS:		Nicole Jingco
--  
--  INTERFACE:          translate(const int* input, const int* translate)
--                      const int* input - pipe for output and input processes
--                      const int* translate - pipe from output and translate process
--
--  RETURNS:
--
--	NOTES:
--	This function will read from the input and translate pipes and echo the characters
--  to the terminal.
---------------------------------------------------------------------------------------*/
void output(const int * input, const int * translate) {
	char readBuff[BUFF_SIZE];
	for (;;) {
        
        //HADLES PRINTING THE INPUT PROCESS
		if (read(input[0], readBuff, BUFF_SIZE) > 0) {
			// PRINT CHAR
			printf("%c", readBuff[0]);
			// RESET BUFFER
			for (int i = 0; i < BUFF_SIZE; i++) {
				readBuff[i] = '\0';
			}
		}

        //HADLES PRINTING THE TRANSLATE PROCESS
		if (read(translate[0], readBuff, BUFF_SIZE) > 0) {
			fprintf(stdout, "\r\n\nTranslated Output: \r\n");
            
			// PRINT TRANSLATED STRING WHILE RESETING THE BUFFER
            for (int i=0; i < BUFF_SIZE; i++){
                if (readBuff[i] !='\x04')
                    printf("%c", readBuff[i]);
                readBuff[i] = '\0';                
            }
            
            fprintf(stdout, "\r\n\n----------------------- \r\n");
            fprintf(stdout, "\r\nNormal Output: \r\n");
        }
        fflush(stdout);
    }
    
}
