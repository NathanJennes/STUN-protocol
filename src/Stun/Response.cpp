//
// Created by Nathan Jennes on 10/25/22.
//

#include "Response.h"

namespace Stun {

Response::Response()
{
}

Response::Response(const Response &other)
{
	(void) other;
}

Response::~Response()
{
}

Response &Response::operator=(const Response &other)
{
	if (this == &other)
		return *this;

	return *this;
}

}
