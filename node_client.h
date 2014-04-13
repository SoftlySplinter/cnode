#ifndef NODE_CLIENT
#define NODE_CLIENT

	int connect_to(struct sockaddr *address, socklen_t len);
	int add_connection(int sock_fid);

	int disconnect_from(int sock_fid);
	int remove_connection(int sock_fid);
	
	int check_connection(int sock_fid);

	int request_peers(int sock_fid);
	int parse_peers();

	

#endif
