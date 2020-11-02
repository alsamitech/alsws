#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <dirent.h>
#include <stdbool.h>
#include <regex.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/sendfile.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFSIZE 8096
#define ERROR 42
#define LOG 44
#define TIMEOUT 10

/* HTTP STATUS CODES */
#define OK 200
#define MOVEDPERMANENTLY 301
#define BADREQUEST 400
#define FORBIDDEN 403
#define NOTFOUND 404
#define METHODNOTALLOWED 405
#define UNSUPPORTEDMEDIATYPE 415
#define HTTPVERSIONNOTSUPPORTED 505

// Prototipos de funcion
void debug(int, char *, char *, int);
bool match(const char *, const char *);
char *get_file_type(const char *);
unsigned long fsize(char *);
char *build_header(int, char *, const char *, unsigned long);
char *build_error_page(int, char *);
void send_response_in_8k_blocks(char *, int);
void send_error_response(int, char *, int);
void close_connection(int);
bool is_directory(const char *);
void redirect_trailing_slash(char *, int);
void list_directory_content(char *, int);
char *replace_substring(const char *, const char *, const char *);
bool search_user(char *, char *);
void validate_login(char *, int);
void process_web_request(int);

// MIME's
struct
{
	char *ext;
	char *filetype;
} extensions[] = {
	{"gif", "image/gif"},
	{"jpg", "image/jpg"},
	{"jpeg", "image/jpeg"},
	{"png", "image/png"},
	{"ico", "image/ico"},
	{"zip", "image/zip"},
	{"gz", "image/gz"},
	{"tar", "image/tar"},
	{"htm", "text/html"},
	{"html", "text/html"},
	{"mp4", "video/mp4"},
	{"css", "text/css"},
};

int main(int argc, char **argv)
{
	int i, port, pid, listenfd, socket_fd;
	socklen_t length;
	static struct sockaddr_in cli_addr;	 // static = Inicializado con ceros
	static struct sockaddr_in serv_addr; // static = Inicializado con ceros

	//  Expected Arguments: 
	//
	//	argv[1]
	//	The first argument of the program expects the port on which the server will listen
	//
	//  argv[2]
	//  In the second argument of the program, the directory in which the server files are located is expected

	/* We verify that the arguments passed when starting the program are as expected */
	if (!argv[1] || !argv[2])
	{
		(void)printf("ERROR: Not all args have been entered\n");
		exit(EXIT_FAILURE);
	}

	if (atoi(argv[1]) < 0)
	{
		(void)printf("ERROR: The port entered is not correct.\n");
		exit(EXIT_FAILURE);
	}

	/*
		We verify that the chosen directory is suitable. What is not a system directory and what are
permissions to be used
	*/
	if (chdir(argv[2]) == -1)
	{
		(void)printf("Unable to change directory to: %s\n", argv[2]);
		exit(4);
	}

	if (fork() != 0)
		return 0;
	(void)signal(SIGCHLD, SIG_IGN);
	(void)signal(SIGHUP, SIG_IGN);

	debug(LOG, "Starting web server...", argv[1], getpid());

	if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		debug(ERROR, "system call", "socket", 0);

	port = atoi(argv[1]);

	if (port < 0 || port > 60000)
		debug(ERROR, "Invalid port, try a port from 1 to 60000", argv[1], 0);

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(port);

	if (bind(listenfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{
		perror("ERROR");
		debug(ERROR, "system call", "bind", 0);
	}

	if (listen(listenfd, 64) < 0)
		debug(ERROR, "system call", "listen", 0);

	// accept Connection
	while (1)
	{
		length = sizeof(cli_addr);
		if ((socket_fd = accept(listenfd, (struct sockaddr *)&cli_addr, &length)) < 0)
		{
			perror("ERROR");
			debug(ERROR, "system call", "accept", 0);
		}
		else
			debug(LOG, "Connection Established", "Server Connection Established", socket_fd);

		if ((pid = fork()) < 0)
		{
			debug(ERROR, "system call", "fork", 0);
		}
		else
		{
			if (pid == 0)
			{ 
				(void)close(listenfd);
				
				fd_set rfds;
				struct timeval tv = {TIMEOUT, 0};
				int retval;
				while (1)
				{
					FD_ZERO(&rfds);
					FD_SET(socket_fd, &rfds);

					retval = select(socket_fd + 1, &rfds, NULL, NULL, &tv);

					if (!retval)
					{
						
						close_connection(socket_fd);
						exit(1);
					}
					else
					{
						if (FD_ISSET(socket_fd, &rfds))
						{
							process_web_request(socket_fd);
						}
					}
				}
			}
			else
			{ 
				(void)close(socket_fd);
			}
		}
	}
}

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

	if ((fd = open("webserver.log", O_CREAT | O_WRONLY | O_APPEND, 0644)) >= 0)
	{
		(void)write(fd, logbuffer, strlen(logbuffer));
		(void)write(fd, "\n", 1);
		(void)close(fd);
	}

	if (log_message_type != LOG)
	{
		if (log_message_type != ERROR)
			close_connection(socket_fd);
		exit(3);
	}
}

bool match(const char *str, const char *pattern)
{
	regex_t regex;
	if (regcomp(&regex, pattern, REG_EXTENDED) != 0)
	{
		fprintf(stderr, "Unable to compile regular expression\n");
		return false;
	}

	int result = regexec(&regex, str, 0, NULL, 0);
	regfree(&regex);
	if (result == REG_NOMATCH)
	{
		return false;
	}
	return true;
}


char *get_file_type(const char *url)
{
	const char *dot = strrchr(url, '.');
	if (!dot || dot == url)
	{
		return NULL;
	}

	const char *ext = dot + 1;

	for (int i = 0; i < sizeof(extensions) / sizeof(extensions[0]); i++)
	{
		if (strcmp(extensions[i].ext, ext) == 0)
		{
			return extensions[i].filetype;
		}
	}

	// Extension no compatible
	return NULL;
}

unsigned long fsize(char *url)
{
	FILE *file = fopen(url, "r");

	fseek(file, 0, SEEK_END);
	unsigned long length = (unsigned long)ftell(file);

	fclose(file);
	return length;
}

char *build_header(int status_code, char *message, const char *filetype, unsigned long file_size)
{
	char *header;

	char buf[30];
	time_t now = time(0);
	struct tm tm = *gmtime(&now);
	strftime(buf, sizeof buf, "%a, %d %b %Y %H:%M:%S %Z", &tm);

	// implement http/1.1
	asprintf(&header, "HTTP/1.1 %d %s\r\n"
					"Host: web.sstt2380.org\r\n"
					"Server: Apache/2.4.1 (Unix)\r\n"
					"Content-Length: %ld\r\n"
					"Date: %s\r\n"
					"Connection: Keep-Alive\r\n"
					"Content-Type: %s; charset=UTF-8\r\n"
					"Keep-Alive: timeout=%d, max=100\r\n"
					"\r\n",
			status_code, message, file_size, buf, filetype, TIMEOUT);

	return header;
}

// Función auxiliar para construir el esqueleto HTML de las páginas de error
char *build_error_page(int status_code, char *message)
{
	char *data;
	asprintf(&data, "<!DOCTYPE html>\n"
					"<html>\n\n"
					"<head>\n"
					"\t<link rel='icon' type='image/ico' href='/favicon.ico'>\n"
					"\t<link rel='stylesheet' type='text/css' href='/styles.css'>\n"
					"\t<meta charset='UTF-8'>\n"
					"\t<title>Servicios Telemáticos</title>\n"
					"</head>\n\n"
					"<body>\n"
					"\t<div>\n"
					"\t\t<h1>ERROR %d</h1>\n"
					"\t\t<h2>%s</h2>\n"
					"\t\t<img src='/index_files/error.jpg' />\n"
					"\t</div>\n"
					"</body>\n\n"
					"</html>",
			 status_code, message);

	return data;
}

// Función auxiliar para enviar la respuesta HTTP en bloques de tamaño maximo 8K
void send_response_in_8k_blocks(char *response, int socket_fd)
{
	int length = strlen(response);
	if (length > BUFSIZE)
	{
		int n_segmentos = length / BUFSIZE;

		for (int i = 0; i < n_segmentos; i++, response += BUFSIZE)
		{
			write(socket_fd, response, BUFSIZE);
		}
	}
	else
	{
		write(socket_fd, response, strlen(response));
	}
}

// Función auxiliar para enviar la respuesta HTTP de los errores
void send_error_response(int status_code, char *message, int socket_fd)
{
	// Código HTML de la página de error
	char *data = build_error_page(status_code, message);

	// Construimos la cabecera de la respuesta HTTP
	char *header = build_header(status_code, message, "text/html", strlen(data));

	// Construimos la respuesta
	char *response;
	asprintf(&response, "%s"
					  "%s",
			header, data);

	/*
		Enviamos la respuesta en bloques de 8K ya que, en cualquier otro caso,
		el código HTML podría superar los 8K de tamaño.
	*/
	send_response_in_8k_blocks(response, socket_fd);

	debug(LOG, "Pagina de error enviada", message, socket_fd);

	free(response);
	free(header);
	free(data);
}

// Función auxiliar para cerrar la conexión
void close_connection(int socket_fd)
{
	char buf[30];
	time_t now = time(0);
	struct tm tm = *gmtime(&now);
	strftime(buf, sizeof(buf), "%a, %d %b %Y %H:%M:%S %Z", &tm);

	char *response;
	asprintf(&response, "HTTP 1.1 OK\r\n"
						"Host: web.sstt2380.org\r\n"
						"Server: Apache/2.4.1 (Unix)\r\n"
						"Date: %s\r\n"
						"Connection: close\r\n"
						"Content-Length: 0\r\n"
						"\r\n",
			 buf);

	write(socket_fd, response, strlen(response));

	debug(LOG, "Conexión cerrada", "El servidor ha cerrado la conexion con el cliente", socket_fd);
	close(socket_fd);
}

// Función auxiliar para comprobar si el fichero es un directorio existente
bool is_directory(const char *path)
{
	struct stat statbuf;
	if (stat(path, &statbuf) != 0)
		return 0;
	return S_ISDIR(statbuf.st_mode);
}

// Funcion auxiliar para redirigir a URLs con Trailing Slash
void redirect_trailing_slash(char *url, int socket_fd)
{
	char buf[30];
	time_t now = time(0);
	struct tm tm = *gmtime(&now);
	strftime(buf, sizeof(buf), "%a, %d %b %Y %H:%M:%S %Z", &tm);

	char *response;
	asprintf(&response, "HTTP/1.1 301 Moved Permanently\r\n"
						"Host: web.sstt2380.org\r\n"
						"Server: Apache/2.4.1 (Unix)\r\n"
						"Date: %s\r\n"
						"Location: %s/\r\n"
						"Connection: close\r\n"
						"Content-Length: 0\r\n"
						"\r\n",
			 buf, url);

	// Enviamos la respuesta de redireccionamiento
	write(socket_fd, response, strlen(response));
	free(response);
	free(url);

	debug(MOVEDPERMANENTLY, "Movido permanentemente", "Redirigiendo a la misma URL con barra diagonal", socket_fd);
}

// Función auxiliar que lista el contenido de un directorio
void list_directory_content(char *url, int socket_fd)
{
	DIR *dir;
	struct dirent *ent;

	// Si el directorio existe...
	if ((dir = opendir(url)) != NULL)
	{
		// Construimos el código HTML con el contenido
		char *data;
		asprintf(&data, "<!DOCTYPE html>\n"
						"<html>\n\n"
						"<head>\n"
						"\t<link rel='icon' type='image/ico' href='/favicon.ico'>\n"
						"\t<meta charset='UTF-8' />"
						"\t<title>Index of %s</title>\n"
						"</head>\n\n"
						"<body>\n"
						"<h1>Index of %s</h1>"
						"\t<table>\n",
				 url, url);

		while ((ent = readdir(dir)) != NULL)
		{
			// Icono correspondiente a cada tipo de entrada
			char *icon_path =
				(ent->d_type == DT_DIR) ? "/icons/dir.png" : "/icons/regfile.png";

			asprintf(&data, "%s"
							"\t\t<tr>\n"
							"\t\t\t<td valign='top'><img src='%s' /></td>\n",
					 data, icon_path);

			// Enlace al contenido
			asprintf(&data, "%s"
							"\t\t\t<td><a href='%s'>%s</a></td>\n"
							"\t\t</tr>\n",
					 data, ent->d_name, ent->d_name);
		}

		asprintf(&data, "%s"
						"\t</table>\n"
						"</body>\n\n"
						"</html>\n",
				 data);

		// Construimos la cabecera de la respuesta HTTP
		char *header = build_header(OK, "OK", "text/html", strlen(data));

		// Construimos la respuesta
		char *response;
		asprintf(&response, "%s"
						  "%s",
				header, data);

		// Enviamos la respuesta con el listado del directorio en bloques de 8K
		send_response_in_8k_blocks(response, socket_fd);

		free(response);
		free(header);
		free(data);

		// Cerramos el directorio
		closedir(dir);
	}
}

// Funcion encargada de reemplazar subcadenas de una cadena
char *replace_substring(const char *s, const char *old, const char *new)
{
	char *result;
	int i, cnt = 0;
	int new_len = strlen(new);
	int old_len = strlen(old);

	for (i = 0; s[i] != '\0'; i++)
	{
		if (strstr(&s[i], old) == &s[i])
		{
			cnt++;
			i += old_len - 1;
		}
	}

	// Construimos una nueva cadena del tamaño apropiado
	result = (char *)malloc(i + cnt * (new_len - old_len) + 1);

	i = 0;
	while (*s)
	{
		// compare the substring with the result
		if (strstr(s, old) == s)
		{
			strcpy(&result[i], new);
			i += new_len;
			s += old_len;
		}
		else
			result[i++] = *s++;
	}

	result[i] = '\0';
	return result;
}

// Funcion encargada de comprobar si existe algun usuario con el mismo correo y contraseña
bool search_user(char *email, char *password)
{
	FILE *fp;
	char *line = NULL;
	size_t len = 0;
	ssize_t read;
	bool user_found = false;

	fp = fopen("../users.txt", "r");
	if (fp == NULL)
	{
		exit(EXIT_FAILURE);
	}

	char *_email = "";
	char *_password = "";
	while ((read = getline(&line, &len, fp)) != -1)
	{
		// Obtenemos el email
		_email = strtok(line, " ");

		// Si se ha encontrado el email...
		if (strcmp(_email, email) == 0)
		{
			// Obtenemos la contraseña
			_password = strtok(NULL, "\n");

			// Observamos si las contraseñas coinciden
			if (strcmp(_password, password) == 0)
			{
				user_found = true;
			}

			// Como cada email es unico en el sistema, ya no buscamos más
			break;
		}
	}

	fclose(fp);
	if (line)
		free(line);

	return user_found;
}

// Funcion encargada de validar el inicio de sesion en nuestro formulario web
void validate_login(char *inputs_values, int socket_fd)
{
	// Obtenemos los campos email y password
	char *email, *password;

	char *token = strtok(inputs_values, "=");
	while (token != NULL)
	{
		if (strcmp(token, "email") == 0)
		{
			email = strtok(NULL, "&");
			// Convertimos el '%40' del email en una '@'
			email = replace_substring(email, "%40", "@");
		}

		if (strcmp(token, "password") == 0)
			password = strtok(NULL, "&");

		token = strtok(NULL, "=");
	}

	// Verificamos que el formato del correo sea correcto y exista ese usuario
	bool user_exists = (match(email, "(\\w+)(\\.|_)?(\\w*)@(\\w+)(\\.(\\w+))+") && search_user(email, password));
	free(email);

	// Informamos al usuario
	char *data = (user_exists)
					 ? "<html>\n\n"
					   "<head>\n"
					   "\t<link rel='icon' type='image/ico' href='/favicon.ico'>\n"
					   "\t<link rel='stylesheet' type='text/css' href='/styles.css'>\n"
					   "\t<meta charset='UTF-8'>\n"
					   "\t<title>Servicios Telemáticos</title>\n"
					   "</head>\n\n"
					   "<body>\n"
					   "\t<div>\n"
					   "\t\t<h1>BIENVENIDO</h1>\n"
					   "\t</div>\n"
					   "</body>\n\n"
					   "</html>"
					 : "<html>\n\n"
					   "<head>\n"
					   "\t<link rel='icon' type='image/ico' href='/favicon.ico'>\n"
					   "\t<link rel='stylesheet' type='text/css' href='/styles.css'>\n"
					   "\t<meta charset='UTF-8'>\n"
					   "\t<title>Servicios Telemáticos</title>\n"
					   "</head>\n\n"
					   "<body>\n"
					   "\t<div>\n"
					   "\t\t<h1>ERROR AL LOGEARTE</h1>\n"
					   "\t\t<h2>Verifique que haya introducido los datos correctamente</h2>\n"
					   "\t</div>\n"
					   "</body>\n\n"
					   "</html>";

	// Construimos la cabecera de la respuesta HTTP
	char *header = build_header(200, "OK", "text/html", strlen(data));

	// Construimos la respuesta
	char *response;
	asprintf(&response, "%s"
					  "%s",
			header, data);

	/*
		Enviamos la respuesta en bloques de 8K ya que, en cualquier otro caso,
		el código HTML podría superar los 8K de tamaño.
	*/
	send_response_in_8k_blocks(response, socket_fd);

	free(header);
	free(response);
}

// Función encargada de procesar cada petición web
void process_web_request(int socket_fd)
{
	// Definimos buffer y variables necesarias para leer las peticiones
	char buffer[BUFSIZE] = {0};
	char *method, *url, *version;

	// Leemos los datos
	read(socket_fd, buffer, BUFSIZE);

	// Comprobación de errores de lectura
	// Es preciso hacer una copia del buffer ya que strtok es una función destructiva
	char *_buffer = (char *)malloc(BUFSIZE);
	strcpy(_buffer, buffer);

	bool check_request_line = true;
	char *token_line = strtok(_buffer, "\r\n");
	while (token_line != NULL)
	{
		// Tratamos la linea de solicitud
		if (check_request_line)
		{
			// Obtenemos los campos de la linea de solicitud y los validamos
			char *field;
			int field_position = 0;
			while ((field = strsep(&token_line, " ")) != NULL)
			{
				field_position += 1;
				switch (field_position)
				{
				case 1:
					// En esta posición se encuentra el método HTTP
					method = strdup(field);

					// Si el método no es ni GET ni POST se produce un error
					if (strcmp(method, "GET") != 0 && strcmp(method, "POST") != 0)
					{
						debug(METHODNOTALLOWED, "Método no permitido", "El servidor solo permite los métodos GET y POST", socket_fd);
					}
					break;

				case 2:
					// En esta posición se encuentra la URL solicitada
					url = strdup(field);

					// Ignoramos el caracter '/' de la URL
					memmove(url, url + 1, strlen(url) + 1);
					break;

				case 3:
					// En esta posición se encuentra la versión de HTTP
					version = strdup(field);

					// Si la versión del protocolo no es la 1.1 se produce un error
					if (strcmp(version, "HTTP/1.1") != 0)
					{
						debug(HTTPVERSIONNOTSUPPORTED, "Versión HTTP no soportada", "Solo se soporta la versión 1.1 de HTTP", socket_fd);
					}
					break;
				default:
					break;
				}
			}

			// Si no hay 3 campos en la linea de solicitud, su formato no es correcto
			if (field_position != 3)
			{
				debug(BADREQUEST, "El formato de la línea de solicitud no es válido", "Formato:\t 'METHOD URL HTTP_VERSION\\r\\n'", socket_fd);
			}
			check_request_line = false;
		}
		else
		{
			// Tratamos las lineas de cabecera
			if (!match(token_line, "[a-zA-Z-]+[:] .*"))
			{
				// Puede tratarse del segmento de datos de una petición tipo POST
				if (strcmp(method, "POST") == 0 && match(token_line, "^(\\w+=.+&)*\\w+=.+$"))
				{
					char *data = token_line;
					validate_login(data, socket_fd);

					// Evitamos fuga de memoria
					free(method);
					free(url);
					free(version);
					free(_buffer);

					return;
				}
				debug(BADREQUEST, "El formato de las cabeceras no es válido", "Formato:\t 'key: value\\r\\n'", socket_fd);
			}
		}

		token_line = strtok(NULL, "\r\n");
	}
	free(_buffer);

	/*
		Prohibimos el acceso ilegal a directorios superiores de la jerarquia 
		de directorios del sistema
	*/
	if (match(url, "^[.].*$") || match(url, "^/.*$"))
	{
		debug(FORBIDDEN, "Acceso prohibido", "El servidor entiende la solicitud pero no la autoriza", socket_fd);
	}

	// Cuando el GET es para / devolvemos /index.html
	if (strcmp(url, "") == 0)
	{
		url = strdup("index.html");
	}

	// Si el fichero solicitado es un directorio existente...
	if (is_directory(url))
	{
		/*
			Si la URL no tiene 'Trailing Slash', redirigimos a una URL
			que si lo tenga para evitar contenido duplicado.
		*/
		if (!match(url, "^(.)*/$"))
		{
			redirect_trailing_slash(url, socket_fd);
		}

		// Listamos el contenido del directorio
		list_directory_content(url, socket_fd);
	}
	else
	{
		/*
			En caso de que no sea un directorio existente,
			comprobamos que el fichero dado al menos exista... 
		*/
		FILE *file;
		char *filetype;
		if (!(file = fopen(url, "rb")))
		{
			debug(NOTFOUND, "La URL solicitada no se encuentra en el servidor", url, socket_fd);
		}
		/*
			En caso de que exista, evaluamos el tipo de fichero que se está solicitando
			y si no está soportado devolvemos el error correspondiente
		*/
		filetype = get_file_type(url);
		if (filetype == NULL)
		{
			fclose(file);
			debug(UNSUPPORTEDMEDIATYPE, "Tipo de archivo no soportado", "Pruebe con otro tipo de fichero", socket_fd);
		}

		/*
			En caso de que el fichero sea soportado, exista, etc. se envia el fichero con la cabecera
			correspondiente, y el envio del fichero se hace en bloques de un máximo de 8kB
		*/

		// Obtenemos la longitud del archivo solicitado
		unsigned long length = fsize(url);

		// Construimos la cabecera de la respuesta HTTP
		char *header = build_header(OK, "OK", filetype, length);

		write(socket_fd, header, strlen(header)); // Enviamos el header
		free(header);

		// Enviamos el fichero solicitado en bloques de 8K
		size_t len = 0;
		unsigned char data[BUFSIZE];
		while (!feof(file))
		{
			len = fread(data, sizeof(char), BUFSIZE, file);
			send(socket_fd, data, len, 0);
		}

		debug(LOG, "Archivo enviado", url, socket_fd);

		fclose(file);
	}

	// Evitamos fuga de memoria
	free(method);
	free(url);
	free(version);
}
