#ifndef ONLINE_TEST_H 
#define ONLINE_TEST_H 

#include <curses.h>

typedef struct {
    int  total_sym;
    int  correct_sym;
    int  total_word;
    char *text;
} stat_t;

#define CORRECT (1)
#define INCORRECT (2)

char* get_content(char const *inp_flnm);
void navigation(void *args);

#endif // ONLINE_TEST_H 

