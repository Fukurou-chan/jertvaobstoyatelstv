#ifndef MYTERM_H
#define MYTERM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>

#define CLEAR 		"\E[H\E[J"
#define GOTOXY 		"\E[%d;%dH"
#define SETFGCOLOR 	"\E[3%dm"
#define SETBGCOLOR 	"\E[4%dm"
#define ESCSPACE	"\E[1C"
#define ESC 		'\E'

#define TERM 		"/dev/tty"


enum colors { black, red, green, yellow, blue, purple, lightblue, white, pink, deflt };

int mt_clrscr(void);
int mt_gotoXY(int p1, int p2);
int mt_getscreensize(int* a, int* b);
int mt_setfgcolor(enum colors n);
int mt_setbgcolor(enum colors n);

#endif
