#ifndef FUNC_H
#define FUNC_H
// Include Guards


void debug(int log_message_type, char *message, char *additional_info, int socket_fd)
{
	int fd;
	char logbuffer[BUFSIZE * 2];

	switch (log_message_type)
	{
	case ERROR:
		(void)sprintf(logbuffer, "ERROR: %s:%s Errno=%d exiting pid=%d\n", message, additional_info, errno, getpid());
		break;
	case MOVEDPERMANENTLY:
		// 301 Moved Permanently
		(void)sprintf(logbuffer, "MOVED PERMANENTLY: %s:%s\n", message, additional_info);
		break;
	case BADREQUEST:
		// 400 Bad Request
		(void)sprintf(logbuffer, "BAD REQUEST: %s:%s\n", message, additional_info);
		send_error_response(BADREQUEST, "Bad Request", socket_fd);
		break;
	case FORBIDDEN:
		// 403 Forbidden
		(void)sprintf(logbuffer, "FORBIDDEN: %s:%s\n", message, additional_info);
		send_error_response(FORBIDDEN, "Forbidden", socket_fd);
		break;
	case NOTFOUND:
		// 404 Not Found
		(void)sprintf(logbuffer, "NOT FOUND: %s:%s\n", message, additional_info);
		send_error_response(NOTFOUND, "Not Found", socket_fd);
		break;
	case METHODNOTALLOWED:
		// 405 Method Not Allowed
		(void)sprintf(logbuffer, "METHOD NOT ALLOWED: %s:%s\n", message, additional_info);
		send_error_response(METHODNOTALLOWED, "Method Not Allowed", socket_fd);
		break;
	case UNSUPPORTEDMEDIATYPE:
		// 415 Unsupported Media Type
		(void)sprintf(logbuffer, "UNSUPPORTED MEDIA TYPE: %s:%s\n", message, additional_info);
		send_error_response(UNSUPPORTEDMEDIATYPE, "Unsupported Media Type", socket_fd);
		break;
	case HTTPVERSIONNOTSUPPORTED:
		// HTTP Version Not Supported
		(void)sprintf(logbuffer, "HTTP VERSION NOT SUPPORTED: %s:%s\n", message, additional_info);
		send_error_response(HTTPVERSIONNOTSUPPORTED, "HTTP Version Not Supported", socket_fd);
		break;
	case LOG:
		(void)sprintf(logbuffer, "INFO: %s:%s:%d\n", message, additional_info, socket_fd);
		break;
	}

#endif