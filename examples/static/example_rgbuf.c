#include <string.h>
#include <stdio.h>
#include "rgbuf.h"

int main(int argc, char *argv[])
{
	rgsize_t size;
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

	// write binary data
	uint8_t bin[] = {1, 2, 3, 4, 5};
	rgbuf_write(&buf, bin, sizeof(bin));

	// read binary data
	size = rgbuf_read(&buf, recvbuf, sizeof(recvbuf));
	printf("bin data: ");
	for (rgsize_t i = 0; i < size; i++) {
		printf("%u, ", (uint8_t)recvbuf[i]);
	}
	printf("\n");
}
