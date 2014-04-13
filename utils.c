#include <stdio.h>
#include <errno.h>
#include "utils.h"
#include "node_server.h"

void message(char *user, char *server_name, int port, char *message) {
	printf("[%s@%s:%d] %s\n", user, server_name, port, message);
}

void trace(char *message) {
	char reason[256] = "Unknown";
	printf("rawr");
	switch(errno) {
		
	}

	char serv_name[256];
	get_serv_name(serv_name);

	fprintf(stderr, "[%s@%s:%d] %s\n", "error", serv_name, get_serv_port(), reason);
}
