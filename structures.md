# Core structures in socket programming

```c
____________________________________________________________________netinet/in.h
/* 
 struct sockaddr_in is a core structure in socket programming used for IPv4 addresses. 
 This structure is used in userspace, passed to functions like bind(), connect(), 
 sendto(), recvfrom(), etc.
*/

struct sockaddr_in {
    sa_family_t     sin_family;         // address family: AF_INET
    in_port_t       sin_port;           // port number (network byte order)
    struct in_addr  sin_addr;           // IP address
    struct in_addr  sin_zero[8];        // unused (padding)
};

/* On 64-bit Linux, the total size is 16 bytes. */
____________________________________________________________________netinet/in.h

____________________________________________________________________netinet/in.h
/* Internet addresss structure */
struct in_addr {
    unsigned inst s_addr;       /* Network byte order (big-endian) */
};
____________________________________________________________________netinet/in.h


________________________________________________________________________netdb.h
struct hostent {
    char *h_name;           /* Official domain name of host */
    char **h_aliases;       /* Null-terminated array of domain names */
    int h_addrtype;         /* Host address type (AF_INET) */
    int h_length;           /* Length of an address, in bytes */
    char **h_addr_list;     /* Null-terminated array of in_addr structs */
________________________________________________________________________netdb.h
```
#### Why Store the scalar IP address in a structure?
***Storing a scalar address in a structure is an unfortunate artifcat from the early implementations of the socket interface. It would make more sence to define a scalar type for IP addresses, but it is too late to change now because of the enormous installed base of application.***

#### Assignment
```
struct in_addr ip;

inet_aton("192.168.10.201", &ip);   // Let the function do the conversion
```

## Breakdown of Common Prefixes and Suffixes
### `sa_` - "Socket Address"
 * Example: `sa_family_t`, `struct sockaddr`
 * Meaning: Refers to ***generic socket address*** types
 * `sa_family_t` is used in `struct sockaddr`, which is the base type for all socket address types (IPv4, IPv6, etc.)

### in_ - "Internet (IPv4)"
 * Example: `in_port_t`, `in_addr_t`, `struct sockaddr_in`
 * Meaning: Referes to **IPv4-specific** address components
 * `in` = short for ***Internet***, orgiginally from BSD Unix (e.g., `AF_INET`, `PF_INET`)
   * `in_port_t` = Inernet Port
   * `in_addr_t` = Internet address (IPv4 address)
   * `struct in_addr` = holds 32-bit IPv4 address
   * IPv6 uses `in6_` (like `in6_addr`)

### sin_ - "Socket Internet"
 * Example: `sin_port`, `sin_addr`
 * Meaning: This is part of `struct sockaddr_in`
   * `sin` is short for **sockaddr_in**
   * These prefixes just namespace the fields inside the struct

### `t` suffix - *typedef*  
 * Example: `in_port_t`, `sa_family_t`, `in_addr_t`
 * Meaning: Indicates it's a **type alias**, usually via `typedef`
 * Convention comes from `<stdint.h>` (like `uint8_t`, `size_t`, `time_t`)

## Core structures in use
|structure name|passed to function|
|---|---|
|`struct sockaddr_in`|`bind()`, `connect()`, `sendto()`, `recvfrom()`|

