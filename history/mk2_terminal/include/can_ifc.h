

#ifndef CAN_IFC_H
#define CAN_IFC_H

#include "stdafx.h"
#include "messages.h"


namespace MK2
{

struct CanPacket
{
	uint32_t id;
	uint8_t port;
	uint8_t data[8];
	uint8_t data_size;
};



int16_t convertMsg2Can(CanPacket packets[], uint16_t packets_count, Message *msg);
int16_t convertCan2Msg(Message *msg, CanPacket *packet);

}

#endif




