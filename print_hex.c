#include <stdio.h>
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
	unsigned char data[] = "Example"
				"\x00\x01\xff\x10"
				"Data";
	print_hex(data, sizeof(data));

	return 0;
}

