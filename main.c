#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <getopt.h>
#include <time.h>
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

void vkl_timer(int duration)
{
    clock_t start = clock();
    while (!completion && 
            duration > (int)((clock() - start) / CLOCKS_PER_SEC)) {
        continue;
    }
    completion = 1;
}
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
    char *filename;
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

            default:
                // МСДЖ: неизвестный аргумент %с [r]
                return 1;
        }
    }
    if (opt->time_duration <= 0 
        || opt->level < EASY || opt->level > HARD
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
        // МСДЖ: Не удалось прочитать исходный текст
        exit(EXIT_FAILURE);
    }

    initscr();   
    noecho();
    cbreak();
    if ((LINES < 24) || (COLS < 80)) {
        endwin();
        free(text);
        fprintf(stderr, "Your terminal needs to be at least 80x24");
        return 2;
    }
    addstr(text);
    
    move(0, 0); // встать в начало консоли
    start_color();
    init_pair(CORRECT, COLOR_WHITE, COLOR_GREEN);
    init_pair(INCORRECT, COLOR_WHITE, COLOR_RED);

    // Запуск
    pthread_t navigation_thread;
    pthread_create(&navigation_thread, NULL, &navigation, text);
    vkl_timer(opt.time_duration);
    
    refresh();
    getch();
    endwin();

    free(text);
    exit(EXIT_SUCCESS);
}

