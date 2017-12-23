#include "main.h"
#include "client.h"
#include "server.h"

void server_init(Server *server) {
	server->fd = socket(AF_INET, SOCK_STREAM, 0);

	if(server->fd < 0)
		error("socket");

	int on = 1;

	if(setsockopt(server->fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0)
		error("setsockopt");

	struct sockaddr_in servaddr;

	memset(&servaddr, 0, sizeof(servaddr));

	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(8122);

	if(bind(server->fd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0)
		error("bind");

	if(listen(server->fd, SOMAXCONN) < 0)
		error("listen");
}

void server_tick(Server *server, Client **client_first) {
	fd_set rset;

	FD_SET(server->fd, &rset);
	int maxfd = server->fd;

	for(Client *client = *client_first; client != NULL; client = client->next) {
		FD_SET(client->fd, &rset);
		maxfd = max(maxfd, client->fd);
	}

	select(maxfd + 1, &rset, NULL, NULL, NULL);

	if(FD_ISSET(server->fd, &rset)) {
		printf("accepting new socket\n");

		Client *client = calloc(1, sizeof(Client));

		if(client == NULL)
			error("calloc");

		client->fd = accept(server->fd, (struct sockaddr *) &client->address, &client->address_length);

		if(client->fd < 0)
			error("accept");

		client_init(client);

		Client **client_node = client_first;

		while(*client_node != NULL) {
			client_node = &(*client_node)->next;
		}

		*client_node = client;
	}

	for(Client **client_node = client_first; *client_node != NULL; client_node = &(*client_node)->next) {
		printf("checking %d\n", (*client_node)->fd);

		if(FD_ISSET((*client_node)->fd, &rset)) {
			if(!client_handle(*client_node)) {
				printf("%d disconnected\n", (*client_node)->fd);
				Client *client = *client_node;
				*client_node = client->next;
				free(client);

				if(*client_node == NULL)
					return;
			}
		}
	}
}
