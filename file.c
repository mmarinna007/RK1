#include "file.h"
#include <stdio.h>
#include <stdlib.h>


_Bool completion = 0; // завершенность программы
char* get_content(char const *inp_flnm)
{
    FILE *file;
    file = fopen(inp_flnm, "rt");
    if (file == NULL) {
        // МСДЖ: Не найден файл с таким именем
        return NULL;
    }

    long file_size;
    fseek(file, 0, SEEK_END);
    file_size = ftell(file);
    if (file_size == -1) {
        // МСДЖ: Не удалось узнать размер файла
        fclose(file);
        return NULL;
    }

    char *text;
    text = malloc(sizeof(char) * (file_size + 1));
    if (text == NULL){ 
        // МСДЖ: не удалось выделить %d память на куче [file_size]
        fclose(file);
        return NULL;
    }
    
    fseek(file, 0, SEEK_SET);
    // ToDo: Улучшить способ взятие содержимого текста
    long i;
    for (i = 0; i < file_size; i++) {
        text[i] = fgetc(file);
    }
    text[i] = '\0';
    fclose(file);
    return text;
}
//stat_t
void
navigation(void *args)
{
    char const *text = args;
    int x = 0, y = 0;
    size_t correct = 0;
    while (!completion) {
        char sym = getch();
        
        int status; //= (sym == text[x + y]) ? CORRECT : INCORRECT;
        if (sym == text[x + y]) {
            correct++;
            status = CORRECT;
        } else {
            status = INCORRECT;
        }
        
        attron(COLOR_PAIR(status));
        mvaddch(x, y, text[x + y]);
        attroff(COLOR_PAIR(status));

        y += 1;
        x += y / COLS;
        y %= COLS;
        if (text[x + y] == '\0') {
            break;
        }
    }
    completion = 1;
}

