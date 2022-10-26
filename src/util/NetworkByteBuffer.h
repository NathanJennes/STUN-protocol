//
// Created by Nathan Jennes on 10/26/22.
//

#ifndef NETWORK_BYTE_BUFFER_H
#define NETWORK_BYTE_BUFFER_H

#include <cstdint>
#include <cstddef>

class NetworkByteBuffer
{
public:
	NetworkByteBuffer();
	NetworkByteBuffer(size_t capacity);
	NetworkByteBuffer(const NetworkByteBuffer &other);
	~NetworkByteBuffer();
	NetworkByteBuffer &operator=(const NetworkByteBuffer &other);

	void reserve(size_t capacity);
	void add_capacity(size_t capacity);

	void seek_start();

	uint8_t *data() const;

	int8_t read_i8();
	uint8_t read_u8();
	int16_t read_i16();
	uint16_t read_u16();
	int32_t read_i32();
	uint32_t read_u32();
	int64_t read_i64();
	uint64_t read_u64();

private:
	uint8_t *_data;
	size_t _capacity;
	size_t _cursor;
};

#endif //NETWORK_BYTE_BUFFER_H