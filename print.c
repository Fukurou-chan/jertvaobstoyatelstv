#include "print.h"

int displayBorders()
{
	int term = open(TERM, O_RDWR);
	if (term == -1) {
		fprintf(stderr, "%s\n", "displayBorders: terminal error ");
		close(term);
		return -1;
	}
	
	mt_clrscr();
	
	bc_box(1, 1, 12, 61);	
	bc_box(1, 62, 3, 22);	
	bc_box(4, 62, 3, 22);	
	bc_box(7, 62, 3, 22);	
	bc_box(10, 62, 3, 22);	
	bc_box(13, 1, 10, 46);	
	bc_box(13, 47, 10, 37);
		
	mt_gotoXY(1, 30);
	write(term, " Memory ", strlen(" Memory "));
	mt_gotoXY(1, 67);
	write(term, " accumulator ", strlen(" accumulator "));
	mt_gotoXY(4, 63);
	write(term, " instructionCounter ", strlen(" instructionCounter "));
	mt_gotoXY(7, 68);
	write(term, " Operation ", strlen(" Operation "));
	mt_gotoXY(10, 69);
	write(term, " Flags ", strlen(" Flags "));
	mt_gotoXY(13, 48);
	write(term, " Keys: ", strlen(" Keys: "));
	
	mt_gotoXY(14, 48);
	write(term, "l  - load", strlen("l  - load"));
	mt_gotoXY(15, 48);
	write(term, "s  - save", strlen("s  - save"));
	mt_gotoXY(16, 48);
	write(term, "r  - run", strlen("r  - run"));
	mt_gotoXY(17, 48);
	write(term, "t  - step", strlen("t  - step"));
	mt_gotoXY(18, 48);
	write(term, "e  - edit cell", strlen("e  - edit cell"));	
	mt_gotoXY(19, 48);
	write(term, "i  - reset", strlen("i  - reset"));
	mt_gotoXY(20, 48);
	write(term, "F5 - accumulator", strlen("F5 - accumulator"));
	mt_gotoXY(21, 48);
	write(term, "F6 - instructionCounter", strlen("F6 - instructionCounter"));
		
	mt_gotoXY(23, 1);
	close(term);
	
}

/* display Accumulator */
int displayAccumulator()
{
    int term = open(TERM, O_RDWR);
    if (term == -1) {
        fprintf(stderr, "%s\n", "displayAccumulator: terminal error");
        close (term);
        return -1;
    }

    char str_1[TMP_STR_LEN];

    mt_gotoXY(2, 69);
    write(term, "+0000", 5);
    if (accum >= 0) {
		sprintf(str_1, "+%04X    ", accum);
		mt_gotoXY(2, 69);
	} else {
		sprintf(str_1, "-%04X    ", -1 * accum);
		mt_gotoXY(2, 69);
	}

    write(term, str_1, strlen(str_1));

    close (term);
    mt_gotoXY(23, 1);
    return 0;
}

/* display Memory */
int displayMemory()
{
	int term = open(TERM, O_RDWR);
	if (term == -1) {
		fprintf(stderr, "%s\n", "displayBorders: terminal error");
		close(term);
		return -1;
	}
	
	int value, comm, oper, k;
	
	mt_gotoXY(2, 2);
	
	int j = 2, i = 0;
	
	for (i = 1; i <= 100; ++i) {
		
		sc_memoryGet(i - 1, &value);

		k    = value >> 14 & 0x1;
		oper = value & 0x7F;
		comm = value >> 7 & 0x7F;
		
		char buf[50];
		
		if (k)
			sprintf(buf, i % 10 ? " %04X " : " %04X", value & 0x1FFF);
		else		
			sprintf(buf, i % 10 ? "%c%02X%02X " : "%c%02X%02X", (k == 0) ? '+' : ' ', comm, oper);
		
		write(term, buf, strlen(buf));
		
		if (i % 10 == 0 && i != 0) {
			++j;
			mt_gotoXY(j, 2);
		}
		
	}
	
    char str_1[50];
    mt_gotoXY(mem_ptr / 10 + 2, mem_ptr % 10 * 6 + 2);
    
    sc_memoryGet(mem_ptr, &value);
    
    k    = value >> 14 & 0x1;
    oper = value & 0x7F;
    comm = value >> 7 & 0x7F;
    
    if (k)
		sprintf(str_1, " %04X", value & 0x1FFF);
    else
		sprintf(str_1, "%c%02X%02X", !k ? '+':' ', comm, oper);
    
    mt_setbgcolor(deflt);
    write(term, str_1, strlen(str_1));
    mt_setbgcolor(deflt);
    mt_gotoXY(23, 1);
    
    close(term);
}

/* display BigChar */
int displayBigChar()
{
    int big[] = {0, 0};
    int column = 2;
    int i, k, oper, comm, value;
    char str_1 [TMP_STR_LEN];

    sc_memoryGet(mem_ptr, &value);

    k    = value >> 14 & 0x1;
    oper = value & 0x7F;
    comm = value >> 7 & 0x7F;

    if (k)
		sprintf(str_1, " %04x", value & 0x1FFF);
    else
		sprintf(str_1, "%c%02X%02X", !k ? '+':' ', comm, oper);
		
    for (i = 0; i < 5; ++i, column += 9) {
        bc_setbigcharpos(big, str_1[i]);
        bc_printbigchar(big, 14, column, deflt, deflt);
    }

    mt_gotoXY(23, 1);
    return 0;
}

/* display Flags */
int displayFlags()
{
    int terminal = open("/dev/tty", O_RDWR);
    if (terminal == -1) {
        fprintf(stderr, "%s\n", "displayFlags: terminal error");
        close (terminal);
        return -1;
    }

    int k;
    mt_gotoXY(11, 63);
    sc_regGet(T, &k); if (k) write(terminal, "T ", 3); else write(terminal, "   ", 3);
    sc_regGet(O, &k); if (k) write(terminal, "O ", 3); else write(terminal, "   ", 3);
    sc_regGet(PC, &k); if (k) write(terminal, "PC ", 3); else write(terminal, "   ", 3);
    sc_regGet(M, &k); if (k) write(terminal, "M ", 3); else write(terminal, "   ", 3);
    sc_regGet(E, &k); if (k) write(terminal, "E ", 3); else write(terminal, "   ", 3);
    sc_regGet(IR, &k); if (k) write(terminal, "IR ", 3); else write(terminal, "   ", 3);

    close(terminal);
    
    mt_gotoXY(23, 1);
    
    return 0;
}

/* display Ip */
int displayIp()
{
    int term = open(TERM, O_RDWR);
    if (term == -1) {
        fprintf(stderr, "%s\n", "displayIp: terminal error");
        close (term);
        return -1;
    }
	
	ip = mem_ptr;
	
    char str_1[TMP_STR_LEN];

    mt_gotoXY(5, 69);
    write(term, "+0000", 5);

    if (ip > 99 || ip < 0)
        sc_regSet(M, 1);

    sprintf(str_1, "%02X", ip);
    mt_gotoXY(5, 72);

    write(term, str_1, strlen(str_1));

    sc_regSet(M, 0);

    close (term);
    mt_gotoXY(23, 1);
    
}

/* display Operation */
int displayOperation()
{
    char str_1 [TMP_STR_LEN];    
    int value, comm, oper, k;

    int term = open(TERM, O_RDWR);
    if (term == -1) {
        fprintf(stderr, "%s\n", "displayOperation: terminal error");
        close (term);
        return -1;
    }
    
    mt_gotoXY(8, 67);
    write(term, "+ 00 : 00", 5);
	
	sc_memoryGet(mem_ptr, &value);
    
    k = value >> 14 & 0x1;
    oper = value & 0x7F;
    comm = value >> 7 & 0x7F;
    
    if (k)
		sprintf(str_1, "   %04X    ", value & 0x1FFF);
    else
		sprintf(str_1, "%c %02X : %02X", !k ? '+':' ', comm, oper);
	
    k = strlen(str_1);
    mt_gotoXY(8, 67);
    write(term, str_1, k);

    close(term);
    mt_gotoXY(23, 1);
    return 0;
}

/* clear input area */
void clearInput()
{
    int term = open(TERM, O_RDWR);
    if (term == -1) {
        fprintf(stderr, "%s\n", "clearInput: terminal error");
        close (term);
        return;
    }

	int i;
    mt_gotoXY (23, 1);
    for (i = 0; i < 80; ++i)
        write (term, " ", 1);
    mt_gotoXY (23, 1);

    close (term);
}


/* display UI */
void displayUI()
{
    displayMemory();
    displayAccumulator();
    displayIp();
    displayOperation();
    displayFlags();
    displayBigChar();

    mt_gotoXY(23, 1);      
}

/* reset terminal */
void reset()
{
	 displayBorders();
     sc_memoryInit();
     sc_regInit();
     displayUI ();
}

void user_input_accum()
{
    int term = open(TERM, O_RDWR);
    if (term == -1) {
        fprintf(stderr, "%s\n", "user_input_ax: terminal error");
        close (term);
        return;
    }

    char buf[5];
    write(term, "Enter AX (hex) > ", 17);
    read(term, buf, 5);
    sscanf(buf, "%x", &accum);

    clearInput();
    close(term);
}

void user_input_ip()
{
    int term = open(TERM, O_RDWR);
    if (term == -1) {
        fprintf(stderr, "%s\n", "user_input_ip: terminal error");
        close (term);
        return;
    }

    char buf[5];
    write(term, "Enter IP (hex) > ", 17);
    read(term, buf, 5);
    sscanf(buf, "%x", &ip);
    if(ip >= 0 && ip < 100)
		mem_ptr = ip;
	else
		sc_regSet(T, 1);
    clearInput();
    close(term);
}

void user_input_mem()
{
	int term = open(TERM, O_RDWR);
    if (term == -1) {
        fprintf(stderr, "%s\n", "user_input_mem: terminal error");
        close (term);
        return;
    }
	
    char buf[5];
    int com = 0x0, oper = 0x0, value;
    sc_memoryGet(mem_ptr, &value);
    sc_commandDecode(value, &com, &oper);
    write(term, "Enter com,oper > ", 17);
    read(term, buf, 5);
    sscanf(buf, "%x%x", &com, &oper);
    sc_commandEncode(com, oper, &value);
    sc_memorySet(mem_ptr, value); 
    clearInput();
    close(term);
}