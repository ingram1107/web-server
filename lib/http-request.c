#include "http-request.h"

#include "http-header.h"
#include <asm-generic/errno-base.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

HTTPRequest* httpRequestInit(void) {
  HTTPRequest* newHTTPRequest = malloc(sizeof(struct HTTPRequest));
  if (!newHTTPRequest) {
    errno = ENOMEM;
    perror("http-request");
    exit(EXIT_FAILURE);
  }

  newHTTPRequest->headers = stackInit();
  return newHTTPRequest;
}

void httpRequestFree(HTTPRequest* httpRequest) {
  stackFree(httpRequest->headers, httpHeaderFree);
  free(httpRequest->version);
  free(httpRequest->uri);
  free(httpRequest->method);
  free(httpRequest);
}
