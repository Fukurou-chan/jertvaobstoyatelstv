#define MMEMORY_H
#ifdef MMEMORY_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int registr;


/*const int commands[] = { 	
0x10, 0x11, 0x20, 0x21, 0x30, 0x31, 0x32,
0x33, 0x40, 0x41, 0x42, 0x43, 0x51, 0x52,
0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59,
0x60, 0x61, 0x63, 0x64, 0x65, 0x66, 0x67,
0x68, 0x69, 0x70, 0x71, 0x72, 0x73, 0x74,
0x02, 0x75, 0x02, 0x76
};*/

int memory[100];

#define PC 1 // перевыполнение при выполнении операции
#define O 2 // деление на 0
#define M 3 // выход за границы памяти
#define T 4 // игнорирование тактовых импульсов
#define E 5 // неверная команда
#define IR 6 // is run

#define BC_ENTER "\E(0"// exit alt_cm
#define BC_EXIT "\E(B"
#define FIRST 1		   //overflow
#define SECOND 1 << 1  //division by 0
#define THIRD 1 << 2   //out of memory
#define FOURTH 1 << 3  //ignoring clock pulses
#define FIFTH 1 << 4    //incorrect command

int sc_memoryInit ();  /*инициализирует оперативную память Simple Computer, за-
давая всем еѐ ячейкам нулевые значения. В качестве «оперативной памяти» использу-
ется массив целых чисел, определенный статически в рамках библиотеки. Размер мас-
сива равен 100 элементам.*/
int sc_memorySet (int address, int value); /*задает значение указанной
ячейки памяти как value. Если адрес выходит за допустимые границы, то устанавлива-
ется флаг «выход за границы памяти» и работа функции прекращается с ошибкой;*/
int sc_memoryGet (int address, int *value);  /*возвращает значение указан-
ной ячейки памяти в value. Если адрес выходит за допустимые границы, то устанавли-
вается флаг «выход за границы памяти» и работа функции прекращается с ошибкой.
Значение value в этом случае не изменяется.*/
int sc_memorySave (char *filename); /*сохраняет содержимое памяти в файл в
бинарном виде (используя функцию write или fwrite);e. int sc_memoryLoad (char * filename) – загружает из указанного файла содер-
жимое оперативной памяти (используя функцию read или fread);*/
int sc_memoryLoad (char *filename);  /*загружает из указанного файла содер-
жимое оперативной памяти (используя функцию read или fread);*/
int sc_regInit ();  /*инициализирует регистр флагов нулевым значением;*/
int sc_regSet (int registr, int value); /*устанавливает значение указанно-
го регистра флагов. Для номеров регистров флагов должны использоваться маски, за-
даваемые макросами (#define). Если указан недопустимый номер регистра или некор-
ректное значение, то функция завершается с ошибкой.*/
int sc_regGet (int registr, int *value); /*возвращает значение указанного
флага. Если указан недопустимый номер регистра, то функция завершается с ошибкой.*/
int sc_commandEncode (int command, int operand, int *value); /* кодиру-
ет команду с указанным номером и операндом и помещает результат в value. Если ука-
заны неправильные значения для команды или операнда, то функция завершается с
ошибкой. В этом случае значение value не изменяется.*/
int sc_commandDecode (int value, int *command, int *operand); /*деко-
дирует значение как команду Simple Computer. Если декодирование невозможно, то
устанавливается флаг «ошибочная команда» и функция завершается с ошибкой.*/



#endif
