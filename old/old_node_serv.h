
#ifndef NODE_SRVER
#define NODE_SERVER
	#define DEFAULT_PORT 3145
	#define PROMPT "[%s@%s:%d] "
	#define MESSAGE "[%s@%s:%d] %s\n"
	#define ERROR_MESSAGE "[error@%s:%d] at %s:%d	%s\n"

	/* Starts a server on the default port */
	int start_server();

	/* Starts a server on a given port */
	int start_server_on(int port);

	/* Stops the server */
	int stop_server();

	/* Forces the server to stop */
	void force_stop();

	/* Restarts the server */
	int restart_server();

	/* Forces a restart */
	void force_restart();

	/* Runs the console */
	int run_console();

	/* Displays help topics */
	void display_help();
#endif
