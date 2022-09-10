/** @file */
#include <arpa/inet.h>
#include <asm-generic/errno.h>
#include <errno.h>
#include <http-handler.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

/**
 * @brief print error and terminate the process
 *
 * Print error status of the process based on @a errno set, and then terminate
 * the process right after, with @a EXIT_FAILURE.
 *
 * @param[in] prompt Brief string that should be displayed before the error
 *                   message, typically the process name
 */
void printErrorAndExit(char* prompt) {
  perror(prompt);
  exit(EXIT_FAILURE);
}

/**
 * @brief main function
 * @return int
 */
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

    char responseMessage[HTTP_HEADER_LEN] = { 0 };
    int handler = handleHTTPClientRequest(messageSize-1, receiveBuffer, HTTP_HEADER_LEN, responseMessage);
    if (handler == -1) {
      fprintf(stderr, "web-server: Failed to handle HTTP client request\n");
      exit(EXIT_FAILURE);
    };

    snprintf(serverData, sizeof(serverData), "%s", responseMessage);
    int responseStatus = write(clientSocket, &serverData, strlen(serverData));
    if (responseStatus == -1) printErrorAndExit("web-server");

    close(clientSocket);
  }
  close(serverSocket);

  return EXIT_SUCCESS;
}
