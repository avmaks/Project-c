#include "s21_cat.h"

struct option long_optionss[] = {{"number-nonblank", 0, NULL, 'b'},
                                 {"number", 0, NULL, 'n'},
                                 {"squeeze-blank", 0, 0, 's'},
                                 {0, 0, 0, 0}};

int cat_parser(int argc, char *argv[], opt *options) {
  int opt;
  int index = 1;
  while (
      ((opt = getopt_long(argc, argv, "bnsETetv", long_optionss, 0)) != -1) &&
      (index == 1)) {
    switch (opt) {
      case 'b':
        options->b = 1;
        break;
      case 'n':
        options->n = 1;
        break;
      case 's':
        options->s = 1;
        break;
      case 'e':
        options->e = 1;
        options->v = 1;
        break;
      case 't':
        options->t = 1;
        options->v = 1;
        break;
      case 'T':
        options->t = 1;
        break;
      case 'E':
        options->e = 1;
        break;
      case 'v':
        options->v = 1;
        break;
      default:
        index = 0;
        break;
    }
  }
  if (index == 0) {
    fprintf(stderr, "Неправильный флаг\n");
    exit(1);
  }
  return optind;  // Возвращает каждый последующий элемент в массив
}

void cat(char *path, opt options) {
  if (access(path, F_OK) == 0) {  // Проверяет доступ к файлу
    FILE *file_name;
    file_name = fopen(path, "r");
    if (!file_name) {
      perror(path);
      exit(EXIT_FAILURE);
    }

    char now, past;
    int line = 1;     // Отслеживание строки
    int squeeze = 0;  // Сжатие пустых строк
    for (past = '\n'; (now = getc(file_name)) != EOF;
         past = now) {  // Считывание посимвольно до конца файла
      if (options.s == 1) {
        if (now == '\n' && past == '\n') {
          if (squeeze == 1) {
            continue;
          }
          squeeze++;
        } else {
          squeeze = 0;
        }
      }

      if (options.b == 1) {
        if (past == '\n' && now != '\n') {
          printf("%6d\t", line);
          line++;
        }
      }

      if (options.n == 1 && options.b == 0 && past == '\n') {
        printf("%6d\t", line);
        line++;
      }

      if (options.e == 1 && now == '\n') {
        if (options.b == 1 && past == '\n' && now == '\n') {
          printf("      \t$");
        } else {
          printf("$");
        }
      }

      if (options.t == 1 && now == '\t') {
        printf("^");
        now = 'I';
      }

      if (options.v == 1) {
        if ((now >= 0 && now <= 31) && now != '\t' && now != '\n') {
          printf("^");
          now = now + 64;
        } else if (now == 127) {
          printf("^");
          now = '?';
        }
      }
      printf("%c", now);
    }
    fclose(file_name);
  } else {
    fprintf(stderr, "Нет файла: %s\n", path);
  }
}

int main(int argc, char *argv[]) {
  opt options = {0};
  int file_index;
  file_index = cat_parser(argc, argv, &options);

  while (file_index < argc) {
    char *path;
    path = argv[file_index];
    cat(path, options);
    file_index++;
  }

  return 0;
}