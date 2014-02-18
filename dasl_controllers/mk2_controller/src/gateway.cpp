

#include "stdafx.h"
#include "gateway.h"
 
int16_t MK2::Can2Gateway(uint8_t *data, uint8_t data_size, CanPacket *packet)
{
	if(packet->data_size > 8 || data_size != 16) return -1;

	data[0] = packet->id & 0xFF;
	data[1] = (packet->id >> 8) & 0xFF;
	data[2] = (packet->id >> 16) & 0xFF;
	data[3] = (packet->id >> 24) & 0xFF;
	data[4] = (packet->data_size);
	data[5] = packet->port & 0x01;
	data[6] = 0;
	data[7] = 0;
	
	for(uint8_t i = 0; i < packet->data_size; i++)
	{
		data[8 + i] = packet->data[i];
	}
	 
	return 0;
}

int16_t MK2::Gateway2Can(CanPacket *packet, uint8_t *data, uint8_t data_size)
{

	if(data[4] > 8 || data[5] > 1) return -1; //wrong size and wrong port

	packet->id = data[0] | (data[1] << 8) | (data[2] << 16) | (data[3] << 24);
	packet->data_size = data[4];
	packet->port = data[5] & 0x01;

	for(uint8_t i = 0; i < data[4]; i++)
	{
		packet->data[i] = data[8+i];
	}

	return 0;

}


