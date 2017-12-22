#include "main.h"
#include "client.h"
#include "server.h"

void error(char *func) {
	perror(func);
	exit(1);
}

int main(int argc, char **argv) {
	Server server;
	server_init(&server);

	Client *client_first = NULL;

	for(;;) {
		server_tick(&server, &client_first);
	}
}
