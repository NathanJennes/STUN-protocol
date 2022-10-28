//
// Created by Nathan Jennes on 10/26/22.
//

#ifndef STUN_CLIENT_H
#define STUN_CLIENT_H

#define STUN_PACKET_SIZE 1024 // resonable length

#include "Response.h"
#include "BindRequest.h"

namespace Stun {

Response send(int sock, const BindRequest &request);

}

#endif //STUN_CLIENT_H
