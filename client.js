const net = require("net");

class Kavy {
	constructor() {
		this.socket = net.connect(8122);

		this.input = Buffer.from([]);
		this.callbacks = [];

		this.socket.on("data", data => {
			this.input = Buffer.concat([ this.input, data ]);
			this._handleInput();
		});
	}

	async set(...args) {
		const key = Buffer.from(args[0]);
		const value = Buffer.from(args[1]);

		const header = Buffer.alloc(9);

		header.writeUInt8(1, 0);
		header.writeUInt32BE(key.length, 1);
		header.writeUInt32BE(value.length, 5);

		await this.cmd(Buffer.concat([
			header,
			key,
			value
		]));
	}

	async get(...args) {
		const key = Buffer.from(args[0]);

		const header = Buffer.alloc(5);

		header.writeUInt8(2, 0);
		header.writeUInt32BE(key.length, 1);

		return await this.cmd(Buffer.concat([
			header,
			key
		]));
	}

	async cmd(input) {
		this.socket.write(input);

		return await new Promise((resolve, reject) => {
			this.callbacks.push({
				resolve,
				reject
			});
		});
	}

	_handleInput() {
		const handlers = [
			cb => {
				cb.reject(new Error("Key doesn't exist!"));
				this.input = this.input.slice(1);

				this.callbacks.shift();
			},
			cb => {
				cb.resolve();
				this.input = this.input.slice(1);

				this.callbacks.shift();
			},
			cb => {
				if(this.input.length < 5)
					return;

				const value_length = this.input.readUInt32BE(1);

				if(this.input.length < 5 + value_length)
					return;

				const value = this.input.slice(5, 5 + value_length);


				this.input = this.input.slice(5 + value_length);

				console.log(this.input, value);
				cb.resolve(value);

				this.callbacks.shift();
			}
		];

		console.log(`cmd: ${this.input[0]}`);
		handlers[this.input[0]](this.callbacks[0]);
	}
};

(async () => {

	const kavy = new Kavy();

	console.log("started");
	await kavy.set("monty", "python");
	console.log("finished");

	console.log("started");
	console.log((await kavy.get("monty")).toString());
	console.log("finished");

})().catch(e => console.log(e));
