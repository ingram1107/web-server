#include <arpa/inet.h>
#include <asm-generic/errno.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define HTTP_HEADER_LEN 8192

int main(void) {
  int errnum;
  char *receiveBuffer = calloc(HTTP_HEADER_LEN, sizeof(char));
  char *serverData = calloc(HTTP_HEADER_LEN, sizeof(char));
  int serverSocket = socket(AF_INET, SOCK_STREAM, 0); // TODO: err handling

  struct sockaddr_in serverAddress = {
    .sin_family = AF_INET,
    .sin_port = htons(80),          /* connect with HTTP protocol */
    .sin_addr.s_addr = INADDR_ANY,
  };

  int bindStatus = bind(serverSocket,
                        (struct sockaddr *) &serverAddress,
                        sizeof(serverAddress));
  if (bindStatus == -1) {
    errnum = errno;
    fprintf(stderr, "web-server: fail to bind server socket\n");
    fprintf(stderr, "web-server: return value %d\n", errnum);
    perror("web-server");
    goto cleanup;
  }

  listen(serverSocket, 5);  // TODO: err handling

  int clientSocket;
  clientSocket = accept(serverSocket,
                        (struct sockaddr *) NULL,
                        (unsigned int *) NULL);
  if (clientSocket == -1) {
    errnum = errno;
    fprintf(stderr, "web-server: fail to bind client socket\n");
    fprintf(stderr, "web-server: return value %d\n", errnum);
    perror("web-server");
    goto cleanup;
  }

  memset(receiveBuffer, 0, HTTP_HEADER_LEN);  /* Null terminate the received string */
  read(clientSocket, receiveBuffer, HTTP_HEADER_LEN-1);
  /**
   * TODO: the web browser is unable to display "Hello", tested in Chromium and
   * Firefox
   */
  snprintf(serverData, sizeof(serverData), "HTTP/1.1 200 OK\r\n\r\nHello");
  write(clientSocket, &serverData, strlen(serverData));
  close(serverSocket);

  free(serverData);
  return EXIT_SUCCESS;

cleanup:
  free(serverData);
  return EXIT_FAILURE;
}
