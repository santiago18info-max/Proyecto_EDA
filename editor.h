#ifndef EDITOR_H
#define EDITOR_H

// ==========================================================
// Definiciones de tipos
// ==========================================================

// Resultado de las operaciones
typedef enum {
    OK,
    ERROR,
    NO_IMPLEMENTADO
} TipoRet;

// Estructura del archivo (opaca, implementada en .c)
typedef struct archivo* Archivo;

// ==========================================================
// Prototipos de funciones del "módulo editor"
// ==========================================================

// Crear un archivo nuevo con el nombre dado
Archivo CrearArchivo(const char *nombre);

// Insertar una línea en la posición indicada
// archivo: puntero al archivo actual
// linea: contenido de la línea
// pos: posición (1 = primera línea)
TipoRet InsertarLinea(Archivo *archivo, const char *linea, unsigned int pos);

// Borrar una línea en la posición indicada
TipoRet BorrarLinea(Archivo *archivo, unsigned int pos);

// Mostrar el contenido completo del archivo
TipoRet MostrarTexto(Archivo archivo);

// Contar el número de líneas del archivo
TipoRet ContarLineas(Archivo archivo, unsigned int *cantidad);

// Borrar todo el archivo (liberar memoria)
TipoRet BorrarArchivo(Archivo *archivo);

#endif // EDITOR_H
