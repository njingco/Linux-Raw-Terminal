#include <unistd.h>
#include "input.h"

/*---------------------------------------------------------------------------------------
--	FUNCTION:			input
--
--	DATE:				January 21, 2020
--
--	REVISIONS:			January 22, 2020 - Included the force kill functionality
--
--	DESIGNERS:			Nicole Jingco
--
--	PROGRAMMERS:		Nicole Jingco
--  
--  INTERFACE:          input(const int* output, const int* translate)
--                      const int* output - pipe for input and output processes
--                      const int* translate - pipe from input and translate process
--
--  RETURNS:
--
--	NOTES:
--	This function take keystrokes from the terminal and process its equivalent 
--  functionality. Each keystroke will be writtenr to the output process
--  to be echoed to the terminal, as well as be stored to a buffer. When certain 
--  keystrokes are entered(T & E) it will be translated. If ^K is entered the program
--  will close.
---------------------------------------------------------------------------------------*/

void input(const int* output, const int* translate) {
    char readBuff[BUFF_SIZE] = {0};
    int count=0;
    char c;

    for(; ; ) {
        c=fgetc(stdin);

        if (c==(char)11) {
            //KILL ALL CHILDREN AND EXIT FROM PROGRAM 
            printf("\r\n");
            kill(0,9);
            break;
        }

        else if (c=='T') {
            //WRITE KEYPRESS TO OUTPUT[1]
            if (write(output[1], &c, INPUT_SIZE)==-1) 
                fprintf(stderr, "Unable to write from input to output\n");

            // CHECK BUFFER SIZE
            if (count >=BUFF_SIZE) 
                fprintf(stdout, "\r\n\nBuffer is full. Will only translate what was in the buffer.\r\n");

            //WRITE READBUFFER TO TRANSLATE[1]
            if (write (translate[1], readBuff, BUFF_SIZE)==-1) 
                fprintf(stderr, "Unable to write from input to translate\n");

            system("stty -raw -igncr echo");
            break;
        }        

        else if (c=='E') {
            
            //WRITE KEYPRESS TO OUTPUT[1]
            if (write(output[1], &c, INPUT_SIZE)==-1) 
                fprintf(stderr, "Unable to write from input to output\n");

            //translate
            if (count >=BUFF_SIZE) {
                printf("\r\nBuffer is full. Will only translate what was in the buffer.\r\n");
            }

            //WRITE READBUFFER TO TRANSLATE[1]
            if (write (translate[1], readBuff, BUFF_SIZE)==-1) 
                fprintf(stderr, "Unable to write from input to translate\n");
            
            // reset buffer
            count=0;

            for (int i=0; i < BUFF_SIZE; i++) {
                readBuff[i]='\0';
            }
        }

        else {
            // SETS THE KEYSTROKE CHARACTER TO THE BUFFER
            if (count < BUFF_SIZE) {
                readBuff[count]=c;
            }

            //WRITE KEYPRESS TO OUTPUT[1]
            if (write(output[1], &c, INPUT_SIZE)==-1) 
                fprintf(stderr, "Unable to write from input to output\n");
            
            count++;
        }
    }
}
