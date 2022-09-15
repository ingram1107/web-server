/** @file
 *  @author Little Clover
 *  @date 2022-09-14 14:11:46 PM +0800
 */
#ifndef HTTP_REQUEST_H
#define HTTP_REQUEST_H

#include "stack.h"

/**
 * Shorthand for struct HTTPRequest
 */
typedef struct HTTPRequest HTTPRequest;

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
  Stack headers;
};

/**
 * Initialise a HTTP request
 *
 * @return A pointer to the newly created HTTP request
 */
HTTPRequest* httpRequestInit(void);

/**
 * Free a HTTP request
 *
 * @param httpRequest A pointer to an HTTP request
 */
void httpRequestFree(HTTPRequest* httpRequest);

#endif
