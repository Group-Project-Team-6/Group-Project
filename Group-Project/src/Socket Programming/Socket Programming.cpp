// Socket Programming.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>


int WSAStartup(
	WORD version,
	WSADATA *data
);

int getaddrinfo(
	const char				*name,
	const char				*service,
	const struct addrinfo   *hints,
	struct addrinfo			*result
);

typedef struct addrinfo {
	int					ai_flags;
	int					ai_family;
	int					ai_socktype;
	int					ai_protocol;
	size_t				ai_addrlen;
	char*				ai_canonname;
	struct sockaddr*	ai_addr;
	struct addrinfo*	ai_next;
};

SOCKET socket(
	int af,
	int type,
	int protocol
);

int bind(
	SOCKET s,
	const struct sockaddr *name,
	int namelen
);

int connect(
	SOCKET s,
	const struct sockaddr *name,
	int namelen
);

int listen(
	SOCKET s,
	int backlog
);

SOCKET accept(
	SOCKET s,
	struct sockaddr *addr,
	int *addrlen
);

int send(
	SOCKET s,
	const char *buf,
	int len,
	int flags
);

int recv(
	SOCKET s,
	char *buf,
	int len,
	int flags
);

int closesocket(
	SOCKET s
);

int WSAGetLastError(void);

int WSACleanup(void);

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
