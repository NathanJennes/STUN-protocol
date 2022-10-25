//
// Created by Nathan Jennes on 10/25/22.
//

#include "StunResponse.h"

StunResponse::StunResponse(void)
{
}

StunResponse::StunResponse(const StunResponse &other)
{
	(void)other;
}

StunResponse::~StunResponse(void)
{
}

StunResponse &StunResponse::operator=(const StunResponse &other)
{
	if (this == &other)
		return *this;

	return *this;
}