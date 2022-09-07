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
  char receiveBuffer[HTTP_HEADER_LEN] = { 0 };
  char serverData[HTTP_HEADER_LEN] = { 0 };
  int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
  if (serverSocket == -1) {
    errnum = errno;
    fprintf(stderr, "web-server: fail to create server socket\n");
    fprintf(stderr, "web-server: return value %d\n", errnum);
    perror("web-server");
    goto failure;
  }

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
    fprintf(stderr, "web-server: socket details shown as follow:\n");
    fprintf(stderr, "web-server:     address family: %d\n", serverAddress.sin_family);
    fprintf(stderr, "web-server:     listen port: %d\n", ntohs(serverAddress.sin_port));
    fprintf(stderr, "web-server:     internet address: %d\n", ntohs(serverAddress.sin_addr.s_addr));
    fprintf(stderr, "web-server: return value %d\n", errnum);
    perror("web-server");
    goto cleanup;
  }

  int listenStatus = listen(serverSocket, 5);
  if (listenStatus == -1) {
    errnum = errno;
    fprintf(stderr, "web-server: fail to listen for connections on server socket\n");
    fprintf(stderr, "web-server: socket details shown as follow:\n");
    fprintf(stderr, "web-server:     address family: %d\n", serverAddress.sin_family);
    fprintf(stderr, "web-server:     listen port: %d\n", ntohs(serverAddress.sin_port));
    fprintf(stderr, "web-server:     internet address: %d\n", ntohs(serverAddress.sin_addr.s_addr));
    fprintf(stderr, "web-server: return value %d\n", errnum);
    perror("web-server");
    goto cleanup;
  }

  int clientSocket;
  clientSocket = accept(serverSocket,
                        (struct sockaddr *) NULL,
                        NULL);
  if (clientSocket == -1) {
    errnum = errno;
    fprintf(stderr, "web-server: fail to bind client socket\n");
    fprintf(stderr, "web-server: return value %d\n", errnum);
    perror("web-server");
    goto cleanup;
  }

  int messageSize = read(clientSocket, receiveBuffer, HTTP_HEADER_LEN-1);
  if (messageSize < 0) {
    errnum = errno;
    fprintf(stderr, "web-server: fail to read client socket\n");
    fprintf(stderr, "web-server: return value %d\n", errnum);
    perror("web-server");
    goto cleanup;
  }
  receiveBuffer[messageSize-1] = '\0';  /* Null terminate the received string */

  char httpMethod[8] = { 0 };
  char httpPath[1024] = { 0 };
  char httpVersion[10] = { 0 };
  char* httpToken;

  /**
   * TODO: Might need to design a filter that do the same job as strtok do but
   * much resilient to buffer overflow and able to treat all space character as
   * the same.
   */
  /* Get HTTP method type */
  httpToken = strtok(receiveBuffer, " ");
  if (httpToken != NULL) {
    strncpy(httpMethod, httpToken, strlen(httpToken));
    printf("web-server: HTTP Method: %s\n", httpMethod);
  }

  /* Get HTTP request path */
  httpToken = strtok(NULL, " ");
  if (httpToken != NULL) {
    strncpy(httpPath, httpToken, strlen(httpToken));
    printf("web-server: HTTP Request Path: %s\n", httpPath);
  }

  /* Get HTTP version */
  httpToken = strtok(NULL, "\n");
  if (httpToken != NULL) {
    strncpy(httpVersion, httpToken, strlen(httpToken));
    printf("web-server: HTTP Version: %s\n", httpVersion);
  }

  fflush(stdout);

  snprintf(serverData, sizeof(serverData), "HTTP/1.1 200 OK\r\n\r\nHello");
  int responseStatus = write(clientSocket, &serverData, strlen(serverData));
  if (responseStatus == -1) {
    errnum = errno;
    fprintf(stderr, "web-server: fail to write a response to the client socket\n");
    fprintf(stderr, "web-server: return value %d\n", errnum);
    perror("web-server");
    goto cleanup;
  }
  close(serverSocket);

  return EXIT_SUCCESS;

cleanup:
  close(serverSocket);
failure:
  return EXIT_FAILURE;
}
