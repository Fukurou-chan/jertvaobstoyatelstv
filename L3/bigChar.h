#ifndef BIGCHAR_H
#define BIGCHAR_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "../L2/MyTerm.c"

#define REC 'a'
#define BR "j"
#define BL "m"
#define TR "k"
#define TL "l"
#define VERT "x"
#define HOR "q"

#define TERM 		"/dev/tty"

#define ENTER_ALT_CHARSET_MODE "\E(0"
#define EXIT_ALT_CHARSET_MODE  "\E(B"
#define ESC_STR_LEN strlen(ENTER_ALT_CHARSET_MODE)

#define SPACE       " "
#define ACS_CKBOARD "a"


int big_chars[256];
const char CHARS[]    = "+0123456789abcdef";

int bc_printbigchar(int *big, int x, int y, enum colors fg, enum colors bg);
int bc_printA(char *str);
int bc_box(int x1, int y1, int x2, int y2);
int bc_setbigcharpos(int *big,  int value);
int bc_getbigcharpos(int *big, int x, int y, int *value);
int bc_bigcharwrite(int fd, int *big, int count);
int bc_bigcharread(int fd, int *big, int need_count, int *count);

#endif 


