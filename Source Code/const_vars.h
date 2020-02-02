#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>

#define PIPE_COUNT 3
#define PROCESS_COUNT 2
#define BUFF_SIZE 256
#define INPUT_SIZE 1
