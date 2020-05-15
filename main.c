#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include "print.h"


int main()
{
    signal (SIGALRM, sighandler);
    signal (SIGUSR1, reset);

    sc_regInit();
    timer_init();	
    accum = 0x0;
    ip = 0x0;

    enum keys key = KEY_other;
	
	displayBorders();
    displayUI();
    
    f_key = 0;
    f_ign = 0;
    
    while (key != KEY_q) {
		
		rk_readkey(&key);
		
		if (key == KEY_t) {
			sc_regSet(IR, 0);
			alarm(0); 
			f_key = 0;
			CU();
		}
		
		if (!f_key) {
			if (key == KEY_r)  {
				sc_regSet(IR, 1);
				sc_regSet(T, 0);
			}
			
			sc_regGet(IR, &f_key);
			
			if (f_key) {
				timer_init();        
			}
			
			if (key == KEY_f5   ) user_input_accum();
			if (key == KEY_f6) user_input_ip();
			if (key == KEY_right) if (mem_ptr < 99) ++mem_ptr;
			if (key == KEY_left) if (mem_ptr >  0) --mem_ptr;
			if (key == KEY_up) if (mem_ptr - 10 >=  0) mem_ptr -= 10;
			if (key == KEY_down) if (mem_ptr + 10 < 100) mem_ptr += 10;
			if (key == KEY_left) sc_memoryLoad("memory.o");
			if (key == KEY_s) sc_memorySave("memory.o");
			if (key == KEY_e) user_input_mem(); 
			if (key == KEY_i) 
			{
				displayBorders();
				displayUI();
				sc_memoryInit();
				sc_regInit();
			}
			displayUI ();
		}
		
	}
    
	return 0;
}
