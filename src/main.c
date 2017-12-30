#include "main.h"
#include "client.h"
#include "server.h"

void error(char *func) {
	perror(func);
	exit(1);
}

int main(int argc, char **argv) {
	Server server;

	memset(&server, 0, sizeof(Server));
	server_init(&server);

	for(;;) {
		server_tick(&server);
	}
}
