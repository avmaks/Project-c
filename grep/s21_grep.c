#include "s21_grep.h"

int main(int argc, char *argv[]) {
  char patterns[SIZE][SIZE] = {
      0};  // двухмерный массив для хранения шаблона поиска
  int opt_ind = 0;
  flags option = {0};
  values value = {0};  // хранение текущего аргумента по структурам

  int file_location = 0;
  opt_ind = parser(argc, argv, &option, &value,
                   patterns);  // анализ аргументов командной строки

  if (argc >= 3 && option.error != 1 &&
      opt_ind != argc) {  // проверка кол-ва аргументов и нет ли ошибок
    file_location = find_pattern(opt_ind, argv, patterns);
    while (file_location < argc) {  // обработка каждого переданного файла
      if (argv[file_location + 1] != NULL) {
        value.count_files +=
            1;  // если есть ещё файл - увеличивает счетчик файлов
      }
      value.path = argv[file_location];
      grep(value, option, patterns);  // устанавливает путь к файлу
      file_location++;  // переход к следующему файлу
    }
  }
}

int find_pattern(int opt_ind, char *argv[],
                 char patterns[SIZE][SIZE]) {  // определяет начало шаблона для
                                               // поиска в массиве
  int file_location = 0;
  if (*patterns[0] == 0) {
    file_location = opt_ind + 1;
    strcpy(patterns[0], argv[optind]);  // если первый шаблон пуст, то следующий
                                        // после флагов - шаблон поиска
  } else {
    file_location = opt_ind;  // если не пуст - значит шаблоны заданы
  }
  return file_location;  // возврат в argv
}

int parser(int argc, char *argv[], flags *flag, values *value,
           char patterns[SIZE][SIZE])  // проверяет аргументы строки и применяет
                                       // соответсвующие флаги
{
  int opt = 0;
  opterr = 0;  // отключение вывода ошибок

  while ((opt = getopt_long(argc, argv, "e:ivclnhsf:o", NULL, NULL)) !=
         -1)  // перебор аргументов командной строки
  {
    switch (opt) {
      case 'e':
        while (*patterns[value->count_pattern] != 0) {
          value->count_pattern = value->count_pattern + 1;
        }
        strcpy(patterns[value->count_pattern],
               optarg);  // копирует значения параметра кс в optarg(хранит его)
        value->count_pattern += 1;
        flag->e = 1;
        break;
      case 'i':
        flag->i = 1;
        break;
      case 'v':
        flag->v = 1;
        break;
      case 'c':
        flag->c = 1;
        break;
      case 'l':
        flag->l = 1;
        break;
      case 'n':
        flag->n = 1;
        break;
      case 'h':
        flag->h = 1;
        break;
      case 's':
        flag->s = 1;
        break;
      case 'f':
        while (*patterns[value->count_pattern] != 0) {
          value->count_pattern += 1;
        }
        f_flag(optarg, patterns, value);  // обрабатывает файл с шаблонами
        flag->f = 1;
        break;
      case 'o':
        flag->o = 1;
        break;
      default:
        flag->error = 1;
        printf("Error\n");
        break;
    }
  }
  return optind;  // возврат следующего элемента массива, который не флаг
}

void grep(values value, flags flag,
          char pattern[SIZE][SIZE]) {  // поиск по шаблону в файлах переданных
                                       // через path

  FILE *filename;
  char string[SIZE] = " ";  // хранение текущей строки из файла
  int count_lines = 0;  // счетчик строк
  int count_matched_lines = 0;  // счетчик совпадающих шаблону строк

  int regular = REG_EXTENDED;  // использование регулярныых выражений

  int stop = 0;  // остановка чтения файла

  int result_regex = 243;  // хранит результат регулярного выражения
  regex_t preg;  // структура для регулярного выражения

  if (flag.v == 1 || flag.c == 1 || flag.l == 1) {
    flag.o = 0;
  }

  if (access(value.path, F_OK) == 0) {  // проверка наличия файла
    filename = fopen(value.path, "r");

    if (value.count_pattern == 0 && flag.e == 0 && flag.f == 0) {
      value.count_pattern = 1;
    }
    if (flag.i == 1) {
      regular = REG_ICASE;  // поиск шаблонов без учета регистра
    }

    while ((fgets(string, SIZE, filename)) &&
           (stop == 0)) {  // чтение файла, пока не достигнут его конец или не
                           // дана команда стоп
      int findline = 0;  // отслеживает нахождение совпадения в строке
      count_lines++;
      int pattern_no_match = 0;  // счетчик не совпадающих шаблонов

      if (strchr(string, '\n') ==
          NULL) {  // добавление символа новой строки, если его нет
        strcat(string, "\n");
      }

      for (int i = 0; i < value.count_pattern; i++) {  // цикл по шаблонам
        int match = 0;  // отслеживает, было ли найдено совпадение шаблона

        regcomp(&preg, pattern[i],
                regular | REG_NEWLINE);  // сохраняет результат регулярного
                                         // выражения в структуре
        result_regex =
            regexec(&preg, string, 0, 0,
                    0);  // проверка соответствия строки регулярному выражению

        if (result_regex == 0 && flag.v == 0) {
          match = 1;
        }

        if (result_regex == REG_NOMATCH &&
            flag.v == 1) {  // если совпадения нет, увеличивается счетчик
          pattern_no_match++;
          if (pattern_no_match == value.count_pattern) {
            match = 1;
          }
        }
        if (flag.l == 1 && match == 1 && flag.c == 0) {
          printf("%s\n", value.path);  // печатаем путь к файлу
          match = 0;
          stop = 1;
        }
        if (flag.c == 1 && match == 1) {
          count_matched_lines++;
          match = 0;
        }
        if (findline == 0 &&
            match == 1) {  // если это первое совпадение, печатаем строку
          printing_strings(value, flag, count_lines, string);
          findline++;
        }
        if (flag.o == 1) {
          flag_o(string, preg);
        }
        regfree(&preg);  // освобождение памяти
      }
    }
    if (flag.c == 1) {
      flag_c_l(value, flag, count_matched_lines);
    }
    fclose(filename);
  } else if (flag.s == 0) {
    fprintf(stderr, "Нет файла: %s\n", value.path);
  }
}

void f_flag(char *path, char pattern[SIZE][SIZE],
            values *value) {  // чтение шаблонов из файла
  FILE *filename;
  int lenght = 0;
  if (access(path, F_OK) == 0) {  // проверка существования файла
    filename = fopen(path, "r");
    while (!feof(filename)) {  // чтение до конца файла
      fgets(pattern[value->count_pattern], SIZE,
            filename);  // запись строки в массив
      lenght =
          strlen(pattern[value->count_pattern]);  // вычисление длины строки

      if (pattern[value->count_pattern][0] != '\n' &&
          pattern[value->count_pattern][lenght - 1] == '\n') {
        pattern[value->count_pattern][lenght - 1] =
            '\0';  // удаление символа новой строки в конце
      }
      value->count_pattern += 1;  // увеличение счетчика шаблонов
    }
    fclose(filename);
  } else {
    printf("Error file");
  }
}

void flag_c_l(values value, flags flag,
              int count_matched_lines) {  // вывод количества строк, соответ-их
                                          // шаблону или вывод имени файла
  if (value.count_files >= 1 && flag.h == 0) {
    printf("%s:", value.path);
  }
  if (flag.l == 0) {
    printf("%d\n", count_matched_lines);
  } else if (flag.l == 1) {
    if (count_matched_lines > 0) {
      printf("1\n");
      printf("%s\n", value.path);
    } else {
      printf("0\n");
    }
  }
}

void printing_strings(values value, flags flag, int count_lines,
                      char *string)  // вывод строк соответсвующих шаблону
{
  if (value.count_files >= 1 && flag.h == 0) {
    printf("%s:", value.path);
  }
  if (flag.n == 1) {
    printf("%d:", count_lines);
  }
  if (flag.o == 0) {
    printf("%s", string);
  }
}

void flag_o(char *string, regex_t preg) {  // печать совпадающих частей строки
  regmatch_t pmatch[1];  // массив, хранящий информ-ю о местоположении
                         // совпадения в строке
  while (regexec(&preg, string, 1, pmatch, 0) ==
         0) {  // цикл выполняется, пока есть совпадения в строке
    for (int j = 0; j < pmatch->rm_eo;
         j++) {  // проверяет посимвольно до конца совпадения rm_eo
      if (j >=
          pmatch->rm_so) {  // печатается символ строки, если текущий индекс
                            // больше или равен началу совпадения rm_so
        printf("%c", string[j]);
      }
      string[j] =
          127;  // исключает уже найденное совпадение из дальнейших поисков
    }
    printf("\n");
  }
}
