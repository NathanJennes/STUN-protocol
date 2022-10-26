//
// Created by Nathan Jennes on 10/26/22.
//

#include "NetworkByteBuffer.h"

#include <stdexcept>
#include <memory>

NetworkByteBuffer::NetworkByteBuffer()
	:_data(nullptr), _capacity(0), _cursor(0)
{
}

NetworkByteBuffer::NetworkByteBuffer(size_t capacity)
	:_data(nullptr), _capacity(capacity), _cursor(0)
{
	_data = new uint8_t[_capacity];
}

NetworkByteBuffer::NetworkByteBuffer(const NetworkByteBuffer &other)
	:_data(nullptr), _capacity(other._capacity), _cursor(other._cursor)
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
	}
	_capacity = other._capacity;
	_cursor = other._cursor;

	memmove(_data, other._data, other._capacity);

	return *this;
}

void NetworkByteBuffer::reserve(size_t capacity)
{
	if (capacity > _capacity) {
		uint8_t *new_data = new uint8_t[capacity];
		memmove(new_data, _data, _capacity);
		delete[] _data;
		_data = new_data;
		_capacity = capacity;
	}
}

void NetworkByteBuffer::add_capacity(size_t capacity)
{
	uint8_t *new_data = new uint8_t[_capacity + capacity];
	memmove(new_data, _data, _capacity);
	delete[] _data;
	_data = new_data;
	_capacity += capacity;
}

void NetworkByteBuffer::seek_start()
{
	_cursor = 0;
}

uint8_t *NetworkByteBuffer::data() const
{
	return _data;
}

int8_t NetworkByteBuffer::read_i8()
{
	if (_capacity - _cursor < 1)
		throw std::out_of_range("Couldn't read 1 byte from network byte buffer");
	_cursor++;
	return *reinterpret_cast<int8_t *>(&_data[_cursor]);
}

uint8_t NetworkByteBuffer::read_u8()
{
	if (_capacity - _cursor < 1)
		throw std::out_of_range("Couldn't read 1 byte from network byte buffer");
	_cursor++;
	return *reinterpret_cast<uint8_t *>(&_data[_cursor]);
}

int16_t NetworkByteBuffer::read_i16()
{
	if (_capacity - _cursor < 2)
		throw std::out_of_range("Couldn't read 2 byte from network byte buffer");
	_cursor += 2;
	uint16_t value = ntohs(*reinterpret_cast<uint16_t *>(&_data[_cursor]));
	return *reinterpret_cast<int16_t *>(&value);
}

uint16_t NetworkByteBuffer::read_u16()
{
	if (_capacity - _cursor < 2)
		throw std::out_of_range("Couldn't read 2 byte from network byte buffer");
	_cursor += 2;
	return ntohs(*reinterpret_cast<uint16_t *>(&_data[_cursor]));
}

int32_t NetworkByteBuffer::read_i32()
{
	if (_capacity - _cursor < 4)
		throw std::out_of_range("Couldn't read 4 byte from network byte buffer");
	_cursor += 4;
	uint32_t value = ntohs(*reinterpret_cast<uint32_t *>(&_data[_cursor]));
	return *reinterpret_cast<int32_t *>(&value);
}

uint32_t NetworkByteBuffer::read_u32()
{
	if (_capacity - _cursor < 4)
		throw std::out_of_range("Couldn't read 4 byte from network byte buffer");
	_cursor += 4;
	return ntohl(*reinterpret_cast<uint32_t *>(&_data[_cursor]));
}

int64_t NetworkByteBuffer::read_i64()
{
	if (_capacity - _cursor < 8)
		throw std::out_of_range("Couldn't read 8 byte from network byte buffer");
	_cursor += 8;
	uint64_t value = ntohs(*reinterpret_cast<uint64_t *>(&_data[_cursor]));
	return *reinterpret_cast<int64_t *>(&value);
}

uint64_t NetworkByteBuffer::read_u64()
{
	if (_capacity - _cursor < 8)
		throw std::out_of_range("Couldn't read 8 byte from network byte buffer");
	_cursor += 8;
	return ntohll(*reinterpret_cast<uint64_t *>(&_data[_cursor]));
}
