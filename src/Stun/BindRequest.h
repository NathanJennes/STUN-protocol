//
// Created by Nathan Jennes on 10/25/22.
//

#ifndef STUN_BIND_REQUEST_H
#define STUN_BIND_REQUEST_H

namespace Stun {

class BindRequest
{
public:
	BindRequest() = default;
	BindRequest(const BindRequest &other) = default;
	~BindRequest() = default;
	BindRequest &operator=(const BindRequest &other) = default;
};

}

#endif //STUN_BIND_REQUEST_H