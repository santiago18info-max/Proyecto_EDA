#include "editor.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static Archivo archivo_actual = NULL;

void mostrar_ayuda() {
  printf("\n=== MENÚ DE COMANDOS DEL EDITOR DE TEXTO ===\n");
  printf("Comandos disponibles:\n");
  printf("  crear <nombre>           - Crear un nuevo archivo\n");
  printf(
      "  insertar <linea> <pos>   - Insertar línea en posición (alias: i)\n");
  printf("  borrar <pos>             - Borrar línea en posición (alias: b)\n");
  printf("  mostrar                  - Mostrar contenido del archivo (alias: "
         "m)\n");
  printf("  contar                   - Contar número de líneas (alias: c)\n");
  printf("  borrar_archivo           - Borrar el archivo actual\n");
  printf("  ayuda                    - Mostrar esta ayuda (alias: h)\n");
  printf("  salir                    - Salir del programa (alias: q)\n");
  printf("\nEjemplos:\n");
  printf("  crear curriculum.txt\n");
  printf("  i \"Nombre: Juan Pérez\" 1\n");
  printf("  i \"Dirección: Rivera 1234\" 2\n");
  printf("  m\n");
  printf("  c\n");
  printf("  b 2\n");
  printf("  c\n");
  printf("\nPara pruebas automatizadas:\n");
  printf("  ./menu_editor < archivo_comandos.txt\n");
  printf("  echo 'comando' | ./menu_editor\n");
  printf("==============================================\n\n");
}

void procesar_crear(char *args) {
  if (archivo_actual != NULL) {
    printf(
        "ERROR: Ya existe un archivo abierto. Use 'borrar_archivo' primero.\n");
    return;
  }

  char *nombre = strtok(args, " \t");
  if (nombre == NULL) {
    printf("ERROR: Debe especificar un nombre de archivo.\n");
    return;
  }

  archivo_actual = CrearArchivo(nombre);
  printf("Archivo '%s' creado exitosamente.\n", nombre);
}

void procesar_insertar(char *args) {
  if (archivo_actual == NULL) {
    printf("ERROR: No hay archivo abierto. Use 'crear' primero.\n");
    return;
  }

  char *linea_start = strchr(args, '"');
  if (linea_start == NULL) {
    printf("ERROR: La línea debe estar entre comillas.\n");
    return;
  }
  linea_start++;

  char *linea_end = strchr(linea_start, '"');
  if (linea_end == NULL) {
    printf("ERROR: Falta la comilla de cierre.\n");
    return;
  }

  size_t linea_len = linea_end - linea_start;
  char *linea = malloc(linea_len + 1);
  strncpy(linea, linea_start, linea_len);
  linea[linea_len] = '\0';

  char *pos_str = linea_end + 1;
  while (*pos_str == ' ' || *pos_str == '\t')
    pos_str++;

  if (*pos_str == '\0') {
    printf("ERROR: Debe especificar la posición.\n");
    free(linea);
    return;
  }

  unsigned int pos = atoi(pos_str);
  if (pos == 0) {
    printf("ERROR: La posición debe ser un número válido mayor que 0.\n");
    free(linea);
    return;
  }

  TipoRet resultado = InsertarLinea(&archivo_actual, linea, pos);

  if (resultado == OK) {
    printf("Línea insertada en posición %u.\n", pos);
  } else if (resultado == ERROR) {
    printf("ERROR: No se pudo insertar la línea en posición %u.\n", pos);
  } else {
    printf("ERROR: Función InsertarLinea no implementada.\n");
  }

  free(linea);
}

void procesar_borrar(char *args) {
  if (archivo_actual == NULL) {
    printf("ERROR: No hay archivo abierto. Use 'crear' primero.\n");
    return;
  }

  char *pos_str = strtok(args, " \t");
  if (pos_str == NULL) {
    printf("ERROR: Debe especificar la posición.\n");
    return;
  }

  unsigned int pos = atoi(pos_str);
  if (pos == 0) {
    printf("ERROR: La posición debe ser un número válido mayor que 0.\n");
    return;
  }

  TipoRet resultado = BorrarLinea(&archivo_actual, pos);

  if (resultado == OK) {
    printf("Línea en posición %u borrada.\n", pos);
  } else if (resultado == ERROR) {
    printf("ERROR: No se pudo borrar la línea en posición %u.\n", pos);
  } else {
    printf("ERROR: Función BorrarLinea no implementada.\n");
  }
}

void procesar_mostrar() {
  if (archivo_actual == NULL) {
    printf("ERROR: No hay archivo abierto. Use 'crear' primero.\n");
    return;
  }

  TipoRet resultado = MostrarTexto(archivo_actual);

  if (resultado != OK) {
    printf("ERROR: No se pudo mostrar el texto.\n");
  }
}

void procesar_contar() {
  if (archivo_actual == NULL) {
    printf("ERROR: No hay archivo abierto. Use 'crear' primero.\n");
    return;
  }

  unsigned int cantidad = 0;
  TipoRet resultado = ContarLineas(archivo_actual, &cantidad);

  if (resultado == OK) {
    printf("El archivo tiene %u líneas.\n", cantidad);
  } else if (resultado == ERROR) {
    printf("ERROR: No se pudo contar las líneas.\n");
  } else {
    printf("ERROR: Función ContarLineas no implementada.\n");
  }
}

void procesar_borrar_archivo() {
  if (archivo_actual == NULL) {
    printf("ERROR: No hay archivo abierto.\n");
    return;
  }

  TipoRet resultado = BorrarArchivo(&archivo_actual);

  if (resultado == OK) {
    printf("Archivo borrado exitosamente.\n");
  } else {
    printf("ERROR: No se pudo borrar el archivo.\n");
  }
}

void procesar_comando(char *linea) {
  while (*linea == ' ' || *linea == '\t')
    linea++;
  char *fin = linea + strlen(linea) - 1;
  while (fin > linea &&
         (*fin == ' ' || *fin == '\t' || *fin == '\n' || *fin == '\r')) {
    *fin = '\0';
    fin--;
  }

  if (*linea == '\0')
    return;

  char comando[256];
  strcpy(comando, linea);
  for (int i = 0; comando[i]; i++) {
    if (comando[i] == ' ')
      break;
    comando[i] = tolower(comando[i]);
  }

  char *args = strchr(linea, ' ');
  if (args != NULL) {
    args++;
  } else {
    args = "";
  }

  if (strncmp(comando, "crear", 5) == 0) {
    procesar_crear(args);
  } else if (strncmp(comando, "insertar", 8) == 0 ||
             strncmp(comando, "i", 1) == 0) {
    procesar_insertar(args);
  } else if (strncmp(comando, "borrar_archivo", 14) == 0) {
    procesar_borrar_archivo();
  } else if (strncmp(comando, "borrar", 6) == 0 ||
             strncmp(comando, "b", 1) == 0) {
    procesar_borrar(args);
  } else if (strncmp(comando, "mostrar", 7) == 0 ||
             strncmp(comando, "m", 1) == 0) {
    procesar_mostrar();
  } else if (strncmp(comando, "contar", 6) == 0 ||
             strncmp(comando, "c", 1) == 0) {
    procesar_contar();
  } else if (strncmp(comando, "ayuda", 5) == 0 ||
             strncmp(comando, "help", 4) == 0 ||
             strncmp(comando, "h", 1) == 0) {
    mostrar_ayuda();
  } else if (strncmp(comando, "salir", 5) == 0 ||
             strncmp(comando, "exit", 4) == 0 ||
             strncmp(comando, "quit", 4) == 0 ||
             strncmp(comando, "q", 1) == 0) {
    if (archivo_actual != NULL) {
      BorrarArchivo(&archivo_actual);
    }
    exit(0);
  } else {
    printf("ERROR: Comando desconocido '%s'. Use 'ayuda' para ver comandos "
           "disponibles.\n",
           comando);
  }
}

int main(void) {
  printf("=== EDITOR DE TEXTO SIMPLE ===\n");
  printf("Para ver comandos disponibles, escriba 'ayuda'\n\n");

  int es_batch = !isatty(STDIN_FILENO);

  if (es_batch) {
    printf("Modo batch detectado - ejecutando comandos desde entrada "
           "estándar\n\n");

    char linea[1024];
    int num_linea = 0;
    while (fgets(linea, sizeof(linea), stdin)) {
      num_linea++;
      procesar_comando(linea);
    }

    return 0;
  }

  char linea[1024];
  printf("Modo interactivo. Escriba comandos (o 'ayuda' para ver opciones):\n");

  while (1) {
    printf("> ");
    if (fgets(linea, sizeof(linea), stdin) == NULL) {
      printf("\nSaliendo...\n");
      break;
    }

    procesar_comando(linea);
  }

  if (archivo_actual != NULL) {
    BorrarArchivo(&archivo_actual);
  }

  return 0;
}
