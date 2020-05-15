#define PRINT_H
#ifdef PRINT_H

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "L2/MyTerm.h"
#include "L3/bigChar.h"
#include "alu.h"

#define TMP_STR_LEN 20

int ip;
int f_key, f_ign;

int displayBorders();
int displayAccumulator();
int displayMemory();
int displayBigChar();
int displayFlags();
int displayOperation();
int displayIp();
void displayUI();
void clearInput();
void reset();
void user_input_accum();
void user_input_ip();
void user_input_mem();


#endif
