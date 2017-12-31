# kavy
Networked in-memory key-value store, written in pure C.

## How does Kavy work?

Kavy runs on an event-loop architecture, which works as follows

* The server [waits until any clients are readable or writable](https://github.com/montyanderson/kavy/blob/576d2b1acc307cc50aae65022ccfff08f7f6fa59/src/server.c#L55), checking each one
	* [If there is input](https://github.com/montyanderson/kavy/blob/576d2b1acc307cc50aae65022ccfff08f7f6fa59/src/server.c#L84), the server [reads and appends to `client->input`](https://github.com/montyanderson/kavy/blob/576d2b1acc307cc50aae65022ccfff08f7f6fa59/src/client.c#L91)
	* If there's enough data to run commands it does so, appending the output to `client->output`
	* If there is data in `client->output`, the server sends as much as possible, shifting the sent data from the beginning of the output buffer

## What makes Kavy different from Redis?

* Binary protocol - speed is prioritised here, rather than compatibility
* Static hash table with a fixed amount of buckets
* Literally only two features - `set` and `get`

## What's the performance like?

In my synthetic benchmarks, Kavy was up to *10 times faster* than Redis - likely due to it's pure simplicity and lack of logging and client-specific error handling.

## Protocol

All values are big-endian.

### `set`

#### Request
`uint8 1` `uint32 key_length` `uint32 value_length` `char[] key` `char[] value`

#### Response

`uint8 0`
or
`uint8 1` for fail/success respectively

### `get`

#### Request
`uint8 2` `uint32 key_length` `char key[]`

#### Response

`uint8 0`

or

`uint8 2` `uint32 value_length` `char[] value`
