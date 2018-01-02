#include "main.h"
#include "client.h"
#include "server.h"
#include "dict.h"

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

	server->recv_buffer = malloc(server->recv_buffer_size = 1024 * 1024);

	if(server->recv_buffer == NULL)
		error("malloc");

	dict_init(&server->dict, 1024 * 1024);
}

void server_tick(Server *server) {
	fd_set rset, wset;

	FD_SET(server->fd, &rset);
	int maxfd = server->fd;

	// build fd sets
	for(Client *client = server->client; client != NULL; client = client->next) {
		FD_SET(client->fd, &rset);

		if(client->output.length > 0) {
			FD_SET(client->fd, &wset);
		}

		maxfd = max(maxfd, client->fd);
	}

	// wait for any descriptors to be readable or writable
	select(maxfd + 1, &rset, &wset, NULL, NULL);

	// check if their is a new client
	if(FD_ISSET(server->fd, &rset)) {
		Client *client = calloc(1, sizeof(Client));

		if(client == NULL)
			error("calloc");

		client->fd = accept(server->fd, (struct sockaddr *) &client->address, &client->address_length);

		fcntl(client->fd, F_SETFL, O_NONBLOCK);

		if(client->fd < 0)
			error("accept");

		client_init(client);

		Client **client_node = &server->client;

		// cycle up linked list until there's a space
		while(*client_node != NULL) {
			client_node = &(*client_node)->next;
		}

		*client_node = client;
	}

	// for each client in the linked list
	for(Client **client_node = &server->client; *client_node != NULL; client_node = &(*client_node)->next) {
		// if client is readable
		if(FD_ISSET((*client_node)->fd, &rset)) {
			// run the client handler, and check if the client is still connected
			if(!client_handle(*client_node, &server->dict, server->recv_buffer, server->recv_buffer_size)) {
				// replace the current client with the next one
				Client *client = *client_node;
				*client_node = client->next;

				client_free(client);
				free(client);

				// if we're now on the last client, end the loop as (NULL)->next will fail
				if(*client_node == NULL)
					return;
			}
		}

		// if client is writable
		if(FD_ISSET((*client_node)->fd, &wset)) {
			ssize_t result = send((*client_node)->fd, (*client_node)->output.data, (*client_node)->output.length, 0);

			if(result > 0)
				buffer_ltrim(&(*client_node)->output, result);
		}
	}
}
