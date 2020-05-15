#include "bigChar.h"

int bc_printA(char *str)
{
	printf("\E(0%s\E(B", str);
	return 0;
}

int bc_box(int x1, int y1, int x2, int y2)
{
	int tmp;
	int maxX, maxY;
	int i, j;
	
	if (x1 > x2) 
    {
		tmp = x1;
		x1 = x2;
		x2 = tmp;
	}

	if (y1 > y2) 
    {
		tmp = y1;
		y1 = y2;
		y2 = tmp;
	}

	mt_getscreensize(&maxY, &maxX);
	if ((x1 < 0) || (y1 < 0) || (x2 > maxX) || (y2 > maxY) || (x2 - x1 < 2) || (y2 - y1 < 2))
		return -1;

	mt_gotoXY(x1, y1);
	bc_printA(TL);

	for (i = x1 + 1; i < x2; i++)
		bc_printA(HOR);

	bc_printA(TR);

	for (i = y1 + 1; i < y2; i++)
    {
		mt_gotoXY(x1, i);
		bc_printA(VERT);
		mt_gotoXY(x2, i);
		bc_printA(VERT);
	}

	mt_gotoXY(x1, y2);
	bc_printA(BL);

	for (i = x1 + 1; i < x2; i++)
		bc_printA(HOR);

	bc_printA(BR);
	return 0;
}

int bc_getbigcharpos(int *big, int x, int y, int *value)
{
	int position;

	if ((x < 0) || (y < 0) || (x > 7) || (y > 7))
		return -1;
	if (y <= 3)
		position = 0;
	else
		position = 1;
	y = y % 4;
	*value = (big[position] >> (y*8 + x)) & 1;

	return 0;
}

   int bc_bigcharwrite(int fd, int *big, int count)
{
	int erro = write(fd, &count, sizeof(count));
	if (erro == -1)
		return -1;
	erro = write(fd, big, count * (sizeof(int)) * 2);
	if (erro == -1)
		return -1;
	
	return 0;
}

int bc_bigcharread(int fd, int *big, int need_count, int *count)
{
	int n, readChar, erro;
	
	erro = read(fd, &n, sizeof(n));
	if (erro == -1 || (erro != sizeof(n)))
		return -1;
	readChar = read(fd, big, need_count * sizeof(int) * 2);
	if (readChar == -1)
		return -1;
	*count = readChar / (sizeof(int) * 2);
	
	return 0;
}

int bc_printbigchar(int *big, int x, int y, enum colors fg, enum colors bg)
{
	int maxX, maxY;
	int position, bit;
	int i, j;
	char row[9];
	
	mt_getscreensize(&maxY, &maxX);
	if ((x < 0) || (y < 0) || (x + 8 > maxX) || (y + 8 > maxY)) 
		return -1;

	row[8] = '\0';
	mt_setfgcolor(fg);
	mt_setbgcolor(bg);

	for (i = 0; i < 8; i++) 
	{
        for (j = 0; j < 8; j++) 
        {
			position = i >> 2;
			bit = (big[position] >> ((i % 4) * 8 + j)) & 1;
			if (bit == 0)
				row[j] = ' ';
			else
				row[j] = REC;
		}
		mt_gotoXY(x, y + i);
		bc_printA(row);
	
    }
	mt_setfgcolor(white);
	mt_setbgcolor(pink);
	return 0;
    
}
int bc_setbigcharpos(int *big, int value)
{
    int term = open(TERM, O_RDWR);

    if (term == -1) {
        fprintf(stderr, "%s\n", "bc_setbigcharpos: terminal error!");
        close(term);
        return -1;
    }

    if (!big) {
        fprintf(stderr, "%s\n", "bc_setbigcharpos: not work!");
        close(term);
        return -1;
    }

    switch (value) {
        case '+':
            big[0] = 0xFF181800;
            big[1] = 0x001818FF;
            break;
        case '0':
            big[0] = 0x818181FF;
            big[1] = 0xFF818181;
            break;
        case '1':
            big[0] = 0x120A0602;
            big[1] = 0x2020202;
            break;
        case '2':
            big[0] = 16843263U;
            big[1] = 4286611711U;
            break;
        case '3':
            big[0] = 4278256127U;
            big[1] = 4278255873U;
            break;
        case '4':
            big[0] = 4286677377U;
            big[1] = 16843009;
            break;
        case '5':
            big[0] = 2155905279U;
            big[1] = 4278256127U;
            break;
        case '6':
            big[0] = 2155905279U;
            big[1] = 4286677503U;
            break;
        case '7':
            big[0] = 134480639;
            big[1] = 538976272;
            break;
        case '8':
            big[0] = 4286677503U;
            big[1] = 4286677377U;
            break;
        case '9':
            big[0] = 4286677503U;
            big[1] = 4278255873U;
            break;
        case 'A':
            big[0] = 2172748158U;
            big[1] = 2172748287U;
            break;
        case 'B':
            big[0] = 4269900286U;
            big[1] = 4269900286U;
            break;
        case 'C':
            big[0] = 2155905279U;
            big[1] = 4286611584U;
            break;
        case 'D':
            big[0] = 2172748286U;
            big[1] = 4269900161U;
            break;
        case 'E':
            big[0] = 4286611711U;
            big[1] = 4286611711U;
            break;
        case 'F':
            big[0] = 4286611711U;
            big[1] = 2155905152U;
            break;
        default:
            big[0] = 0;
            big[1] = 0;
            break;
    }
    close(term);
    return 0;
}


