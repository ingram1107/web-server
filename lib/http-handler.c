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
  char* httpRequestToken;
  
  /**
   * TODO: Might need to design a filter that do the same job as strtok do but
   * much resilient to buffer overflow and able to treat all space character as
   * the same.
   */
  /* Get HTTP request method type */
  httpRequestToken = strtok(receiveMessage, delimeter);
  if (httpRequestToken != NULL) {
    strncpy(httpRequestMethod, httpRequestToken, strlen(httpRequestToken));
    printf("web-server: HTTP Method:       %s\n", httpRequestMethod);
  } else goto cleanup;

  /* Get HTTP request path */
  httpRequestToken = strtok(NULL, delimeter);
  if (httpRequestToken != NULL) {
    strncpy(httpRequestPath, httpRequestToken, strlen(httpRequestToken));
    printf("web-server: HTTP Request Path: %s\n", httpRequestPath);
  } else goto cleanup;

  /* Get HTTP request version */
  httpRequestToken = strtok(NULL, "\n");  /* TODO: replace this as delimeter so that it is generic */
  if (httpRequestToken != NULL) {
    strncpy(httpRequestVersion, httpRequestToken, strlen(httpRequestToken));
    printf("web-server: HTTP Version:      %s\n", httpRequestVersion);
  } else goto cleanup;

  fflush(stdout);
  return 0; /* Success */

cleanup:
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
