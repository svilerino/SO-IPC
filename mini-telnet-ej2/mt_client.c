#include "mt.h"

int main(int argc, char** argv)
{
  int s, len, n;
  struct sockaddr_in remote;
  char str[MAX_MSG_LENGTH];

  if (argc != 2){
    printf("Modo de uso: %s <ip_server>\n", argv[0]);
    printf("Ejemplo:");
    printf("\t%s 127.0.0.1\n", argv[0]);
    exit(1);
  }

  if ((s = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
      perror("creando socket");
      exit(1);
  }

  /* Establecer la dirección a la cual conectarse. */
  remote.sin_family = AF_INET;
  remote.sin_port = htons(PORT);
  if (inet_aton(argv[1], (struct in_addr *)&remote.sin_addr.s_addr) == 0)
    perror("defininendo ip");
  len = sizeof(remote);

  /* Conectarse. */
  if (connect(s, (struct sockaddr *)&remote, len) == -1) {
      perror("conectandose");
      exit(1);
  }

  /* Establecer la dirección a la cual conectarse para escuchar. */
  while(printf("> "), fgets(str, MAX_MSG_LENGTH, stdin), !feof(stdin)) {
    
    if (strcmp(str, "chau\n") == 0) break;
    
    if (send(s, str, strlen(str), 0) == -1) {
        perror("enviando");
        exit(1);
    }
      
    if ((n = recv(s, str, MAX_MSG_LENGTH, 0)) == -1) {
      printf("Recibiendo errno: %i\n", errno);
      exit(1);
    }

    str[n] = '\0';
    printf("%s", str);
  }

  /* Cerrar el socket. */
  close(s);

  return 0;
}



