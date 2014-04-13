#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "node_server.h"

static int server_status = -1;
static int server_fid;
static int server_port;
static int running;
static char* user;
static char server_name[INET_ADDRSTRLEN] = "unknown";

int error(char *message, int line) {
	fprintf(stderr, ERROR_MESSAGE, server_name, server_port, __FILE__,  line, message);
	if(server_status > 0)
		force_stop();
	exit(1);
}

void socket_name() {
	struct sockaddr_in socket;
	int result = 0;
	int size;
	size = sizeof(socket);
	
	result = getsockname(server_fid, (struct sockaddr *) &socket, &size);

	if(result < 0) 
		error("Unable to get socket name",__LINE__);

	inet_ntop(AF_INET, &(socket.sin_addr), server_name, INET_ADDRSTRLEN);
}

int warn(char *message, int line) {
	fprintf(stderr, "W[%s:%d]	%s\n", __FILE__, line, message);
}

int main(int argc, char **argv) {
	pid_t process_id;
	if(argc < 2) {
		server_status = start_server();
	} else {
		//TODO Get port number from argv
		int port = -1;
		server_status = start_server_on(port);
	}

	if(server_status < 0) {
		error("Failed to start server.",__LINE__);
	}

	while(running) {
		run_console();
	}

	exit(0);
}

int run_console() {
	char line[256];
	printf(PROMPT, user, server_name, server_port);
	gets(line);	
	if(strcmp(line, "stop") == 0) {
		stop_server();
		return 0;
	} else if(strcmp(line, "help") == 0) {
		display_help();
	} else {
		printf("%s: Unrecognised command.\nUse `help` to display a list of commands\n", line);
	}
	return 1;
}

void display_help() {
	printf("stop	Stops the server.\n");
}

int start_server() {
	running = 1;
	return start_server_on(DEFAULT_PORT);
}

int start_server_on(int port) {
	struct sockaddr_in socket_address;
	int ret_code;
	if(port < 0 || port > 10000)
		error("Specified port is invalid.",__LINE__);

	server_fid = socket(AF_INET, SOCK_STREAM, 0);

	socket_address.sin_family = AF_INET;
	socket_address.sin_port = htons(port);
	socket_address.sin_addr.s_addr = htonl(INADDR_ANY);
	
	server_port = port;
	ret_code = bind(server_fid, (struct sockaddr *) &socket_address, sizeof(socket_address));
	socket_name();
	printf(MESSAGE, "server", server_name, server_port, "Started.");
	return ret_code;
}

int stop_server_error(int ret_code) {
	if(ret_code < 0) {
		char *reason;

		switch(errno) {
			case EBADF:
				reason = "Failed to close socket: File descriptor is not a valid socket.";
				break;
			case EINVAL:
				reason = "Failed to close socket: Invalid argument.";
				break;
			case ENOTCONN:
				// If not connected; close the file descriptor instead.
				ret_code = close(server_fid);
				if(ret_code >= 0) {
					running = 0;
					return ret_code;
				} 
				reason = "Failed to close socket: Socket is not connected and could not close the file descriptor.";
				break;
			case ENOTSOCK:
				reason = "Failed to close socket: File descriptor is not a socket.";
				break;
			case ENOBUFS:
				reason = "Failed to close socket: Insufficient resources available to perform this operation.";
				break;
			case ENOSR:
				reason = "Failed to close socket: Insufficient STREAMs available to perform this operation.";
				break;
			default:
				reason = "Failed to close socket.";
		}

		error(reason, __LINE__);
	}

	return ret_code;
}

int stop_server() {
	int ret_code = -1;
	
	ret_code = shutdown(server_fid, SHUT_RDWR);
	ret_code = stop_server_error(ret_code);
	printf(MESSAGE, "server", server_name, server_port, "Stopped.");
	running = 0;
	return ret_code;
}

void force_stop() {

}

int restart_server() {
	return -1;
}

void force_restart() {

}
