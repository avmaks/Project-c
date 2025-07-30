#ifndef S21_GREP_H_
#define S21_GREP_H_

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define SIZE 1024

typedef struct flag {
  int e;  // шаблон
  int i;  // Игнорирует различия регистра.
  int v;  // Инвертирует смысл поиска соответствий.
  int c;  // Выводит только количество совпадающих строк.
  int l;  // Выводит только совпадающие файлы.
  int n;  // Предваряет каждую строку вывода номером строки из файла ввода.
  int h;  // Выводит совпадающие строки, не предваряя их именами файлов.
  int s;  // Подавляет сообщения об ошибках о несуществующих или нечитаемых
          // файлах.
  int f;  // Получает регулярные выражения из файла.
  int o;  // Печатает только совпадающие (непустые) части совпавшей строки.
  int error;  // неправильный флаг
} flags;

typedef struct {
  char *path;
  int count_files;
  int count_pattern;  // сколько шаблонов
} values;

int parser(int argc, char *argv[], flags *flag, values *value,
           char pattern[SIZE][SIZE]);
void grep(values value, flags flag, char pattern[SIZE][SIZE]);
int find_pattern(int opt_ind, char *argv[], char patterns[SIZE][SIZE]);
void f_flag(char *path, char pattern[SIZE][SIZE], values *value);
void flag_c_l(values value, flags flag, int count_matched_lines);
void flag_o(char *start_point, regex_t preg);
void printing_strings(values value, flags flag, int count_lines, char *string);

#endif  // S21_GREP_H
