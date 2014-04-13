#ifndef NODE_SERV
#define NODE_SERV;
	#include <sys/socket.h>
	#define DEFAULT_PORT 3142
	struct list_node {
		int sock_fid;
		struct sockaddr* address;
		socklen_t addres_len;
		struct list_node* next;
	};

	int start(int port);
	int stop();
	int restart();

	void get_serv_name(char *serv_name);
	int get_serv_port();

	int listen_for_connections();
	int accept_connection();
	int send_peers(int sock_fid);

	int get_peers(char *peers);
	int get_peer(char *peer, int index);
	int peers_length();

	int add_peer(int sock_fid, struct sockaddr* address, socklen_t address_len);
	int remove_peer(int sock_fid);
#endif
