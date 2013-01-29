

#ifndef DATA_TYPES_H
#define DATA_TYPES_H

#include "stdafx.h"
 

namespace MK2
{
	struct NodeID
	{
		static const uint8_t INVALID_ID		= 0;
		static const uint8_t ALL_DRIVES		= 14;
		static const uint8_t ALL_DEVICES	= 15;

	};


	struct MessageNumberEnum
	{
	public:
		static const uint8_t INVALID_MSG_NUM	= 0;	
		static const uint8_t RESET_CMD			= 2;
		static const uint8_t CHANGE_STATE_CMD	= 3;
		static const uint8_t BIT_ERR_TLM		= 4;
		static const uint8_t MOTOR_CONTROL_CMD	= 10;
		static const uint8_t IMPEDANCE_CMD		= 11;
		static const uint8_t HIGH_SPEED_TLM		= 12;
		static const uint8_t LOW_SPEED_TLM		= 13;    
		static const uint8_t MED_SPEED_TLM		= 26;
		static const uint8_t STATUS_REQ_CMD		= 28;
		static const uint8_t STATUS_TLM			= 29;     
		static const uint8_t CMD_ACK_TLM		= 32;
		static const uint8_t  CMD_NACK_TLM		= 33;  
	};

	struct SWStates
	{
		static const uint8_t INIT = 0;
		static const uint8_t PRG = 1;
		static const uint8_t FS = 2;
		static const uint8_t NOS_CONTROL = 3;
		static const uint8_t NOS_IDLE = 4;
		static const uint8_t NOS_SLEEP = 5;
		static const uint8_t NOS_DEBUG = 6;
		static const uint8_t UNKNOWN = 15;
	};

	struct FOCStates
	{
 
        static const uint8_t IDLE = 0;
        static const uint8_t INIT = 1;
        static const uint8_t START = 2;
        static const uint8_t RUN = 3;
        static const uint8_t STOP = 4;
        static const uint8_t BRAKE = 5;
        static const uint8_t WAIT = 6;
        static const uint8_t FAULT = 7;
        static const uint8_t EOTADJ = 8;
        static const uint8_t UNKNOWN = 9;
	};


	struct BITContext
	{
        static const uint8_t RBIT = 0;
        static const uint8_t BBIT = 1;
        static const uint8_t CBIT = 2;
        static const uint8_t NO_BIT_CONTEXT = -1;
	};

	struct BITType
	{
        static const uint32_t NO_BIT_ERROR = 0x00000000;
        static const uint32_t OVER_TEMP = 0x00000100;
        static const uint32_t OVER_VOLT = 0x00000800;
        static const uint32_t UNDER_VOLT = 0x00001000;
        static const uint32_t OVER_CURRENT = 0x00002000;
        static const uint32_t FOC_INIT_FAIL = 0x00008000;
        static const uint32_t FOC_MIN_SPEED = 0x00010000;
        static const uint32_t WRONG_DIRECTION = 0x00020000;
	};

	struct BITStatus
	{
        static const uint8_t NO_FAILURE = 0;
        static const uint8_t CRITICAL_FAILURE = 1;
        static const uint8_t NON_CRITICAL_FAILURE = 2;
	};

char * GetMessageNameString(uint8_t msg_id);
char * GetSwStateString(uint8_t sw_state);
char * GetFOCStateString(uint8_t foc_state);
char * GetBITTypeString(uint32_t bit_type);

};





#endif
