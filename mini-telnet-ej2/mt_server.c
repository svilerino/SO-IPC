#include "mt.h"

int main(void)
{
    int s, s1, t, len;
    struct sockaddr_in local, remote;
    char str[MAX_MSG_LENGTH];
    int n;

  /* Crear un socket de tipo INET con TCP (SOCK_STREAM). */
    if ((s = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("creando socket");
        exit(1);
    }

  /* Establecer la dirección a la cual conectarse para escuchar. */
    local.sin_family = AF_INET;
    local.sin_addr.s_addr = INADDR_ANY;
    local.sin_port = htons(PORT);
    len = sizeof(local);
    if (bind(s, (struct sockaddr *)&local, len) == -1) {
        perror("haciendo bind");
        exit(1);
    }

  /* Escuchar en el socket y permitir 1 conexiones en espera. */
    if (listen(s, 1) == -1) {
        perror("escuchando");
        exit(1);
    }

  /* Aceptar una conexión entrante. */
    t = sizeof(remote);
    if ((s1 = accept(s, (struct sockaddr*) &remote, (socklen_t*) &t)) == -1) {
        perror("aceptando la conexión entrante");
        exit(1);
    }

  /* Mostrar en pantalla todo lo que recibimos. */
    dup2(s1, fileno(stdout));
    dup2(s1, fileno(stderr));
    for(;;) {

        n = recv(s1, str, MAX_MSG_LENGTH, 0);
        if (n == 0) 
            break;
        if (n < 0) { 
          perror("recibiendo");
          exit(1);
        }
        str[n] = '\0'; /* Agregar caracter de fin de cadena a lo recibido. */
        if (strcmp(str, "chau\n") == 0) break;
        printf("%s", str);

        system(str);
        // strcpy(str,"recibidirijillo amiguillo pillo\n");
        // if (send(s1, str, strlen(str), 0) == -1) {
        //   perror("enviando");
        //   exit(1);
        // }
        
    }

  /* Cerrar la conexión entrante. */
    close(s1);

  /* Cerrar la conexión que escucha. */
  close(s);
  
    return 0;
}

