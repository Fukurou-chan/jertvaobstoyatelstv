#include "readKey.h"


/*
Функции termios описывают общий терминальный интерфейс, гарантирующий управление асинхронным коммуникационным портом.
Большинство функций, описанных здесь, используют параметр termios_p, который указывает на структуру termios. Данная структура содержит, как минимум, следующие элементы:
tcflag_t c_iflag;      / режимы ввода /
tcflag_t c_oflag;      / режимы вывода /
tcflag_t c_cflag;      / режимы управления /
tcflag_t c_lflag;      / режимы локали /
cc_t c_cc[NCCS];       / управляющие символы /
c_iflag - флаги констант:

ICANON
запустить канонический режим. Это означает, что линии используют специальные символы: EOF, EOL, EOL2, ERASE, KILL, LNEXT, REPRINT, STATUS и WERASE, а также строчную буферизацию.

*/
int rk_readkey(int *key)
{
	struct termios orig_options;
	char buf[32];
	int num_read;
	
    /*STDIN_FILENO это файловый дескриптор (файл, сокет, пайп и т.д.; тип int). Используется не буферизованный ввод/вывод, а прямой вызов системных функций. Заголовок <unistd.h>.
Функции: read, write, select и т.д.*/
	if (tcgetattr(STDIN_FILENO, &orig_options) != 0) //int tcgetattr(int __fd, struct termios *__termios_p) Put the state of FD into *TERMIOS_P.
		return -1;
	if (rk_mytermregime(0, 0, 1, 0, 1) != 0)
		return -1;
	num_read = read(STDIN_FILENO, buf, 15); //#define STDIN_FILENO 0
	if (num_read < 0)
		return -1;
	buf[num_read] = '\0';
	if (strcmp(buf, "l") == 0)
		*key = KEY_l;
	else
		if (strcmp(buf, "s") == 0)
			*key = KEY_s;
		else
			if (strcmp(buf, "r") == 0)
				*key = KEY_r;
			else
				if (strcmp(buf, "t") == 0)
					*key = KEY_t;
				else
					if (strcmp(buf, "i") == 0)
						*key = KEY_i;
					else
						if (strcmp(buf, "e") == 0)
							*key = KEY_e;
						else
							if (strcmp(buf, "q") == 0)
							*key = KEY_q;
						else
							if (strcmp(buf, "\n") == 0)
								*key = KEY_enter;
							else
								if (strcmp(buf, "\033[15~") == 0)
									*key = KEY_f5;
								else
									if (strcmp(buf, "\033[17~") == 0)
										*key = KEY_f6;
									else
										if (strcmp(buf, "\033[A") == 0)
											*key = KEY_up;
										else
											if (strcmp(buf, "\033[B") == 0)
												*key = KEY_down;
											else
												if (strcmp(buf, "\033[C") == 0)
													*key = KEY_right;
												else
													if (strcmp(buf, "\033[D") == 0)
														*key = KEY_left;
													else
														*key = KEY_other;
	if (tcsetattr(STDIN_FILENO, TCSANOW, &orig_options) != 0)
		return -1;

	return 0;
}

int rk_mytermsave()
{
	struct termios options;
	FILE *save;
	
	if (tcgetattr(STDIN_FILENO, &options) != 0)
		return -1;
	if ((save = fopen("termsettings", "wb")) == NULL)
		return -1;
	fwrite(&options, sizeof(options), 1, save);
	fclose(save);
	
	return 0;
}

int rk_mytermrestore()
{
	struct termios options;
	FILE *save;

	if ((save = fopen("termsettings", "rb")) == NULL)
		return -1;
	if (fread(&options, sizeof(options), 1, save) > 0)
	{
        if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &options) != 0)
			return -1;
    }	
	else
		return -1;

	return 0;
}

int rk_mytermregime(int regime, int vtime, int vmin, int echo, int sigint)
{
	struct termios options;
	
	if (tcgetattr(STDIN_FILENO, &options) != 0)
		return -1;
	if (regime == 1)
		options.c_lflag |= ICANON; //Canonical input (erase and kill processing). #define ICANON 0000002
	else
	if (regime == 0)
		options.c_lflag &= ~ICANON; 
	else
		return -1;
	if (regime == 0)
	{
		options.c_cc[VTIME] = vtime;
		options.c_cc[VMIN] = vmin;
		if (echo == 1)
			options.c_lflag |= ECHO; //#define ECHO 0000010 Enable echo.
		else
			if (echo == 0)
				options.c_lflag &= ~ECHO;
			else
				return -1;
		if (sigint == 1)
			options.c_lflag |= ISIG; //#define ISIG 0000001
		else
			if (sigint == 0)
				options.c_lflag &= ~ISIG;
			else
				return -1;
	}
	if (tcsetattr(STDIN_FILENO, TCSANOW, &options) != 0)
		return -1;
	
	return 0;
}
