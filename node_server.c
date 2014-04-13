/*
 * node_server.c
 *
 * Source file for a node server.
 *
 * Alexander Brown
 * http://alexanderdbrown.com
 * 2011
 */

/* Standard includes */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

/* Socket includes */
#include <netinet/in.h>

/* Definition Header */
#include "node_server.h" 

#define SERVER_MESSAGE(msg) message("server", server_name, server_port, msg)

/* Linked list root node */
static struct list_node* peers_root = NULL;

static int server_fid;
static int server_port;
static char server_name[80] = "localhost";
static int running = 0;
int request_stop = 0;

void free_list_f(struct list_node *current) {
	if(current->next != NULL) {
		free_list_f(current->next);
	}

	free(current);
	current = NULL;
}

void free_list() {
	free_list_f(peers_root);
}

void run() {
	while(running) {
		if(request_stop == 1) {
			stop();
			break;
		}
		wait(0);
	}
}

int main(int argv, char **argc) {
	if(fork() == 0) {
		char line[256];
		while(printf("> "), gets(line) != NULL) {
			if(strcmp(line, "stop") == 0) {
				request_stop = 1;
			} else {
				printf("Unknown command `%s`\n", line);
			}
		}
	} else {
		if(start(DEFAULT_PORT) < 0) {
			exit(1);
		}
		run();
	}

	exit(0);
}

int start(int port) {
	int ret_code = -1;
	struct sockaddr_in address;

	server_port = port;

	server_fid = socket(AF_INET, SOCK_STREAM, 0);


	address.sin_family = AF_INET;
	address.sin_port = htons(port);
	address.sin_addr.s_addr = htonl(INADDR_ANY);

	ret_code = bind(server_fid, (struct sockaddr *) &address, sizeof(address));

	if(ret_code < 0) {
#ifdef TRACE
		trace("Failed to start server");
#else
		SERVER_MESSAGE("Failed to start.");
#endif
	} else {
		SERVER_MESSAGE("Started.");
	}
	running = 1;
	return ret_code;
}

int stop() {
	int ret_code = -1;

	ret_code = shutdown(server_fid, 2);

	if(ret_code < 0 && errno == ENOTCONN)
		ret_code = close(server_fid);

	if(ret_code < 0) {
#ifdef TRACE
		trace("Failed to stop server");
#else
		SERVER_MESSAGE("Failed to stop.");
#endif
	} else {
		SERVER_MESSAGE("Stopped.");
	}
	running = 0;
	return ret_code;
}

int restart() {
	int ret_code;
	SERVER_MESSAGE("Attempting to restart server");
	ret_code = stop();
	if(ret_code < 0) {
		return ret_code;
	}

	ret_code = start(server_port);
	return ret_code;
}

void get_serv_name(char *serv_name) {
	serv_name="localhost";
}

int get_serv_port() {
	return server_port;
}

int listen_for_connections() {
	return -1;
}

int accept_connection() {
	return -1;
}

int send_peers(int sock_fid) {
	return -1;
}

int get_peers(char *peers) {
	return -1;
}

int get_peer(char *peer, int index) {
	return -1;
}

int peers_length() {
	return -1;
}

int add_peer(int sock_fid, struct sockaddr* address, socklen_t addr_len) {
	return -1;
}

int remove_peer(int sock_fid) {
	return -1;
}
