//
// Created by Nathan Jennes on 10/26/22.
//

#ifndef NETWORK_BYTE_BUFFER_H
#define NETWORK_BYTE_BUFFER_H

#include <cstdint>
#include <cstddef>
#include <cstdio>

// A ByteBuffer that holds bytes in network byte-order
class NetworkByteBuffer
{
public:
	NetworkByteBuffer();
	explicit NetworkByteBuffer(size_t capacity);
	NetworkByteBuffer(const NetworkByteBuffer &other);
	~NetworkByteBuffer();
	NetworkByteBuffer &operator=(const NetworkByteBuffer &other);

	void reserve(size_t capacity);
	void add_capacity(size_t capacity);

	void seek_start();

	ssize_t read(int fildes, size_t nbyte);

	int8_t read_i8();
	uint8_t read_u8();
	int16_t read_i16();
	uint16_t read_u16();
	int32_t read_i32();
	uint32_t read_u32();

	void write_i8(int8_t value);
	void write_u8(uint8_t value);
	void write_i16(int16_t value);
	void write_u16(uint16_t value);
	void write_i32(int32_t value);
	void write_u32(uint32_t value);

	uint8_t *data() const { return _data; };
	size_t size() const {return _size; };

private:
	uint8_t *_data;
	size_t _capacity;
	size_t _size;
	size_t _cursor;

	void grow(size_t new_capacity);
};

#endif //NETWORK_BYTE_BUFFER_H