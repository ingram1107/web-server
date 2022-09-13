/**
 * @file
 * @author Little Clover
 * @date 2022-09-10 18:13:09 PM +0800
 */
#ifndef HTTP_HANDLER_H
#define HTTP_HANDLER_H

/**
 * Supported maximum HTTP header size
 */
#define HTTP_HEADER_LEN 8192
/**
 * Maximum string length of HTTP request method
 */
#define HTTP_REQUEST_METHOD_LEN 8
/**
 * Maximum string length of HTTP request path
 */
#define HTTP_REQUEST_PATH_LEN 1024
/**
 * Maximum string length of HTTP request version
 */
#define HTTP_REQUEST_VERSION_LEN 10
/**
 * Maximum size of HTTP status line
 */
#define HTTP_REQUEST_STATUS_LINE_LEN (HTTP_REQUEST_METHOD_LEN) + (HTTP_REQUEST_PATH_LEN) + (HTTP_REQUEST_VERSION_LEN)
/**
 * CRLF character
 */
#define CRLF "\r\n\r\n"

/**
 * Supported HTTP request method adhered to the standard.
 *
 * @see https://developer.mozilla.org/en-US/docs/Web/HTTP/Methods
 */
enum httpMethod { GET, HEAD, POST, PUT, DELETE, CONNECT, OPTIONS, TRACE, PATCH };

/**
 * A string map to @a httpMethod.
 *
 * @private
 *
 * @see httpMethod
 */
static const char* const httpMethodStr[] = {
  [GET] = "GET",
  [HEAD] = "HEAD",
  [POST] = "POST",
  [PUT] = "PUT",
  [DELETE] = "DELETE",
  [CONNECT] = "CONNECT",
  [OPTIONS] = "OPTIONS",
  [TRACE] = "TRACE",
  [PATCH] = "PATCH",
};

/**
 * Supported HTTP response status code adhered to the standard.
 *
 * @see https://developer.mozilla.org/en-US/docs/Web/HTTP/Status
 */
enum httpStatus {
  CONTINUE = 100,
  SWITCHING_PROTOCOLS,
  PROCESSING,
  EARLY_HINTS,
  OK = 200,
  CREATED,
  ACCEPTED,
  NON_AUTHORITATIVE_INFORMATION,
  NO_CONTENT,
  RESET_CONTENT,
  PARTIAL_CONTENT,
  MULTI_STATUS,
  ALREADY_REPORTED,
  IM_USED = 226,
  MULTIPLE_CHOICE = 300,
  MOVED_PERMANENTLY,
  FOUND,
  SEE_OTHER,
  NOT_MODIFIED,
  USE_PROXY,
  UNUSED,
  TEMPORARY_REDIRECT,
  PERMANENT_REDIRECT,
  BAD_REQUEST = 400,
  UNAUTHORIZED,
  PAYMENT_REQUIRED,
  FORBIDDEN,
  NOT_FOUND,
  METHOD_NOT_ALLOWED,
  NOT_ACCEPTABLE,
  PROXY_AUTHENTICATION_REQUIRED,
  REQUEST_TIMEOUT,
  CONFLICT,
  GONE,
  LENGTH_REQUIRED,
  PRECONDITION_FAILED,
  PAYLOAD_TOO_LARGE,
  URI_TOO_LONG,
  UNSUPPORTED_MEDIA_TYPE,
  RANGE_NOT_SATISFIABLE,
  EXPECTATION_FAILED,
  IM_A_TEAPOT,
  MISDIRECTED_REQUEST = 421,
  UNPROCESSABLE_ENTITY,
  LOCKED,
  FAILED_DEPENDENCY,
  TOO_EARLY,
  UPGRADE_REQUIRED,
  PRECONDITION_REQUIRED = 428,
  TOO_MANY_REQUESTS,
  REQUEST_HEADER_FIELDS_TOO_LARGE = 431,
  UNAVAILABLE_FOR_LEGAL_REASONS = 451,
  INTERNAL_SERVER_ERROR = 500,
  NOT_IMPLEMENTED,
  BAD_GATEWAY,
  SERVICE_UNAVAILABLE,
  GATEWAY_TIMEOUT,
  HTTP_VERSION_NOT_SUPPORTED,
  VARIANT_ALSO_NEGOTIATES,
  INSUFFICIENT_STORAGE,
  LOOP_DETECTED,
  NOT_EXTENDED = 510,
  NETWORK_AUTHENTICATION_REQURIED
};

/**
 * A string map to @a httpStatus.
 *
 * @private
 *
 * @see httpStatus
 */
static const char* const httpStatusStr[] = {
  [CONTINUE] = "100 OK" CRLF,
  [SWITCHING_PROTOCOLS] = "101 Switching Protocols" CRLF,
  [PROCESSING] = "102 Processing" CRLF,
  [EARLY_HINTS] = "103 Early Hints" CRLF,
  [OK] = "200 OK" CRLF,
  [CREATED] = "201 Created" CRLF,
  [ACCEPTED] = "202 Accepted" CRLF,
  [NON_AUTHORITATIVE_INFORMATION] = "203 Non-Authoritative Information" CRLF,
  [NO_CONTENT] = "204 No Content" CRLF,
  [RESET_CONTENT] = "205 Reset Content" CRLF,
  [PARTIAL_CONTENT] = "206 Partial Content" CRLF,
  [MULTI_STATUS] = "207 Multi-Status" CRLF,
  [ALREADY_REPORTED] = "208 Already Reported" CRLF,
  [IM_USED] = "226 IM Used" CRLF,
  [MULTIPLE_CHOICE] = "300 Multiple Choice" CRLF,
  [MOVED_PERMANENTLY] = "301 Moved Permanently" CRLF,
  [FOUND] = "302 Found" CRLF,
  [SEE_OTHER] = "303 See Other" CRLF,
  [NOT_MODIFIED] = "304 Not Modified" CRLF,
  [USE_PROXY] = "305 Use Proxy" CRLF,
  [TEMPORARY_REDIRECT] = "306 Temporary Redirect" CRLF,
  [PERMANENT_REDIRECT] = "307 Permanent Redirect" CRLF,
  [BAD_REQUEST] = "400 Bad Request" CRLF,
  [UNAUTHORIZED] = "401 Unauthorized" CRLF,
  [PAYMENT_REQUIRED] = "402 Payment Required" CRLF,
  [FORBIDDEN] = "403 Forbidden" CRLF,
  [NOT_FOUND] = "404 Not Found" CRLF,
  [METHOD_NOT_ALLOWED] = "405 Method Not Allowed" CRLF,
  [NOT_ACCEPTABLE] = "406 Not Acceptable" CRLF,
  [PROXY_AUTHENTICATION_REQUIRED] = "407 Proxy Authentication Required" CRLF,
  [REQUEST_TIMEOUT] = "408 Request Timeout" CRLF,
  [CONFLICT] = "409 Conflict" CRLF,
  [GONE] = "410 Gone" CRLF,
  [LENGTH_REQUIRED] = "411 Length Required" CRLF,
  [PRECONDITION_FAILED] = "412 Precondition Failed" CRLF,
  [PAYLOAD_TOO_LARGE] = "413 Payload Too Large" CRLF,
  [URI_TOO_LONG] = "414 URI Too Long" CRLF,
  [UNSUPPORTED_MEDIA_TYPE] = "415 Unsupported Media Type" CRLF,
  [RANGE_NOT_SATISFIABLE] = "416 Range Not Satisfiable" CRLF,
  [EXPECTATION_FAILED] = "417 Expectation Failed" CRLF,
  [IM_A_TEAPOT] = "418 I'm a teapot" CRLF,
  [MISDIRECTED_REQUEST] = "421 Misdirected Request" CRLF,
  [UNPROCESSABLE_ENTITY] = "422 Unprocessable Entity" CRLF,
  [LOCKED] = "423 Locked" CRLF,
  [FAILED_DEPENDENCY] = "424 Failed Dependency" CRLF,
  [TOO_EARLY] = "425 Too Early" CRLF,
  [UPGRADE_REQUIRED] = "426 Upgrade Required" CRLF,
  [PRECONDITION_REQUIRED] = "428 Precondition Required" CRLF,
  [TOO_MANY_REQUESTS] = "429 Too Many Requests" CRLF,
  [REQUEST_HEADER_FIELDS_TOO_LARGE] = "431 Request Header Fields Too Large" CRLF,
  [UNAVAILABLE_FOR_LEGAL_REASONS] = "451 Unavailable For Legal Reasons" CRLF,
  [INTERNAL_SERVER_ERROR] = "500 Internal Server Error" CRLF,
  [NOT_IMPLEMENTED] = "501 Not Implemented" CRLF,
  [BAD_GATEWAY] = "502 Bad Gateway" CRLF,
  [SERVICE_UNAVAILABLE] = "503 Service Unavailable" CRLF,
  [GATEWAY_TIMEOUT] = "504 Gateway Timeout" CRLF,
  [HTTP_VERSION_NOT_SUPPORTED] = "505 Http Version Not Supported" CRLF,
  [VARIANT_ALSO_NEGOTIATES] = "506 Variant Also Negotiates" CRLF,
  [INSUFFICIENT_STORAGE] = "507 Insufficient Storage" CRLF,
  [LOOP_DETECTED] = "508 Loop Detected" CRLF,
  [NOT_EXTENDED] = "510 Not Extended" CRLF,
  [NETWORK_AUTHENTICATION_REQURIED] = "511 Network Authentication Requried" CRLF
};

/**
 * parse HTTP request from the received message to get its request method,
 * requested path and version base on the delimeter.
 *
 * @param[in] receiveMessageSize  The size of the @a receiveMessage
 * @param[in] receiveMessage      The received HTTP request message with a size
 *                                of @a receiveMessageSize
 * @param[out] httpRequestMethod  The parsed HTTP request method from
 *                                @a receiveMessage
 * @param[out] httpRequestPath    The parsed HTTP request path from
 *                                @a receiveMessage
 * @param[out] httpRequestVersion The parsed HTTP request version from
 *                                @a receiveMessage
 * @param[in] delimeter           The delimeter for the parser to parse
 *                                @a receiveMessage
 *
 * @retval 0  @a httpRequestMethod, @a httpRequestPath and
 *            @a httpRequestVersion is parsed successfully from
 *            @a receiveMessage.
 * @retval -1 Parsing error on either @a httpRequestMethod, @a httpRequestPath
 *            or @a httpRequestVersion.
 * @private
 */
static int parseHTTPRequest(int receiveMessageSize,
                            char receiveMessage[receiveMessageSize],
                            char* httpRequestMethod,
                            char* httpRequestPath,
                            char* httpRequestVersion,
                            char* delimeter);

/**
 * create a HTTP response message based on the status code.
 *
 * @param[in] httpResponseMessageSize The size of the @a httpResponseMessage
 * @param[out] httpResponseMessage    The will be created HTTP response Message
 *                                    with a size of @a httpResponseMessage
 * @param[in] httpResponseStatus      The response status code to create
 *                                    @a httpResponseMessage
 *
 * @retval 0  @a httpResponseMessage has been successfully created.
 * @retval -1 There is an encoding error or the HTTP status message string is
 *            too large to @a httpResponseMessage
 * @private
 *
 * @see httpStatus
 * @see httpStatusStr[]
 */
static int createHTTPResponseMessage(int httpResponseMessageSize,
                                     char httpResponseMessage[httpResponseMessageSize],
                                     int httpResponseStatus);

/**
 * handle a HTTP client request message and then create an HTTP response status
 * code based its request method.
 *
 * @param[in] receiveMessageSize The size of the @a receiveMessage
 * @param[in] receiveMessage The HTTP client request message received with a
 *                           size of @a receiveMessageSize
 * @param[in] responseMessageSize The size of the @a responseMessage
 * @param[out] responseMessage The HTTP response status code formatted as a
 *                             string with a size of @a responseMessageSize
 *
 * @retval 0  @a responseMessage has been successfully created.
 * @retval -1 Failure in either parsing the @a receiveMessage or creating a
 *            @a responseMessage
 *
 * @see parseHTTPRequest()
 * @see createHTTPResponseMessage()
 */
int handleHTTPClientRequest(int receiveMessageSize,
                            char receiveMessage[receiveMessageSize],
                            int responseMessageSize,
                            char responseMessage[responseMessageSize]);

#endif
