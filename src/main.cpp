//
// Created by Nathan Jennes on 10/25/22.
//

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <unistd.h>
#include <iostream>
#include <random>

#include "StunResponse.h"
#include "StunBindRequest.h"

#define STUN_PACKET_LEN 1024

int sock;
struct sockaddr_in address{};

StunResponse send_stun_request(const StunBindRequest &request)
{
	(void)request;
	uint8_t message_header[STUN_PACKET_LEN];
	memset(message_header, 0, STUN_PACKET_LEN);

	// Message type (0x0001 for bind request)
	message_header[0] = 0;
	message_header[1] = 1;

	// Message length (0 for no attributes)
	message_header[2] = 0;
	message_header[3] = 0;

	// Message id
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<uint32_t> rand_uint;

	uint32_t *message_id = reinterpret_cast<uint32_t *>(&message_header[4]);
	message_id[0] = rand_uint(rng);
	message_id[1] = rand_uint(rng);
	message_id[2] = rand_uint(rng);
	message_id[3] = rand_uint(rng);
	std::cout << "Sending message id: " << message_id[0] << message_id[1] << message_id[2] << message_id[3] << std::endl;

	if (write(sock, message_header, 20) == -1) {
		std::cout << "Error when writing to the socket" << std::endl;
		close(sock);
		exit(1);
	}
	std::cout << "sent !" << std::endl;

	ssize_t bytes_read = read(sock, message_header, STUN_PACKET_LEN);
	if (bytes_read == -1) {
		std::cout << "Error while reading message header from socket" << std::endl;
	}
	std::cout << "Read " << bytes_read << " bytes" << std::endl;
	uint8_t tmp = message_header[0];
	message_header[0] = message_header[1];
	message_header[1] = tmp;

	uint16_t response_type = *reinterpret_cast<uint16_t *>(message_header);
	std::cout << "Message response is of type: " << std::hex << response_type << std::dec << std::endl;

	tmp = message_header[2];
	message_header[2] = message_header[3];
	message_header[3] = tmp;

	uint16_t response_length = *reinterpret_cast<uint16_t *>(&message_header[2]);
	std::cout << "Message response is of length: " << response_length << std::endl;

	uint32_t *response_id = reinterpret_cast<uint32_t *>(&message_header[4]);
	std::cout << "Message response id: " << response_id[0] << response_id[1] << response_id[2] << response_id[3] << std::endl;

	uint8_t *response_data = &message_header[20];

	while (response_length) {
		tmp = response_data[0];
		response_data[0] = response_data[1];
		response_data[1] = tmp;
		tmp = response_data[2];
		response_data[2] = response_data[3];
		response_data[3] = tmp;
		uint16_t attrib_type = *reinterpret_cast<uint16_t *>(response_data);
		uint16_t attrib_len = *reinterpret_cast<uint16_t *>(&response_data[2]);

		std::cout << "Attribute of type: " << std::hex << attrib_type << std::dec << std::endl;
		std::cout << "Attribute len: " << attrib_len << std::endl;

		// Find sender ip and port
		if (attrib_type == 1) {
			tmp = response_data[6];
			response_data[6] = response_data[7];
			response_data[7] = tmp;
			tmp = response_data[8];
			response_data[8] = response_data[11];
			response_data[11] = tmp;
			tmp = response_data[9];
			response_data[9] = response_data[10];
			response_data[10] = tmp;

			uint16_t port = *reinterpret_cast<uint16_t *>(&response_data[6]);
			uint32_t ip = *reinterpret_cast<uint16_t *>(&response_data[8]);
			char str[INET_ADDRSTRLEN];
			bzero(str, INET_ADDRSTRLEN);
			inet_ntop(AF_INET, &ip, str, INET_ADDRSTRLEN);

			std::cout << "Sender port: " << port << std::endl;
			std::cout << "Sender ip: " << str << std::endl;
		}

		short a = 0x1234;

		uint16_t total_len = 4 + attrib_len;
		if (total_len >= response_length)
			break ;
		response_length -= total_len;
		response_data += total_len;
	}

	StunResponse response;
	return response;
}

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

	StunBindRequest request;
	StunResponse response = send_stun_request(request);

	close(sock);
	return 0;
}
