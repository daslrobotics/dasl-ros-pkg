

#ifndef GATEWAY_H
#define GATEWAY_H


#include "stdafx.h"
#include "can_ifc.h"
#include "messages.h"

namespace MK2
{
	#define GATEWAY_RIGHT_CAN_PORT 0
	#define GATEWAY_LEFT_CAN_PORT 1		
	#define GATEWAY_IP_ADDRESS_STRING	"192.168.0.10"
	#define GATEWAY_IP_PORT_NUM			5555


	int16_t Can2Gateway(uint8_t *data, uint8_t data_size, CanPacket *packet);
	int16_t Gateway2Can(CanPacket *packet, uint8_t *data, uint8_t data_size);
 

}


#endif
