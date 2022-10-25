//
// Created by Nathan Jennes on 10/25/22.
//

#ifndef STUN_RESPONSE_H
#define STUN_RESPONSE_H

#include <cstdint>

class StunResponse
{
public:
	StunResponse(void);
	StunResponse(const StunResponse &other);
	~StunResponse(void);
	StunResponse &operator=(const StunResponse &other);
};

#endif //STUN_RESPONSE_H