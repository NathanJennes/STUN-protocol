//
// Created by Nathan Jennes on 10/26/22.
//

#ifndef STUN_CLIENT_H
#define STUN_CLIENT_H

#define STUN_PACKET_SIZE 1024 // resonable length

#include "StunResponse.h"
#include "StunBindRequest.h"

namespace STUNClient {

StunResponse send(int sock, const StunBindRequest &request);

}

#endif //STUN_CLIENT_H
