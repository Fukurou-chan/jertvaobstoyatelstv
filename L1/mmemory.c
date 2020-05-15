#include "mmemory.h"
#define N 100

int sc_memoryInit ()
{
	int *memory = calloc(N, sizeof(int));
	return *memory;
}

int sc_memorySet (int address, int value)
{
    if (address >= 0 && address < 100) {
        memory[address] = value;
        return 0;
    }
    else {
         sc_regSet(M, 1);
        return -1;
    }
}

int sc_memoryGet (int address, int *value)
{
    if (address >= 0 && address < 100) {
        *value = memory[address];
        return 0;
    }
    else {
         sc_regSet(M, 1);
        return -1;
    }
}
int sc_memorySave (char * filename)
{
	FILE *saveData;
	int result;
	
	saveData = fopen(filename, "wb");
	if (NULL == saveData)
		return 1;
	result = fwrite(memory, sizeof(int) * N, 1, saveData);
	fclose(saveData);
	if (result != 1)
		return 2;
	else
		return 0;	

}


int sc_memoryLoad(char *filename)
{
	FILE *saveData;
	int res;
	
	if (saveData == NULL)
		return 1;
	res = fread(memory, sizeof(int) * N, 1, saveData);
	for (int i = 0; i < N; ++i)
		memory[i] &= 0x7FFF;
	fclose(saveData);
	if (res != 1)
		return 2;
	else
		return 0;
}



int sc_regInit()
{
    registr = 0;
	return 0;
}



int sc_regSet (int registr, int value) 
{
	if ((registr>0)&&(registr<9))
	{
		if (value == 1)
		{//если требуется установить значение флага в единицу:
			registr |= (1<<(registr-1));
			return registr;
		}
		if (value == 0)
    	{//если требуется установить значение в нуль:
			registr &= ~registr;
			return registr;
		}
	}
   else return 1;
}

int sc_regGet(int register_flag, int *value)
{
    if (register_flag >= 0 && register_flag < 32) {
        *value = (registr >> (register_flag - 1)) & 0x1;
        return *value;
    }
    else {
        printf("Invalid register number\n");
        return -1;
    }
}

int sc_commandEncode(int command, int operand, int *value)
{
	if(value == NULL)
		return(1);
	(*value) = ((command & 0x7F) << 7) | (operand & 0x7F);
	return(0);
}

int sc_commandDecode(int value, int *command, int *operand)
{
	if((value < 0)||(value >> 14))
		return(1);
	if(command == NULL) return(2);
	if(operand == NULL) return(3);
	(*operand) = value & 0x7F;
	(*command) = (value & 0x7F80) >> 7;
	return(0);
}



	





