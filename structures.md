# Core structures in socket programming

```c
/* struct sockaddr_in is a core structure in socket programming used for IPv4 addresses. This structure is used in userspace, passed to functions like bind(), connect(), sendto(), recvfrom(), etc.
*/

struct sockaddr_in {
    sa_family_t     sin_family;         // address family: AF_INET
    in_port_t       sin_port;           // port number (network byte order)
    struct in_addr  sin_addr;           // IP address
    struct in_addr  sin_zero[8];        // unused (padding)
};

/* On 64-bit Linux, the total size is 16 bytes. */
```

## Breakdown of Common Prefixes and Suffixes
### sa_ -- "Socket Address"
```c
Example: sa_family_t, struct sockaddr
Meaning: Refers to generic socket address types
sa_family_t is used in struct sockaddr, which is the base type for all socket address types (IPv4, IPv6, etc.)
```

### in_ -- "Internet (IPv4)"




|structure name|passed to function|
|---|---|
|`struct sockaddr_in`|`bind()`, `connect()`, `sendto()`, `recvfrom()`|

