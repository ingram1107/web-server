#include "http-handler.h"

#include "http-header.h"
#include "http-request.h"
#include "stack.h"
#include <asm-generic/errno.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int parseHTTPRequest(int receiveMessageSize,
                            char receiveMessage[receiveMessageSize],
                            HTTPRequest* httpRequest)
{
  char* readBuffer = receiveMessage;
  int lineLen = 0;

  lineLen = strcspn(readBuffer, " ");
  httpRequest->method = calloc(lineLen, sizeof(char));
  strncpy(httpRequest->method, readBuffer, lineLen);
  readBuffer += lineLen + 1;
  httpRequest->method[lineLen] = '\0';

  lineLen = strcspn(readBuffer, " ");
  httpRequest->uri = calloc(lineLen, sizeof(char));
  strncpy(httpRequest->uri, readBuffer, lineLen);
  readBuffer += lineLen + 1;
  httpRequest->uri[lineLen] = '\0';

  lineLen = strcspn(readBuffer, CRLF);
  httpRequest->version = calloc(lineLen, sizeof(char));
  strncpy(httpRequest->version, readBuffer, lineLen);
  readBuffer += lineLen + 2; /* Skip through the read line and CRLF */
  httpRequest->version[lineLen] = '\0';

  Stack headers = stackInit();

  for (; readBuffer[0] != '\r'
       && readBuffer[1] != '\n'; readBuffer += lineLen + 2) {
    HTTPHeader* currentHeader = httpHeaderInit();
    lineLen = strcspn(readBuffer, ":");
    currentHeader->name = calloc(lineLen, sizeof(char));
    strncpy(currentHeader->name, readBuffer, lineLen);
    readBuffer += lineLen + 1;
    currentHeader->name[lineLen] = '\0';

    while (*readBuffer == ' ') {
      readBuffer++;
    }

    lineLen = strcspn(readBuffer, CRLF);
    currentHeader->value = calloc(lineLen, sizeof(char));
    strncpy(currentHeader->value, readBuffer, lineLen);
    currentHeader->value[lineLen] = '\0';
    stackPush(currentHeader, headers);
  }

  httpRequest->headers = headers;

  fflush(stdout);
  return 0; /* Success */

cleanup:
  perror("web-server");
  fflush(stdout);
  return -1;  /* Failure */
};

static int createHTTPResponseMessage(int httpResponseMessageSize,
                                     char httpResponseMessage[httpResponseMessageSize],
                                     int httpResponseStatus)
{
  FILE* dateCmd = popen("date -u +\"%a, %d %b %Y %T %Z\"", "r");

  if (dateCmd) {
    char date[1024] = { 0 };
    int dateStrSize = 0;

    for (int i = 0, c = fgetc(dateCmd); c != EOF; i++, c = fgetc(dateCmd)) {
      date[i] = c;
      dateStrSize++;
    }

    date[dateStrSize] = '\0';

    int returnStatus = snprintf(httpResponseMessage,
                                httpResponseMessageSize,
                                "HTTP/1.1 %s" CRLF "Server: web-server/0.0.1" CRLF "Last-Modified: %s" CRLF
                                CRLF "<h1>Hello World</h1><p>Welcome to the web-server</p>",
                                httpStatusStr[httpResponseStatus],
                                date);

    if (returnStatus < 0) {
      errno = EILSEQ;     /* Encoding Error */
      goto cleanup;
    } else if (returnStatus > httpResponseMessageSize) {
      errno = EOVERFLOW;  /* String Too Large */
      goto cleanup;
    }

    return 0; /* Success */
  }

cleanup:
  pclose(dateCmd);
  return -1;
};

int handleHTTPClientRequest(int receiveMessageSize,
                            char receiveMessage[receiveMessageSize],
                            int responseMessageSize,
                            char responseMessage[responseMessageSize])
{
  HTTPRequest* httpRequest = malloc(sizeof(struct HTTPRequest));

  int httpParseStatus = parseHTTPRequest(receiveMessageSize,
                                         receiveMessage,
                                         httpRequest);

  if (httpParseStatus == -1) {
    fprintf(stderr, "web-server: Failed to parse HTTP request header\n");
    fprintf(stderr, "web-server: Received HTTP request as follows:\n%s",
            receiveMessage);
    return -1;  /* Failure */
  }

  printf("web-server: HTTP request method: %s\n", httpRequest->method);
  printf("web-server: HTTP request uri: %s\n", httpRequest->uri);
  printf("web-server: HTTP request version: %s\n", httpRequest->version);

  for (NextStackNode currentHeader = httpRequest->headers->next;
       currentHeader != NULL; currentHeader = currentHeader->next) {
    printf("web-server: HTTP header: %s: %s\n", currentHeader->element->name,
           currentHeader->element->value);
  }

  int httpRequestMethodHandler = -1;

  for (int i = 0; i <= PATCH; i++)
    if (strcmp(httpRequest->method, httpMethodStr[i]) == 0) {
      httpRequestMethodHandler = i;
    }

  switch (httpRequestMethodHandler) {
    case GET:
      if (createHTTPResponseMessage(responseMessageSize, responseMessage,
                                    OK) == -1) {
        return -1;  /* Failure */
      }

      break;

    case HEAD:
    case POST:
    case PUT:
    case DELETE:
    case CONNECT:
    case OPTIONS:
    case TRACE:
    case PATCH:
      if (createHTTPResponseMessage(responseMessageSize, responseMessage,
                                    METHOD_NOT_ALLOWED) == -1) {
        return -1;  /* Failure */
      }

      break;

    default:
      if (createHTTPResponseMessage(responseMessageSize, responseMessage,
                                    NOT_IMPLEMENTED) == -1) {
        return -1;  /* Failure */
      }

      break;
  };

  httpRequestFree(httpRequest);

  return 0; /* Success */
};
