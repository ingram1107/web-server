#include "http-header.h"

#include <stdlib.h>

HTTPHeader* httpHeaderInit(void) {
  HTTPHeader* newHTTPHeader = malloc(sizeof(struct HTTPHeader));
  return newHTTPHeader;
}

void httpHeaderFree(HTTPHeader* httpHeader) {
  free(httpHeader->value);
  free(httpHeader->name);
  free(httpHeader);
}
