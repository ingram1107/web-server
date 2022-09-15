#include "http-header.h"

#include <asm-generic/errno-base.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

HTTPHeader* httpHeaderInit(void) {
  HTTPHeader* newHTTPHeader = malloc(sizeof(struct HTTPHeader));

  if (!newHTTPHeader) {
    errno = ENOMEM;
    perror("http-header");
    exit(EXIT_FAILURE);
  }

  return newHTTPHeader;
}

void httpHeaderFree(HTTPHeader* httpHeader) {
  free(httpHeader->value);
  free(httpHeader->name);
  free(httpHeader);
}
