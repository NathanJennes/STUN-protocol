//
// Created by Nathan Jennes on 10/25/22.
//

#ifndef STUN_BIND_REQUEST_H
#define STUN_BIND_REQUEST_H

class StunBindRequest
{
public:
	StunBindRequest(void) = default;
	StunBindRequest(const StunBindRequest &other) = default;
	~StunBindRequest(void) = default;
	StunBindRequest &operator=(const StunBindRequest &other) = default;
};

#endif //STUN_BIND_REQUEST_H