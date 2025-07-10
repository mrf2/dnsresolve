#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

int main()
{
	int sock = socket(AF_INET, SOCK_DGRAM, 0);
	struct sockaddr_in addr, client;
	char buffer[512];

	addr.sin_family = AF_INET;
	addr.sin_port = htons(12345);
	addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	bind(sock, (struct sockaddr *)&addr, sizeof(addr));

	while (1) {
		socklen_t len = sizeof(client);
		ssize_t n = recvfrom(sock, buffer, sizeof(buffer), 0,
				(struct sockaddr *)&client, &len);

		if (n > 0) {
			sendto(sock, buffer, n, 0, (struct sockaddr *) &client, len);
		}
	}

	close(sock);
	return 0;
}

