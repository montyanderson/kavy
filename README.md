# kavy
Networked in-memory key-value store, written in pure C.

## How does Kavy work?

Kavy runs on an event-loop architecture, which works as follows

* The server waits until any clients are readable or writable, checking each one
	* If there is input, the server reads and appends to `client->input`
	* If there's enough data to run commands it does so, appending the output to `client->output`
	* If there is data in `client->output`, the server sends as much as possible, shifting the sent data from the beginning of the output buffer

## What makes Kavy different from Redis?

* Binary protocol - speed is prioritised here, rather than compatibility
* Static hash table with a fixed amount of buckets
* Literally only two features - `set` and `get`
