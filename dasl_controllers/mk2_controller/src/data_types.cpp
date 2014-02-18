

#include "stdafx.h"
#include "data_types.h"
#include <stdint.h>

char * MK2::GetMessageNameString(uint8_t msg_id)
{
	switch(msg_id)
	{
	case 0: return "INVALID MSG";
	case 2: return "RESET CMD  ";
	case 3: return "STATE CMD  ";
	case 4: return "BIT TLM    ";
	case 10: return "CNTRL CMD ";
	case 11: return "IMP CMD   ";
	case 12: return "HS TLM    ";
	case 13: return "LS TLM    ";
	case 26: return "MS TLM    ";
	case 28: return "STATUS CMD";
	case 29: return "STATUS TLM";
	case 32: return "CMD ACK   ";
	case 33: return "CMD NACK  ";
	default:
		return "UNKNOWN MSG ID";
	}

}

char * MK2::GetSwStateString(uint8_t sw_state)
{
	switch(sw_state)
	{
	case 0: return "INIT";
	case 1: return "PROGRAM";
	case 2:return "FAIL SAFE";
	case 3: return "NOS CONTROL";
	case 4: return "NOS IDLE";
	case 5: return "NOS SLEEP";
	case 6: return "NOS DIAGNOSTICS";
	case 9: return "NOS DIAGNOSTICS 2";
	default:
		return "UNKNOWN SW STATE";
	}

}

char * MK2::GetFOCStateString(uint8_t foc_state)
{
	switch(foc_state)
	{
	case 0: return "FOC IDLE";
	case 1: return "FOC INIT";
	case 2: return "FOC START";
	case 3: return "FOC RUN";
	case 4: return "FOC STOP";
	case 5: return "FOC BRAKE";
	case 6: return "FOC WAIT";
	case 7: return "FOC FAULT";
	case 8: return "FOC EOTADJ";
	case 9: return "FOC STATE UNKNOWN";
	default:
		return "UNKNOWN FOC STATE";
	}

}

 
char * MK2::GetBITTypeString(uint32_t bit_type)
{
	switch(bit_type)
	{
	case MK2::BITType::OVER_TEMP: return "BIT: OVER TEMPERATURE";
	case MK2::BITType::OVER_VOLT: return "BIT: OVER VOLTAGE";
	case MK2::BITType::UNDER_VOLT: return "BIT: UNDER VOLTAGE";
	case MK2::BITType::OVER_CURRENT: return "BIT: OVER CURRENT";
	case MK2::BITType::FOC_INIT_FAIL: return "BIT: FOC INIT FAILURE";
	case MK2::BITType::FOC_MIN_SPEED: return "BIT: FOC MIN SPEED";
	case MK2::BITType::WRONG_DIRECTION: return "BIT: WRONG DIRECTION";		
	default: return "MULTIPLE BIT FAILURES";
	}

}





