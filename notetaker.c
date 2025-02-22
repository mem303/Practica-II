#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>  // Para write(), close()

// Función para manejo de errores
void fatal(char *message) {
    perror(message);
    exit(1);
}

// Función de malloc con verificación de error
void *ec_malloc(unsigned int size) {
    void *ptr = malloc(size);
    if (ptr == NULL)
        fatal("Error en ec_malloc() al asignar memoria");
    return ptr;
}

// Función para mostrar el uso del programa
void usage(char *prog_name, char *filename) {
    printf("Uso: %s <dato a agregar en %s>\n", prog_name, filename);
    exit(0);
}

int main(int argc, char *argv[]) {
    int fd;
    char *buffer, *datafile;

    buffer = (char *) ec_malloc(100);
    datafile = (char *) ec_malloc(20);
    strcpy(datafile, "/var/notes");

    if (argc < 2) {  // Si no hay argumentos, mostrar uso y salir
        usage(argv[0], datafile);
    }

    strcpy(buffer, argv[1]);  // Copiar el argumento en buffer

    // Depuración
    printf("[DEBUG] buffer @ %p: '%s'\n", (void *)buffer, buffer);
    printf("[DEBUG] datafile @ %p: '%s'\n", (void *)datafile, datafile);

    // Abrir el archivo con permisos adecuados
    fd = open(datafile, O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR);
    if (fd == -1)
        fatal("Error al abrir el archivo");

    // Escribir en el archivo
    if (write(fd, buffer, strlen(buffer)) == -1)
        fatal("Error al escribir en el archivo");

    if (write(fd, "\n", 1) == -1)  // Agregar salto de línea
        fatal("Error al escribir salto de línea");

    // Cerrar el archivo
    if (close(fd) == -1)
        fatal("Error al cerrar el archivo");

    printf("Nota guardada correctamente.\n");
    free(buffer);
    free(datafile);
    return 0;
}
