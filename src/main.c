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

/**
 * TODO: implement other HTTP method: HEAD, DELETE, CONNECT, OPTIONS, TRACE,
 * PATCH
 */
const char httpGET[] = "GET";
const char httpPOST[] = "POST";
const char httpPUT[] = "PUT";

void printErrorAndExit(char* prompt) {
  perror(prompt);
  exit(EXIT_FAILURE);
}

int main(void) {
  int errnum;
  char receiveBuffer[HTTP_HEADER_LEN] = { 0 };
  char serverData[HTTP_HEADER_LEN] = { 0 };
  int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
  if (serverSocket == -1) printErrorAndExit("web-server");

  struct sockaddr_in serverAddress = {
    .sin_family = AF_INET,
    .sin_port = htons(80),          /* connect with HTTP protocol */
    .sin_addr.s_addr = INADDR_ANY,
  };

  int bindStatus = bind(serverSocket,
                        (struct sockaddr *) &serverAddress,
                        sizeof(serverAddress));
  if (bindStatus == -1) printErrorAndExit("web-server");


  int listenStatus = listen(serverSocket, 5);
  if (listenStatus == -1) printErrorAndExit("web-server");

  while (1) {
    int clientSocket;
    clientSocket = accept(serverSocket,
        (struct sockaddr *) NULL,
        NULL);
    if (clientSocket == -1) printErrorAndExit("web-server");

    int messageSize = read(clientSocket, receiveBuffer, HTTP_HEADER_LEN-1);
    if (messageSize < 0) printErrorAndExit("web-server");
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

    if (strcmp(httpMethod, httpGET) == 0) {
      if (strcmp(httpPath, "/") == 0) {
        char httpResponse[HTTP_HEADER_LEN] = "HTTP/1.1 200 OK\r\n\r\n";
        char htmlFileName[] = "../tests/index.html";
        FILE* htmlPage = fopen(htmlFileName, "r");
        if (!htmlPage) printErrorAndExit("web-server");
        char readBuffer[] = { 0 };
        fread(readBuffer, sizeof(char), 4096, htmlPage);
        strcat(httpResponse, readBuffer);

        snprintf(serverData, sizeof(serverData), "%s", httpResponse);
      } else
        snprintf(serverData, sizeof(serverData), "HTTP/1.1 404 Not Found\r\n\r\n");

      int responseStatus = write(clientSocket, &serverData, strlen(serverData));
      if (responseStatus == -1) printErrorAndExit("web-server");
    } else {
      snprintf(serverData, sizeof(serverData), "HTTP/1.1 405 Method Not Allowed\r\n\r\n");
      int responseStatus = write(clientSocket, &serverData, strlen(serverData));
      if (responseStatus == -1) printErrorAndExit("web-server");
    }
    close(clientSocket);
  }
  close(serverSocket);

  return EXIT_SUCCESS;
}
