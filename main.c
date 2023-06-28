#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <getopt.h>
#include "file.h"

extern _Bool completion;

typedef struct {
    char *filename;

    enum {
        EASY,
        MEDIUM,
        HARD
    } level;
    
    int time_duration;
    int max_fail;
} option_t;


/*
 *  Обратный отсчёт.
 */
double vkl_timer(int duration)
{
    // 
    clock_t start = clock(); 
    while (!completion && 
            duration > (int)((clock() - start) / CLOCKS_PER_SEC)) {
        continue;
    }
    completion = 1; // значение влияет на функцию navigation (Время истекло)
    return (clock() - start) * 1.0 / CLOCKS_PER_SEC;
}
/*
 * Парсинг опций программы.
 */
int parse_args(int argc, char **argv, option_t *opt)
{
    if (argc == 1 || strcmp(argv[1], "--help") == 0)
    {
        printf("TEST TYPING\n"
                "-i <filename> (входной файл)\n"
                "-t [0 - +inf] (таймер)\n"
                "-i [0 - 2]    (уровень сложности)\n"
                "-f [0 - +inf] (макс. кол-во ошибок)\n"
                );
            
        return 1;
    }
    int r;
    while ((r = getopt(argc, argv, "f:t:i:l:")) != -1) {
        switch (r) {
            case 'i':
                opt->filename = optarg;
                break;

            case 't':
                opt->time_duration = atoi(optarg);
                break;

            case 'l':
                opt->level = atoi(optarg);
                break;

            case 'f':
                opt->max_fail = atoi(optarg);
                break;

            default: // неверная опция (getopt сообщит какой)
                return 1;
        }
    }
    // проверка корректность значений
    if (opt->time_duration <= 0  || opt->level < EASY || opt->level > HARD
        || opt->max_fail < 0)
    {
        return 1;
    }
    return 0;
}

int main(int argc, char **argv)
{
    option_t opt;
    opt.filename = "input.txt";
    opt.level = EASY;
    opt.time_duration = 30;
    opt.max_fail = 100;

    if (parse_args(argc, argv, &opt) == 1) {
        return 0;
    }
    char *text = get_content(opt.filename);
    if (text == NULL) {
        exit(EXIT_FAILURE);
    }

    // Настройка режима работы.
    initscr();   
    noecho();
    cbreak();
    if ((LINES < 24) || (COLS < 80)) { 
        endwin();
        free(text);
        fprintf(stderr, "Your terminal needs to be at least 80x24\n");
        return 2;
    }

    addstr(text); // Печать прочитанного текста
    
    move(0, 0); // Перенос указателя в начало 
    start_color(); // найстрока палитры
    init_pair(CORRECT, COLOR_WHITE, COLOR_GREEN);
    init_pair(INCORRECT, COLOR_WHITE, COLOR_RED);

    // Поток для таймера
    pthread_t navigation_thread;

    // Сбор статистики
    stat_t st;
    st.text = text;
    
    pthread_create(&navigation_thread, NULL, &navigation, &st); // запустили навигацию
    double dur = vkl_timer(opt.time_duration); // таймер

    
    // Подсчёт статистики
    double avg_sym  = st.total_sym / dur;
    double avg_word = st.total_word / dur;

    // Строка-статистики
    char stat_str[1029] = {0};
    sprintf(stat_str, "STAT\nCorrect sym: %i\nIncorrect sym: %i\nTotal sym: %i\nSPS: %.2lf\nWPS: %.2lf\n",
            st.correct_sym, st.total_sym - st.correct_sym, st.total_sym, avg_sym, avg_word);

    wclear(stdscr); // чистка консоли
    addstr(stat_str); // Вывод

    refresh();
    getch();
    endwin();

    free(text);
    exit(EXIT_SUCCESS);
}

