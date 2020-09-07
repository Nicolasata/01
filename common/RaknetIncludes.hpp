#ifndef RACKNETINCLUDES_HPP_
#define RACKNETINCLUDES_HPP_

#define MAX_PLAYER_CONNEXION 10

#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>

#include "../Raknet/MessageIdentifiers.h"
#include "../Raknet/RakPeerInterface.h"
#include "../Raknet/RakNetStatistics.h"
#include "../Raknet/RakNetTypes.h"
#include "../Raknet/NetworkIDObject.h"
#include "../Raknet/NetworkIDManager.h"
#include "../Raknet/BitStream.h"
#include "../Raknet/RakSleep.h"
#include "../Raknet/PacketLogger.h"

#include "./ThreadPool.hpp"

enum SERVER_MESSAGES
{
    ID_ADD_PLAYER = ID_USER_PACKET_ENUM+1,
    ID_DELETE_PLAYER = ID_USER_PACKET_ENUM+2,
    ID_GET_INFORMATIONS = ID_USER_PACKET_ENUM+3,
    ID_MYPING = ID_USER_PACKET_ENUM+4,

    ID_ACTION_MOVE = ID_USER_PACKET_ENUM+5,
    ID_ACTION_STOP_MOVE = ID_USER_PACKET_ENUM+6,

};

#endif