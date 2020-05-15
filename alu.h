#define ALU_H
#ifdef ALU_H

#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <string.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <unistd.h>

#include "L1/mmemory.h"
#include "L4/readKey.h"

/*global vars for timer*/
struct itimerval nval, oval;
int accum;
int mem_ptr, operation1 , operation2;
int sc_accumGet(int *value);
void timer_init();
int switchbit(const int value, const int position);
int positive(int num);
int getNum(int *n);
int ALU(int command, int operand);
int CU();
void sighandler(int sig);

#endif