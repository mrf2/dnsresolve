#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <ctype.h>

void print_hex(const unsigned char *buf, size_t len)
{
	for (size_t i = 0; i < len; i += 16) {
		printf("%04zx: ", i);
		for (size_t j = 0; j < 16; j++) {
			if ( i+j < len)
				printf("%02x ", buf[i+j]);
			else
				printf(" ");
		}
		printf(" ");
		for (size_t j = 0; j < 16; j++) {
			if (i + j < len)
				printf("%c", isprint(buf[i+j])? buf[i+j] : '.');
		}
		printf("\n");
	}
}

int main()
{
	int sock = socket(AF_INET, SOCK_DGRAM, 0);
	struct sockaddr_in server;
	char message[] = "Hello UDP!";
	unsigned char reply[512];

	
	server.sin_family = AF_INET;
	server.sin_port = htons(12345);
	inet_pton(AF_INET, "127.0.0.1", &server.sin_addr);

	sendto(sock, message, strlen(message), 0, (struct sockaddr *)&server, sizeof(server));

	socklen_t slen = sizeof(server);
	ssize_t n = recvfrom(sock, reply, sizeof(reply), 0, 
			(struct sockaddr *)&server, &slen);

	printf("Received %zd bytes: \n", n);
	print_hex(reply, n);

	close(sock);
	
	return 0;
}


