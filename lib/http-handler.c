#include "http-handler.h"

#include <asm-generic/errno.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct HTTPRequest HTTPRequest;
typedef struct HTTPHeaders HTTPHeaders;

/**
 * A struct represents a typical HTTP Request parsed from a web client
 */
struct HTTPRequest {
  /** The requested HTTP method */
  char* method;
  /** The requested HTTP URI */
  char* uri;
  /** The version of the HTTP from the web client */
  char* version;
  /** The additional headers requested from the web client */
  HTTPHeaders* headers;
};

/**
 * A recursive struct represents a typical HTTP header with a linked list
 * approach
 */
struct HTTPHeaders {
  /** The current header name */
  char* name;
  /** The value of the current header */
  char* value;
  /** The next header */
  HTTPHeaders* next;
};

static int parseHTTPRequest(int receiveMessageSize,
                            char receiveMessage[receiveMessageSize],
                            HTTPRequest* httpRequest)
{
  printf("%s", receiveMessage);

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

  lineLen = strcspn(readBuffer, "\r\n");
  httpRequest->version = calloc(lineLen, sizeof(char));
  strncpy(httpRequest->version, readBuffer, lineLen);
  readBuffer += lineLen + 2; /* Skip through the read line and CRLF */
  httpRequest->version[lineLen] = '\0';

  HTTPHeaders* headers = NULL;
  HTTPHeaders* currentHeader = NULL;
  for (; readBuffer[0] != '\r' && readBuffer[1] != '\n'; readBuffer += lineLen + 2) {
    currentHeader = headers;
    headers = malloc(sizeof(HTTPHeaders));
    if (!headers) goto cleanup;
    lineLen = strcspn(readBuffer, ":");
    headers->name = calloc(lineLen, sizeof(char));
    strncpy(headers->name, readBuffer, lineLen);
    readBuffer += lineLen + 1;
    headers->name[lineLen] = '\0';
    while (*readBuffer == ' ') readBuffer++;
    lineLen = strcspn(readBuffer, "\r\n");
    headers->value = calloc(lineLen, sizeof(char));
    strncpy(headers->value, readBuffer, lineLen);
    headers->value[lineLen] = '\0';
    headers->next = currentHeader;
    printf("web-server: Inside: HTTP header: %s: %s\n", headers->name, headers->value);
  }
  httpRequest->headers = headers;

  for (HTTPHeaders* cur = httpRequest->headers; cur != NULL; cur = cur->next) {
    printf("web-server: Result: HTTP header: %s: %s\n", cur->name, cur->value);
  }

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
  int returnStatus = snprintf(httpResponseMessage,
                              httpResponseMessageSize,
                              "HTTP/1.1 %s",
                              httpStatusStr[httpResponseStatus]);
  if (returnStatus < 0) {
    errno = EILSEQ;     /* Encoding Error */
    return -1;          /* Failure */
  } else if (returnStatus > httpResponseMessageSize) {
    errno = EOVERFLOW;  /* String Too Large */
    return -1;          /* Failure */
  }
  return 0; /* Success */
};

int handleHTTPClientRequest(int receiveMessageSize,
                            char receiveMessage[receiveMessageSize],
                            int responseMessageSize,
                            char responseMessage[responseMessageSize])
{
  HTTPRequest httpRequest;

  int httpParseStatus = parseHTTPRequest(receiveMessageSize,
                                         receiveMessage,
                                         &httpRequest);
  if (httpParseStatus == -1 ) {
    fprintf(stderr, "web-server: Failed to parse HTTP request header\n");
    fprintf(stderr, "web-server: Received HTTP request as follows:\n%s",
            receiveMessage);
    return -1;  /* Failure */
  }

  printf("web-server: HTTP request method: %s\n", httpRequest.method);
  printf("web-server: HTTP request uri: %s\n", httpRequest.uri);
  printf("web-server: HTTP request version: %s\n", httpRequest.version);

  for (HTTPHeaders* currentHeader = httpRequest.headers; currentHeader != NULL; currentHeader = currentHeader->next) {
    printf("web-server: HTTP header: %s: %s\n", currentHeader->name, currentHeader->value);
  }

  int httpRequestMethodHandler = -1;
  for (int i = 0; i <= PATCH; i++)
    if (strcmp(httpRequest.method, httpMethodStr[i]) == 0)
      httpRequestMethodHandler = i;

  switch (httpRequestMethodHandler) {
    case GET:
      if (createHTTPResponseMessage(responseMessageSize, responseMessage, OK) == -1)
        return -1;  /* Failure */
      break;
    case HEAD:
    case POST:
    case PUT:
    case DELETE:
    case CONNECT:
    case OPTIONS:
    case TRACE:
    case PATCH:
      if (createHTTPResponseMessage(responseMessageSize, responseMessage, METHOD_NOT_ALLOWED) == -1)
        return -1;  /* Failure */
      break;
    default:
      if (createHTTPResponseMessage(responseMessageSize, responseMessage, NOT_IMPLEMENTED) == -1)
        return -1;  /* Failure */
      break;
  };

  return 0; /* Success */
};
