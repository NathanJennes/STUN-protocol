//
// Created by Nathan Jennes on 10/25/22.
//

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <unistd.h>

#include "Stun/Response.h"
#include "Stun/BindRequest.h"
#include "Stun/stun_client.h"

int sock;
struct sockaddr_in address{};

int main()
{
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock == -1) {
		std::cout << "Couldn't create the socket" << std::endl;
		return 1;
	}

	address.sin_port = htons(19302);
	address.sin_family = AF_INET;
	std::cout << inet_pton(AF_INET, "64.233.163.127", &address.sin_addr) << std::endl;

	if (connect(sock, (const sockaddr *)&address, sizeof (address))) {
		std::cout << "Couldn't connect the socket to: " << "stun1.l.google.com:19302" << std::endl;
		close(sock);
		return 1;
	}

	Stun::BindRequest request;
	Stun::Response response = Stun::send(sock, request);

	close(sock);
	return 0;
}
