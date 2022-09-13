#include "http-handler.h"

#include <asm-generic/errno.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

static int parseHTTPRequest(int receiveMessageSize,
                            char receiveMessage[receiveMessageSize],
                            char* httpRequestMethod,
                            char* httpRequestPath,
                            char* httpRequestVersion,
                            char* delimeter)
{
  printf("%s", receiveMessage);

  char httpStatusLine[HTTP_REQUEST_STATUS_LINE_LEN] = { 0 };
  char* readBuffer = receiveMessage;
  int lineLen = 0;

  lineLen = strcspn(readBuffer, "\r\n");
  strncpy(httpStatusLine, readBuffer, lineLen);
  readBuffer += lineLen + 2; /* Skip through the read line and CRLF */
  httpStatusLine[lineLen] = '\0';

  printf("web-server: HTTP Status Line: %s\n", httpStatusLine);

  char httpHeaderLines[256][HTTP_REQUEST_STATUS_LINE_LEN] = { 0 };
  for (int i = 0; readBuffer[0] != '\r' && readBuffer[1] != '\n'; i++, readBuffer += lineLen + 2) {
    lineLen = strcspn(readBuffer, "\r\n");
    strncpy(httpHeaderLines[i], readBuffer, lineLen);
    httpHeaderLines[i][lineLen] = '\0';
    printf("web-server: HTTP Header Line: %s\n", httpHeaderLines[i]);
  }

  readBuffer = httpStatusLine;
  lineLen = strcspn(readBuffer, " ");
  strncpy(httpRequestMethod, readBuffer, lineLen);
  readBuffer += lineLen + 1;
  httpRequestMethod[lineLen] = '\0';
  printf("web-server: HTTP Method: %s\n", httpRequestMethod);

  lineLen = strcspn(readBuffer, " ");
  strncpy(httpRequestPath, readBuffer, lineLen);
  readBuffer += lineLen + 1;
  httpRequestPath[lineLen] = '\0';
  printf("web-server: HTTP Path: %s\n", httpRequestPath);

  lineLen = strcspn(readBuffer, " ");
  strncpy(httpRequestVersion, readBuffer, lineLen);
  httpRequestVersion[lineLen] = '\0';
  printf("web-server: HTTP Version: %s\n", httpRequestVersion);

  char httpHeaders[256][32] = { 0 };
  char httpHeaderValues[256][HTTP_REQUEST_STATUS_LINE_LEN - 32] = { 0 };
  for (int i = 0; i < 256; i++) {
    readBuffer = httpHeaderLines[i];
    lineLen = strcspn(readBuffer, ":");
    strncpy(httpHeaders[i], readBuffer, lineLen);
    readBuffer += lineLen + 1;
    httpHeaders[i][lineLen] = '\0';
    while (*readBuffer == ' ') readBuffer++;
    strncpy(httpHeaderValues[i], readBuffer, strlen(readBuffer));
    httpHeaderValues[i][strlen(readBuffer)] = '\0';
    printf("web-server: %s: %s\n", httpHeaders[i], httpHeaderValues[i]);
  }

  fflush(stdout);
  return 0; /* Success */

cleanup:
  perror("web-server");
  fflush(stdout);
  strncpy(httpRequestMethod, "", strlen(""));
  strncpy(httpRequestPath, "", strlen(""));
  strncpy(httpRequestVersion, "", strlen(""));
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
  char httpRequestMethod[HTTP_REQUEST_METHOD_LEN] = { 0 };
  char httpRequestPath[HTTP_REQUEST_PATH_LEN] = { 0 };
  char httpRequestVersion[HTTP_REQUEST_VERSION_LEN] = { 0 };

  int httpParseStatus = parseHTTPRequest(receiveMessageSize,
                                         receiveMessage,
                                         httpRequestMethod,
                                         httpRequestPath,
                                         httpRequestVersion,
                                         " ");
  if (httpParseStatus == -1 ) {
    fprintf(stderr, "web-server: Failed to parse HTTP request header\n");
    fprintf(stderr, "web-server: Received HTTP request as follows:\n%s",
            receiveMessage);
    return -1;  /* Failure */
  }

  int httpRequestMethodHandler = -1;
  for (int i = 0; i <= PATCH; i++)
    if (strcmp(httpRequestMethod, httpMethodStr[i]) == 0)
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
