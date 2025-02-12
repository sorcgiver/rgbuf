# RING BUFFER
### About
Current status: under development
Planned:
- implementation for C, without dynamic memory allocation. byte-by-byte and packet buffers
- implementation for C++, without allocating dynamic memory. byte-by-byte and packet buffers

**byte-by-byte**: data is written and read in bytes
**packet buffer**: data is written and read in batches. If a 5 byte packet is being written, then 5 bytes will be read at a time. It is worth considering that there is a header containing information about the package. its size varies depending on the size of the data

### Build
```sh
make
```

### Examples
```C
	uint8_t container[100];
	rgbuf_t buf;
	rgbuf_init(&buf, container, sizeof(container));

	// write message
	char* msg = "Hello world!";
	rgbuf_write(&buf, msg, strlen(msg)+1);

	// read message
	char recvbuf[100];
	rgbuf_read(&buf, recvbuf, sizeof(recvbuf));
	printf("msg: %s\n", recvbuf);
```
