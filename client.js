const net = require("net");

const socket = net.connect(8122);

function set(...args) {
	const key = Buffer.from(args[0]);
	const value = Buffer.from(args[1]);

	const header = Buffer.alloc(9);

	header.writeUInt8(1, 0);
	header.writeUInt32BE(key.length, 1);
	header.writeUInt32BE(value.length, 5);

	socket.write(Buffer.concat([
		header,
		key,
		value
	]));
}

setInterval(() => set("monty", "python"), 1000);
