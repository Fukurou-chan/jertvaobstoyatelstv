#include "alu.h"

void timer_init()
{
    nval.it_interval.tv_sec = 0;
    nval.it_interval.tv_usec = 100000;
    nval.it_value.tv_sec = 0;
    nval.it_value.tv_usec = 1000000;
}

int switchbit(const int value, const int position)
{
    return (value ^ (1 << position));
}

int positive (int num)
{
    if ((num & 65535) < 32768)
        return 0;
    return 1;
}

int getNum(int *n)
{
    char buf[256];

    printf("Enter the value: ");
    fgets(buf, 256, stdin);

    if(sscanf(buf, "%x", n) != 1)
        return -1;
    return 0;

}

int ALU(int command, int operand)
{
    switch (command)
    {
        case 0x30:
        {
            accum += (switchbit(memory[operand], 15) & 65535) & 32767;
        } break;
        case 0x31:
        {
            accum -= (switchbit(memory[operand], 150) & 65535) & 32767;
        }break;
        case 0x32:
        {
            if(memory[operand]== 32768 || ((switchbit(memory[operand], 15) & 65535) & 32767) ==0)
            {
                sc_regSet(SECOND, 1);
                return 1;
            } 
            accum /= (switchbit(memory[operand], 15) & 65535) & 32767;   
        } break;

        case 0x33:{//MUL
			accum *= (switchbit(memory[operand], 15) & 65535) & 32767;
		} break;
        case 0x54: //XOR 
        { 
            int a = 0;
            sc_accumGet(&a);
            int value = 0;
            sc_memoryGet(operand, &value);
            int b = 0,
                tmp = 0;
            sc_commandDecode(value, &tmp, &b);
            int c = 0;
            for (int i = 0; i < 8; i++)
                if ((a & (1 << i)) ^ (b & (1 << i)))
                    c |= 1 << i;
            accum = c;
            return 0;
        } break;


    }
    return 0;
}

int CU()
{
    int command, operand, t = mem_ptr;
    accum &= 65535;

    if ((memory[mem_ptr] & 32768)==1)
    {
        sc_regSet(FIFTH, 1);
        sc_regSet (FOURTH, 1);
        return 1;
    }

    int tmp = sc_commandDecode(memory[mem_ptr], &command, &operand);
	if(tmp == 1){
		sc_regSet(FIFTH, 1);
		sc_regSet(FOURTH, 1);
		return 1;	
	}	
	
	if(operand >= 100 || operand < 0){
		sc_regSet(THIRD, 1);
		sc_regSet(FOURTH, 1);
       	return 1;
	}	
	
	switch(command)
    {
		case 0x10:
        {//READ
			operation1 = command;
			operation2 = operand;
			
            int p, n = 0;
            setitimer(ITIMER_REAL, NULL, NULL);//STOP			

            printf("Enter the value: ");
			if(!scanf("%d", &n) || n > 16383 || n < 0)
            {
				sc_regSet(FIRST, 1);
				sc_regSet(FOURTH, 1);
                printf("Error!");
       			return 1;
			}				

			memory[operand] = n | 32768;
            sc_regGet(FOURTH, &p);

            if (p == 0)
                setitimer(ITIMER_REAL, &nval, &oval);//START  						
            printf(" ");
		} break;
		case 0x11:
        {//WRITE
			operation1 = command;
			operation2 = operand;
    		printf("%X", (memory[operand] & 32767));
		} break;
        case 0x20:
        {//LOAD
            operation1 = command;
            operation2 = operand;
            if (positive(memory[operand]))
                accum = (switchbit(memory[operand], 15) & 65535) & 32767;
            else
                accum = (switchbit(memory[operand], 15) & 65535) | 32768;
            
        } break;
        case 0x21:
        {//STORE
            operation1 = command;
            operation2 = operand;
            if (accum > 16384)
                memory[operand] = (switchbit(accum, 15) & 65535);
            else
                memory[operand] = (switchbit(accum, 15) & 65535) | 32768 ;
        } break;
		case 0x30:
        {//ADD
			operation1 = command;
			operation2 = operand;
			tmp = ALU(command, operand);	
			if(tmp == 1)
            {	
				sc_regSet(FOURTH, 1);
				return 1;	
			}	
		} break;
		case 0x31:
        {//SUB
			operation1 = command;
			operation2 = operand;
			tmp = ALU(command, operand);	
			
			if(tmp == 1)
            {	
				sc_regSet(FOURTH, 1);
				return 1;	
			}	
		} break;
		case 0x32:
        {//DIVIDE
			operation1 = command;
			operation2 = operand;
			tmp = ALU(command, operand);	
			
			if(tmp == 1)
            {	
				sc_regSet(FOURTH, 1);
				return 1;	
			}	
		} break;
		case 0x33:
        {//MUL
			operation1 = command;
			operation2 = operand;
			tmp = ALU(command, operand);	
			
			if(tmp == 1){	
				sc_regSet(FOURTH, 1);
				return 1;	
			}	
		} break;
		case 0x40:
        {//JUMP
			operation1 = command;
			operation2 = operand;
			mem_ptr = operand;			
		} break;
		case 0x41:
        {//JNEG
			operation1 = command;
			operation2 = operand;            
			if((accum & 65535)> 16384)
				mem_ptr = operand;
            else 
                mem_ptr++;			
		} break;
		case 0x42:
        {//JZ
			operation1 = command;
			operation2 = operand;
			if(accum == 0)
				mem_ptr = operand;
            else 
                mem_ptr++;			
		} break;
		case 0x43:
        {//HALT
			operation1 = command;
			operation2 = 0;
            sc_regSet(FOURTH, 1);
			t = -1;
		} break;
	
		case 0x55://JNS
		{	operation1 = command;
			operation2 = operand;
			if(accum > 0)
				mem_ptr = operand;			
		} break;
		
        case 0x70://RCCR
        {    operation1 = command;
            operation2 = operand;
            tmp = ALU(command, operand);
			if(tmp == 1){
				sc_regSet(FOURTH, 1);
				return 1;	
			}	
        } break;
		default:
        {
			printf("Command %02X is not supported\n", command);
			sc_regSet(FOURTH, 1);
            return 0;
		} break;
	}
	if(t == mem_ptr)	
		mem_ptr++;
    return 0;	
}

void sighandler(int sig)
{
    if (sig == SIGALRM) 
    	CU();
	
    if (sig == SIGUSR1) 
    {//user-defined signal
    	alarm(0);
    	sc_regInit();
    	sc_regSet(FOURTH, 1);
    	mem_ptr = 0;
        accum = 0;
    }
}

int sc_accumGet(int *value)
{
    *value = accum;
    return 0;
}