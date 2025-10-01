#ifndef EDITOR_H
#define EDITOR_H

#include <stdbool.h>

// Estructura para representar una línea de texto
typedef struct Line {
    char *text;
    struct Line *next;
    struct Line *prev;
} Line;
/* Prototipo para la estructura de línea y funciones auxiliares */

// Crea una nueva línea con el texto dado
Line* line_create(const char *text);

// Libera la memoria de una línea
void line_destroy(Line *line);
// Estructura para el editor
typedef struct Editor {
    Line *head;
    Line *tail;
    Line *current;
    int num_lines;
} Editor;


// Funciones del editor
Editor* editor_create();
void editor_destroy(Editor *editor);

bool editor_insert_line(Editor *editor, const char *text, int position);
bool editor_delete_line(Editor *editor, int position);
const char* editor_get_line(Editor *editor, int position);
void editor_print(Editor *editor);

#endif // EDITOR_H