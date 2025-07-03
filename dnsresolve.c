/* dnsresolve.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>

/* DNS header structure */
struct DNS_HEADER {
	unsigned short id;		// Identification

	unsigned char rd :1;		// Recursion desired
	unsigned char tc :1;		// Truncated message
	unsigned char aa :1;		// Authoritative answer
	unsigned char opcode :4;	// Purpose of message
	unsigned char qr :1;		// Query/Response flag

	unsigned char rcode :4;		// Respose code
	unsigned char cd :1;		// Checking disabled
	unsigned char ad :1;		// Authenticated data
	unsigned char z :1;		// Reserved
	unsigned char ra :1;		// Recursion available

	unsigned short q_count;		// Number of question entries
	unsigned short ans_count;
	unsigned short auth_count;
	unsigned short add_count;
};

// Question section format
struct QUESTION {
	unsigned short qtype;
	unsigned short qclass;
};

// Function to convert "google.com" to DNS name format
void encode_dns_name(unsigned char *dns, const char *host)
{
	int lock = 0, i;
	strcat((char *)host, ".");

	for (i = 0; i < strlen(host); i++) {
		if (host[i] == '.') {
			*dns++ = i - lock;
			for (; lock < i; lock++) {
				*dns++ = host[lock];
			}
			lock++
		}
	}
	*dns++ = '\0';
}

int main()
{
	unsigned char buf[65536], *qname;
	int sockfd;
	struct sockaddr_in dest;

	sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (sockfd < 0) {
		perror("Socket failed");
		exit (1);
	}

	dest.sin_family = AF_INET;
	dest.sin_port = htons (53);
	dest.sin_addr.s_addr = inet_addr("8.8.8.8");		// google DNS

	// Build DNS query
	struct DNS_HEADER *dns = (struct DNS_HEADER *) &buf;

	dns->id = (unsigned short) htons(getpid());
	dns->qr = 0;		// query
	dns->opcode = 0;	// standard query
	dns->aa = 0;		// Authoritative answer
	dns->tc = 0;		// Turncated message flase
	dns->rd = 1;		// Recursion desired yes
	
