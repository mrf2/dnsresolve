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
	char host_data[250];
	int lock = 0, i;
	strcpy(host_data, host);
	strcat(host_data, ".");

	for (i = 0; i < strlen(host_data); i++) {
		if (host_data[i] == '.') {
			*dns++ = i - lock;
			for (; lock < i; lock++) {
				*dns++ = host_data[lock];
			}
			lock++;
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

	dns->ra = 0;		// Recursion available no
	dns->z = 0;		// Reserved no
	dns->ad = 0;		// Authenticated data no
	dns->cd = 0;		// Checking disabled no
	dns->rcode = 0;		// Response code no

	dns->q_count = 0;	// Number of question entries set to 0
	dns->ans_count = 0;	// Number of answer is set to 0
	dns->auth_count = 0;	// Number of auth count is set to 0
	dns->add_count = 0;	// Add count set to 0

	qname = (unsigned char *) &buf[sizeof(struct DNS_HEADER)];
	encode_dns_name(qname, "google.com");

	struct QUESTION *qinfo = (struct QUESTION *) &buf[sizeof(struct DNS_HEADER) + strlen((const char *)qname) + 1];
	qinfo->qtype = htons(1);	// Type A
	qinfo->qclass = htons(1);	// Class IN

	int query_len = sizeof (struct DNS_HEADER)+strlen((const char *)qname) + 1 + sizeof (struct QUESTION);

	if (sendto(sockfd, buf, query_len, 0, (struct sockaddr *)&dest, sizeof(dest)) < 0) {
		perror("sendto failed");
	}

	printf("Sent DNS query for google.com\n");

	socklen_t len = sizeof(dest);
	int response_len = recvfrom(sockfd, buf, sizeof(buf), 0, (struct sockaddr *) &dest, &len);
	if (response_len < 0) {
		perror("recvfrom failed");
	} else {
		printf("Received %d btes\n", response_len);

		// Parse first answer (not robust)
		unsigned char *reader = &buf[query_len];
		reader += 10;	// skip naem pointer, type, class, TTL
		unsigned short rdlength = ntohs(*(unsigned short *) reader);
		reader += 2;

		printf("Resolved IP: %d.%d.%d.%d\n", reader[0], reader[1], reader[2], reader[3]);
	}

	close (sockfd);
	return 0;
}

