//
// Created by Nathan Jennes on 10/26/22.
//

#include "NetworkByteBuffer.h"

#include <stdexcept>
#include <memory>
#include <unistd.h>
#include <cstring>
#include <netinet/in.h>

NetworkByteBuffer::NetworkByteBuffer()
	:_data(nullptr), _capacity(0), _size(0), _cursor(0)
{
}

NetworkByteBuffer::NetworkByteBuffer(size_t capacity)
	:_data(nullptr), _capacity(capacity), _size(0), _cursor(0)
{
	_data = new uint8_t[_capacity];
	memset(_data, 0, _capacity);
}

NetworkByteBuffer::NetworkByteBuffer(const NetworkByteBuffer &other)
	:_data(nullptr), _capacity(other._capacity), _size(other._size), _cursor(other._cursor)
{
	_data = new uint8_t[_capacity];
	memmove(_data, other._data, other._capacity);
}

NetworkByteBuffer::~NetworkByteBuffer()
{
	delete[] _data;
}

NetworkByteBuffer &NetworkByteBuffer::operator=(const NetworkByteBuffer &other)
{
	if (this == &other)
		return *this;

	if (other._capacity > _capacity) {
		delete[] _data;
		_data = new uint8_t[other._capacity];
		memset(_data, 0, other._capacity);
	}
	_capacity = other._capacity;
	_size = other._size;
	_cursor = other._cursor;

	memmove(_data, other._data, other._capacity);

	return *this;
}

void NetworkByteBuffer::reserve(size_t capacity)
{
	if (capacity > _capacity) {
		uint8_t *new_data = new uint8_t[capacity];
		memset(_data, 0, capacity);
		memmove(new_data, _data, _capacity);
		delete[] _data;
		_data = new_data;
		_capacity = capacity;
	}
}

void NetworkByteBuffer::add_capacity(size_t capacity)
{
	uint8_t *new_data = new uint8_t[_capacity + capacity];
	memset(_data, 0, _capacity + capacity);
	memmove(new_data, _data, _capacity);
	delete[] _data;
	_data = new_data;
	_capacity += capacity;
}

void NetworkByteBuffer::seek_start()
{
	_cursor = 0;
}

int8_t NetworkByteBuffer::read_i8()
{
	if (_capacity - _cursor < 1)
		throw std::out_of_range("Couldn't read 1 byte from network byte buffer");
	int8_t val = *reinterpret_cast<int8_t *>(&_data[_cursor]);
	_cursor++;
	return val;
}

uint8_t NetworkByteBuffer::read_u8()
{
	if (_capacity - _cursor < 1)
		throw std::out_of_range("Couldn't read 1 byte from network byte buffer");
	uint8_t val = *reinterpret_cast<uint8_t *>(&_data[_cursor]);
	_cursor++;
	return val;
}

int16_t NetworkByteBuffer::read_i16()
{
	if (_capacity - _cursor < 2)
		throw std::out_of_range("Couldn't read 2 byte from network byte buffer");
	uint16_t value = ntohs(*reinterpret_cast<uint16_t *>(&_data[_cursor]));
	int16_t val = *reinterpret_cast<int16_t *>(&value);
	_cursor += 2;
	return val;
}

uint16_t NetworkByteBuffer::read_u16()
{
	if (_capacity - _cursor < 2)
		throw std::out_of_range("Couldn't read 2 byte from network byte buffer");
	uint16_t val = ntohs(*reinterpret_cast<uint16_t *>(&_data[_cursor]));
	_cursor += 2;
	return val;
}

int32_t NetworkByteBuffer::read_i32()
{
	if (_capacity - _cursor < 4)
		throw std::out_of_range("Couldn't read 4 byte from network byte buffer");
	uint32_t value = ntohl(*reinterpret_cast<uint32_t *>(&_data[_cursor]));
	int32_t val = *reinterpret_cast<int32_t *>(&value);
	_cursor += 4;
	return val;
}

uint32_t NetworkByteBuffer::read_u32()
{
	if (_capacity - _cursor < 4)
		throw std::out_of_range("Couldn't read 4 byte from network byte buffer");
	uint32_t val = ntohl(*reinterpret_cast<uint32_t *>(&_data[_cursor]));
	_cursor += 4;
	return val;
}

void NetworkByteBuffer::write_i8(int8_t value)
{
	if (_capacity < _size + 1)
		grow(_capacity + 1);
	int8_t *ptr = reinterpret_cast<int8_t *>(&_data[_size]);
	*ptr = value;
	_size += 1;
}

void NetworkByteBuffer::write_u8(uint8_t value)
{
	if (_capacity < _size + 1)
		grow(_capacity + 1);
	uint8_t *ptr = reinterpret_cast<uint8_t *>(&_data[_size]);
	*ptr = value;
	_size += 1;
}

void NetworkByteBuffer::write_i16(int16_t value)
{
	if (_capacity < _size + 2)
		grow(_capacity + 2);
	uint16_t *ptr = reinterpret_cast<uint16_t *>(&_data[_size]);
	uint16_t unsigned_value = *reinterpret_cast<uint16_t *>(&value);
	*ptr = htons(unsigned_value);
	_size += 2;
}

#include <iostream>
void NetworkByteBuffer::write_u16(uint16_t value)
{
	if (_capacity < _size + 2)
		grow(_capacity + 2);
	uint16_t *ptr = reinterpret_cast<uint16_t *>(&_data[_size]);
	*ptr = htons(value);
	_size += 2;
}

void NetworkByteBuffer::write_i32(int32_t value)
{
	if (_capacity < _size + 4)
		grow(_capacity + 4);
	uint32_t *ptr = reinterpret_cast<uint32_t *>(&_data[_size]);
	uint32_t unsigned_value = *reinterpret_cast<uint32_t *>(&value);
	*ptr = htonl(unsigned_value);
	_size += 4;
}

void NetworkByteBuffer::write_u32(uint32_t value)
{
	if (_capacity < _size + 4)
		grow(_capacity + 4);
	uint32_t *ptr = reinterpret_cast<uint32_t *>(&_data[_size]);
	*ptr = htonl(value);
	_size += 4;
}

void NetworkByteBuffer::grow(size_t new_capacity)
{
	uint8_t *new_data = new uint8_t[new_capacity];
	memset(new_data, 0, new_capacity);
	memmove(new_data, _data, _capacity);
	delete[] _data;
	_data = new_data;
	_capacity = new_capacity;
}

ssize_t NetworkByteBuffer::read(int fildes, size_t nbyte)
{
	if (_capacity < _size + nbyte)
		grow(_capacity + nbyte);

	ssize_t bytes_read = ::read(fildes, &_data[_size], nbyte);
	if (bytes_read > 0)
		_size += bytes_read;

	return bytes_read;
}
