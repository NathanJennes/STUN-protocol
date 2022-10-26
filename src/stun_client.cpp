//
// Created by Nathan Jennes on 10/26/22.
//

#include "stun_client.h"
#include "util/NetworkByteBuffer.h"
#include <random>
#include <iostream>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

namespace STUNClient {

StunResponse send(int sock, const StunBindRequest &request)
{
	(void)request;

	NetworkByteBuffer request_buffer;

	// Message type (0x0001 for bind request)
	request_buffer.write_u16(0x0001);
	// Message length (0 for no attributes)
	request_buffer.write_u16(0x0000);

	// Message id
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<uint32_t> rand_uint;
	request_buffer.write_u32(rand_uint(rng));
	request_buffer.write_u32(rand_uint(rng));
	request_buffer.write_u32(rand_uint(rng));
	request_buffer.write_u32(rand_uint(rng));

	std::cout << "Sending request_buffer of length " << request_buffer.size() << std::endl;

	if (write(sock, request_buffer.data(), request_buffer.size()) == -1) {
		std::cout << "Error when writing to the socket" << std::endl;
		close(sock);
		exit(1);
	}
	std::cout << "sent !" << std::endl;

	// Reading the response
	NetworkByteBuffer response_buffer;
	if (response_buffer.read(sock, STUN_PACKET_SIZE) == -1) {
		std::cout << "Error while reading message header from socket" << std::endl;
	}

	uint16_t response_type = response_buffer.read_u16();
	std::cout << "Message response is of type: " << std::hex << response_type << std::dec << std::endl;

	uint16_t response_length = response_buffer.read_u16();
	std::cout << "Message response is of length: " << response_length << std::endl;

	std::cout << "Message response id: " << response_buffer.read_u64() << response_buffer.read_u64() << std::endl;

	while (response_length) {
		uint16_t attrib_type = response_buffer.read_u16();
		uint16_t attrib_len = response_buffer.read_u16();

		std::cout << "Attribute of type: " << std::hex << attrib_type << std::dec << std::endl;
		std::cout << "Attribute len: " << attrib_len << std::endl;

		// Find sender ip and port
		if (attrib_type == 1) {
			response_buffer.read_u16(); // Skip 2 bytes
			uint16_t port = response_buffer.read_u16();
			uint32_t ip = htonl(response_buffer.read_u32());
			char str[INET_ADDRSTRLEN + 1];
			bzero(str, INET_ADDRSTRLEN + 1);
			inet_ntop(AF_INET, &ip, str, INET_ADDRSTRLEN);

			std::cout << "Sender port: " << port << std::endl;
			std::cout << "Sender ip: " << str << std::endl;
		}

		uint16_t total_len = 4 + attrib_len;
		if (total_len >= response_length)
			break ;
		response_length -= total_len;
	}

	StunResponse response;
	return response;
}

}
