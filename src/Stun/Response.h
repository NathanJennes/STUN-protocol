//
// Created by Nathan Jennes on 10/25/22.
//

#ifndef STUN_RESPONSE_H
#define STUN_RESPONSE_H

#include <cstdint>

namespace Stun {

class Response
{
public:
	Response();
	Response(const Response &other);
	~Response();
	Response &operator=(const Response &other);
};
}

#endif //STUN_RESPONSE_H