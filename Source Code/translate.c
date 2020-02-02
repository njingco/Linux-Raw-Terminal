#include <unistd.h>
#include "translate.h"

/*---------------------------------------------------------------------------------------
--	FUNCTION:			translate
--
--	DATE:				January 21, 2020
--
--	REVISIONS:			January 22, 2020 - Fixed the X functionality
--
--	DESIGNERS:			Nicole Jingco
--
--	PROGRAMMERS:		Nicole Jingco
--  
--  INTERFACE:          translate(const int* input, const int* output)
--                      const int* input - pipe for translate and input processes
--                      const int* output - pipe from translate and output process
--
--  RETURNS:
--
--	NOTES:
--	This function will read from the input pipe and process each character. If its a 
--  special character, it will perform a different function (X to erase, K to kill 
--  line, A to Z, and a to z). After the read buffer is done getting modified, it will 
--  be writted to the output pipe to be echoed to the terminal. 
---------------------------------------------------------------------------------------*/
void translate(const int * input, const int * output) {
    char readBuff[BUFF_SIZE];
    char modBuff[BUFF_SIZE];
    int counter = 0;
    int xcount = 1;

    for (;;) {
        // READ FROM INPUT[0]
        if (read(input[0], readBuff, BUFF_SIZE) == -1){
            fprintf(stderr, "Unable to read from input to translate\n");
        }
        else {
            counter = 0;
            xcount = 1;
            for (int i = 0; i < BUFF_SIZE; i++) {
                if (readBuff[i] == 'X'){            
                    if (i != 0){
                        readBuff[i] = '\x04';
                        readBuff[i-xcount] = '\x04';
                        xcount+=2;
                    } 
                } else {
                    xcount = 1;
                    if (readBuff[i] == 'K'){
                        counter = i+1;
                    }
                }
            }
            strncpy(modBuff, &readBuff[counter], BUFF_SIZE);
            for (int i = 0; i < BUFF_SIZE; i++) {
                switch (modBuff[i]) {
                    case 'A':
                        //REPLACING A WITH Z
                        modBuff[i] = 'Z';
                        break;
                    case 'a':
                        modBuff[i] = 'z';
                        break;
                    default:
                        break;
                }
            }
            if (write(output[1], modBuff, BUFF_SIZE) == -1)
                fprintf(stderr, "Unable to write from translate to output\n");
        }
    }

}
