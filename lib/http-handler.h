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
 * CTRLF character
 */
#define CTRLF "\r\n\r\n"

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
  [CONTINUE] = "100 OK" CTRLF,
  [SWITCHING_PROTOCOLS] = "101 Switching Protocols" CTRLF,
  [PROCESSING] = "102 Processing" CTRLF,
  [EARLY_HINTS] = "103 Early Hints" CTRLF,
  [OK] = "200 OK" CTRLF,
  [CREATED] = "201 Created" CTRLF,
  [ACCEPTED] = "202 Accepted" CTRLF,
  [NON_AUTHORITATIVE_INFORMATION] = "203 Non-Authoritative Information" CTRLF,
  [NO_CONTENT] = "204 No Content" CTRLF,
  [RESET_CONTENT] = "205 Reset Content" CTRLF,
  [PARTIAL_CONTENT] = "206 Partial Content" CTRLF,
  [MULTI_STATUS] = "207 Multi-Status" CTRLF,
  [ALREADY_REPORTED] = "208 Already Reported" CTRLF,
  [IM_USED] = "226 IM Used" CTRLF,
  [MULTIPLE_CHOICE] = "300 Multiple Choice" CTRLF,
  [MOVED_PERMANENTLY] = "301 Moved Permanently" CTRLF,
  [FOUND] = "302 Found" CTRLF,
  [SEE_OTHER] = "303 See Other" CTRLF,
  [NOT_MODIFIED] = "304 Not Modified" CTRLF,
  [USE_PROXY] = "305 Use Proxy" CTRLF,
  [TEMPORARY_REDIRECT] = "306 Temporary Redirect" CTRLF,
  [PERMANENT_REDIRECT] = "307 Permanent Redirect" CTRLF,
  [BAD_REQUEST] = "400 Bad Request" CTRLF,
  [UNAUTHORIZED] = "401 Unauthorized" CTRLF,
  [PAYMENT_REQUIRED] = "402 Payment Required" CTRLF,
  [FORBIDDEN] = "403 Forbidden" CTRLF,
  [NOT_FOUND] = "404 Not Found" CTRLF,
  [METHOD_NOT_ALLOWED] = "405 Method Not Allowed" CTRLF,
  [NOT_ACCEPTABLE] = "406 Not Acceptable" CTRLF,
  [PROXY_AUTHENTICATION_REQUIRED] = "407 Proxy Authentication Required" CTRLF,
  [REQUEST_TIMEOUT] = "408 Request Timeout" CTRLF,
  [CONFLICT] = "409 Conflict" CTRLF,
  [GONE] = "410 Gone" CTRLF,
  [LENGTH_REQUIRED] = "411 Length Required" CTRLF,
  [PRECONDITION_FAILED] = "412 Precondition Failed" CTRLF,
  [PAYLOAD_TOO_LARGE] = "413 Payload Too Large" CTRLF,
  [URI_TOO_LONG] = "414 URI Too Long" CTRLF,
  [UNSUPPORTED_MEDIA_TYPE] = "415 Unsupported Media Type" CTRLF,
  [RANGE_NOT_SATISFIABLE] = "416 Range Not Satisfiable" CTRLF,
  [EXPECTATION_FAILED] = "417 Expectation Failed" CTRLF,
  [IM_A_TEAPOT] = "418 I'm a teapot" CTRLF,
  [MISDIRECTED_REQUEST] = "421 Misdirected Request" CTRLF,
  [UNPROCESSABLE_ENTITY] = "422 Unprocessable Entity" CTRLF,
  [LOCKED] = "423 Locked" CTRLF,
  [FAILED_DEPENDENCY] = "424 Failed Dependency" CTRLF,
  [TOO_EARLY] = "425 Too Early" CTRLF,
  [UPGRADE_REQUIRED] = "426 Upgrade Required" CTRLF,
  [PRECONDITION_REQUIRED] = "428 Precondition Required" CTRLF,
  [TOO_MANY_REQUESTS] = "429 Too Many Requests" CTRLF,
  [REQUEST_HEADER_FIELDS_TOO_LARGE] = "431 Request Header Fields Too Large" CTRLF,
  [UNAVAILABLE_FOR_LEGAL_REASONS] = "451 Unavailable For Legal Reasons" CTRLF,
  [INTERNAL_SERVER_ERROR] = "500 Internal Server Error" CTRLF,
  [NOT_IMPLEMENTED] = "501 Not Implemented" CTRLF,
  [BAD_GATEWAY] = "502 Bad Gateway" CTRLF,
  [SERVICE_UNAVAILABLE] = "503 Service Unavailable" CTRLF,
  [GATEWAY_TIMEOUT] = "504 Gateway Timeout" CTRLF,
  [HTTP_VERSION_NOT_SUPPORTED] = "505 Http Version Not Supported" CTRLF,
  [VARIANT_ALSO_NEGOTIATES] = "506 Variant Also Negotiates" CTRLF,
  [INSUFFICIENT_STORAGE] = "507 Insufficient Storage" CTRLF,
  [LOOP_DETECTED] = "508 Loop Detected" CTRLF,
  [NOT_EXTENDED] = "510 Not Extended" CTRLF,
  [NETWORK_AUTHENTICATION_REQURIED] = "511 Network Authentication Requried" CTRLF
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
