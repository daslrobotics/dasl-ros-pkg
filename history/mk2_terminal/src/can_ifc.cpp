

#include "stdafx.h"
#include "can_ifc.h"


int16_t MK2::convertMsg2Can(CanPacket packets[], uint16_t packets_count, Message *msg)
{

	if(msg->data_size <= 8)
	{
		if(packets_count < 1) return -1; //not enough packets to build message;

		uint32_t id_msg = (msg->message_id & 0xFF) << 22;
		uint32_t id_dest = ((msg->destination & 0xF) << 18) | ((msg->destination >> 0x03) & 0x6);
		uint32_t id_src = (msg->source & 0x3F) << 12;
		uint32_t id_frame_seq = 0; //6 bits, 0 for single frame type message
		uint32_t id_frame_type = (3 << 3); //single frame type = 3
		
		packets[0].id = id_msg | id_dest | id_src | id_frame_seq | id_frame_type;
		
		for(uint8_t i = 0; i < msg->data_size; i++)
		{
			packets[0].data[i] = msg->data[i];
		}

		packets[0].data_size= (uint8_t)msg->data_size;
		packets[0].port = msg->destination < 32 ? 0 : 1;
		return 1;

	}else
	{
		//multiframe packet must follow sequence:

		//	start frame
		//	continuing frame (multiples off)
		//	end frame
		
		return 0;
	}
}


int16_t MK2::convertCan2Msg(Message *msg, CanPacket *packet)
{
 
	uint32_t id = packet->id;
		 
	uint8_t msg_id = (id >> 22) & 0x3F;
	uint8_t msg_dest = ((id >> 18) & 0x0F) | (id << 3) & 0x30;
	uint8_t msg_src = (id >> 12) & 0x3F;
	uint8_t msg_seq = (id >> 6) & 0x3F;
	uint8_t msg_type = (id >> 3) & 0x7;
	uint8_t msg_size = packet->data_size;

	//a multiframe message requires that the message be held internally as it is being built
	//but we are currently not using any messages that require it so we can ignore it
	switch(msg_type)
	{
		case 1: //multiframe: start frame
			break;
		case 2: //multiframe: end frame
			break;
		case  4://multiframe: continuing frame
			break;
		case 3: //single frame

			//single frames are 8 bytes or smaller and fit within a can packet
			msg->source = msg_src;
			msg->destination = msg_dest;
			msg->message_id = msg_id;
			msg->data_size = msg_size;

			for(uint8_t j = 0; j < msg_size; j++)
			{
				msg->data[j] = packet->data[j];
			}
		  
			return 1;
		default: //invalid frame type
			break;
 
	}		
	 
	  
	return 0;

}


