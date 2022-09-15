#ifndef HTTP_HEADER_H
#define HTTP_HEADER_H

/**
 *  Shorthand for struct HTTPHeader
 */
typedef struct HTTPHeader HTTPHeader;
/**
 * Define Stack's Element as HTTPRequest
 */
typedef struct HTTPHeader* Element;

/**
 * A recursive struct represents a typical HTTP header with a linked list
 * approach
 */
struct HTTPHeader {
  /** The current header name */
  char* name;
  /** The value of the current header */
  char* value;
};

/**
 * Initialise an HTTP header structure
 *
 * @return A pointer to the newly created HTTP header
 */
HTTPHeader* httpHeaderInit(void);

/**
 * Free an HTTP header
 *
 * @param httpHeader A pointer to an HTTP Header to be freed
 */
void httpHeaderFree(HTTPHeader* httpHeader);

#endif
