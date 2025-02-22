#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h> // Agregado para write() y close()

void usage(char *prog_name, char *filename) {
    printf("Usage: %s <data to add to %s>\n", prog_name, filename);
    exit(0);
}

void fatal(char *);           // Función para errores fatales
void *ec_malloc(unsigned int); // malloc() con verificación de error

int main(int argc, char *argv[]) {
    int fd; // Descriptor de archivo
    char *buffer, *datafile;

    buffer = (char *) ec_malloc(100);
    datafile = (char *) ec_malloc(20);
    strcpy(datafile, "/tmp/notes");

    if (argc < 2) {
        usage(argv[0], datafile); // Mostrar mensaje de uso y salir
    }

    strncpy(buffer, argv[1], 99); // Copia segura del argumento
    buffer[99] = '\0'; // Asegurar que el buffer termina en '\0'

    printf("[DEBUG] buffer @ %p: '%s'\n", (void *)buffer, buffer);
    printf("[DEBUG] datafile @ %p: '%s'\n", (void *)datafile, datafile);

    strcat(buffer, "\n"); // Agregar nueva línea de forma segura

    // Abrir el archivo con permisos de lectura y escritura solo para el usuario
    fd = open(datafile, O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR);
    if (fd == -1)
        fatal("in main() while opening file");

    printf("[DEBUG] file descriptor is %d\n", fd);

    // Escribir datos en el archivo
    if (write(fd, buffer, strlen(buffer)) == -1)
        fatal("in main() while writing buffer to file");

    // Cerrar archivo
    if (close(fd) == -1)
        fatal("in main() while closing file");

    printf("Note has been saved.\n");

    free(buffer);
    free(datafile);
    return 0;
}

// Función para manejar errores fatales
void fatal(char *message) {
    char error_message[100];
    strcpy(error_message, "[!!] Fatal Error ");
    strncat(error_message, message, 83);
    perror(error_message);
    exit(-1);
}

// malloc() con verificación de error
void *ec_malloc(unsigned int size) {
    void *ptr = malloc(size);
    if (ptr == NULL)
        fatal("in ec_malloc() on memory allocation");
    return ptr;
}
