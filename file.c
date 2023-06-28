#include "file.h"
#include <stdio.h>
#include <stdlib.h>


_Bool completion = 0; // завершенность программы
                      
/*
 * Чтение текста из файла.
 */
char* get_content(char const *inp_flnm)
{
    FILE *file;
    file = fopen(inp_flnm, "rt"); // открытие
    if (file == NULL) {
        fprintf(stderr, "Not found file %s\n", inp_flnm);
        return NULL;
    }

    long file_size;
    fseek(file, 0, SEEK_END); // переместить указатель файла в конец
    file_size = ftell(file); // размер файла
    if (file_size == -1) {
        fprintf(stderr, "Could not find file size\n");
        fclose(file);
        return NULL;
    }

    char *text = malloc(sizeof(char) * (file_size + 1));
    if (text == NULL) { 
        fprintf(stderr, "Memory allocation failure\n");
        fclose(file);
        return NULL;
    }
    
    // Перенос файлого-указателя в начало файла для чтения текста.
    fseek(file, 0, SEEK_SET);
    if (fread(text, sizeof(char), file_size, file) != file_size) {
        fprintf(stderr, "Unable to read content of %s\n", inp_flnm);
        free(text);
        fclose(file);
        return NULL;
    }
    text[file_size] = '\0';
    fclose(file);
    return text;
}
/*
 * Отслеживание нажатых клавиш.
 */
void
navigation(void *args)
{
    stat_t *st = args; // Статистика нажатий
    char const *text = st->text;
    int x = 0, y = 0;
    
    st->correct_sym = 0; 
    st->total_sym = 0;   
    st->total_word = 0;   

    while (!completion) {
        char sym = getch();
        st->total_sym++;

        int status; // определение цветовой палитры
        if (text[x + y] == ' ') { // В слове слова закончились.
            st->total_word++;
        }
        if (sym == text[x + y]) { // Правильно введённых символ
            st->correct_sym++;
            status = CORRECT;
        } else {
            status = INCORRECT; 
        }
        
        attron(COLOR_PAIR(status)); // определили цвет печатающего символа
        mvaddch(x, y, text[x + y]); // напечатали введённый символ
        attroff(COLOR_PAIR(status)); // отключили

        y += 1; // переход на следующий символ на экране
        x += y / COLS; // перенос на следущую строку
        y %= COLS;
        if (text[x + y] == '\0') { // Текст закончился
            break;
        }
    }
    completion   = 1; // отключаем таймер
}

