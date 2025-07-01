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


