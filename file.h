#ifndef ONLINE_TEST_H 
#define ONLINE_TEST_H 

#include <curses.h>
// count correct = 
/*typedef struct {
    long correct;
} stat_t;
*/
#define CORRECT (1)
#define INCORRECT (2)

char* get_content(char const *inp_flnm);
void navigation(void *args);

#endif // ONLINE_TEST_H 

