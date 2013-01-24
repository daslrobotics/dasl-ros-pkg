

#include "stdafx.h"
#include "messages.h"

using namespace MK2;

static uint8_t this_source = 9;
static sender_callback sender_cb = NULL;


void MK2::set_source_id(uint8_t source)
{
	this_source = source;
}

void MK2::set_output_cb(sender_callback sender)
{
	sender_cb = sender;
}

void MK2::send_reset_cmd(uint8_t destination)
{
	Message msg;

	msg.message_id = MessageNumberEnum::RESET_CMD;
	msg.destination = destination;
	msg.source = this_source;
	msg.data_size = 1;
	msg.data[0] = destination;

	if(sender_cb != NULL)
	{
		(*sender_cb)(&msg);
	}

}

void MK2::send_status_req(uint8_t destination)
{
	Message msg;

	msg.message_id = MessageNumberEnum::STATUS_REQ_CMD;
	msg.destination = destination;
	msg.source = this_source;
	msg.data_size = 0;
	

	if(sender_cb != NULL)
	{
		sender_cb(&msg);
	}
}

void MK2::send_change_state_cmd(uint8_t destination, uint8_t new_state)
{
	Message msg;

	msg.message_id = MessageNumberEnum::CHANGE_STATE_CMD;
	msg.destination = destination;
	msg.source = this_source;
	msg.data_size = 2;
	msg.data[0] = destination;
	msg.data[1] = new_state;

	if(sender_cb != NULL)
	{
		(*sender_cb)(&msg);
	}
}

void MK2::send_control_cmd(uint8_t destination, ControlCmd *cmd)
{
	Message msg;
 
	msg.message_id = MessageNumberEnum::MOTOR_CONTROL_CMD;
	msg.destination = destination;
	msg.source = this_source;
	msg.data_size = 8;
	
	uint16_t pos_fp = (uint16_t)((cmd->position * pow(2.0f, 11.0f)) / MK2_PI);
	uint16_t vel_fp = (uint16_t)((cmd->velocity * pow(2.0f, 11.0f)) / 8.0f);
	uint16_t torque_fp = (uint16_t)((cmd->torque * pow(2.0f, 11.0f)) / 64.0f);
	uint16_t inst_curr_fp = (uint16_t)((cmd->inst_current_limit * pow(2.0f, 11.0f)) / 32.0f);

	msg.data[0] = (uint8_t)((pos_fp >> 0) & 0xFF);
	msg.data[1] = (uint8_t)((pos_fp >> 8) & 0xFF);
	msg.data[2] = (uint8_t)((vel_fp >> 0) & 0xFF);
	msg.data[3] = (uint8_t)((vel_fp >> 8) & 0xFF);
	msg.data[4] = (uint8_t)((torque_fp >> 0) & 0xFF);
	msg.data[5] = (uint8_t)((torque_fp >> 8) & 0xFF);
	msg.data[6] = (uint8_t)((inst_curr_fp >> 0) & 0xFF);
	msg.data[7] = (uint8_t)((inst_curr_fp >> 8) & 0xFF);
	 
	if(sender_cb != NULL)
	{
		(*sender_cb)(&msg);
	}
}
 
void MK2::send_impedance_cmd(uint8_t destination, ImpedanceCmd cmd)
{
	Message msg;
 
	msg.message_id = MessageNumberEnum::IMPEDANCE_CMD;
	msg.destination = destination;
	msg.source = this_source;
	msg.data_size = 6;
	
	uint16_t inertia_fp = (uint16_t)(cmd.inertia * 1000.0);
	uint16_t damping_fp = (uint16_t)((cmd.damping * pow(2.0f, 16.0f)) / 64.0f);
	uint16_t stiffness_fp = (uint16_t)((cmd.stiffness * pow(2.0f, 16.0f)) / 4096.0f);
	
	msg.data[0] = (uint8_t)((inertia_fp >> 0) & 0xFF);
	msg.data[1] = (uint8_t)((inertia_fp >> 8) & 0xFF);
	msg.data[2] = (uint8_t)((damping_fp >> 0) & 0xFF);
	msg.data[3] = (uint8_t)((damping_fp >> 8) & 0xFF);
	msg.data[4] = (uint8_t)((stiffness_fp >> 0) & 0xFF);
	msg.data[5] = (uint8_t)((stiffness_fp >> 8) & 0xFF);
	 
	if(sender_cb != NULL)
	{
		(*sender_cb)(&msg);
	}
}

bool MK2::process_status_response(StatusReponse *resp, Message *msg)
{
	if(msg->message_id != MK2::MessageNumberEnum::STATUS_TLM && msg->data_size != 1)
	{
		return false;
	}

	resp->sw_state = msg->data[0] & 0x0F;
	resp->bit_error_status = (msg->data[0] >> 4) & 0x3;
	resp->debug_enabled = (msg->data[0] >> 6) & 0x03;
	
	return true;
}



bool MK2::process_hs_telemetry(HSTelemetry *resp, Message *msg)
{
	if(msg->message_id != MK2::MessageNumberEnum::HIGH_SPEED_TLM && msg->data_size != 8)
	{
		return false;
	}
	//12 bit fixed point data from telemetry
	int16_t pos_fp = (msg->data[0] | ((msg->data[1] & 0x0F) << 8) | ((msg->data[3] & 0x0F) << 12)) & 0xFFFF;
	int16_t vel_fp = ((msg->data[1] >> 4) | msg->data[2] << 4) & 0xFFF;
	int16_t trq_fp = ((msg->data[4] >> 4) | msg->data[5] << 4) & 0xFFF;
	int16_t curr_fp = (msg->data[6] | msg->data[7] << 8) & 0xFFF;

	if((vel_fp & 0xF00) == 0xF00) vel_fp |= 0xFF00;
    if((trq_fp & 0xF00) == 0xF00) trq_fp |= 0xFF00;
    if((curr_fp & 0xF00) == 0xF00) curr_fp |= 0xFF00;
	 
	resp->position = (pos_fp * (float)MK2_PI) / (float)pow(2.0f, 11.0f);
	resp->velocity = (vel_fp * 8) / (float)pow(2.0f, 11.0f);
	resp->torque = (vel_fp * 64) / (float)pow(2.0f, 11.0f);
	resp->inst_current = (vel_fp * 32) / (float)pow(2.0f, 11.0f);
	resp->position_limit = (msg->data[7] >> 4) & 0xF;

	return true;
}
bool MK2::process_ms_telemetry(MSTelemetry *resp, Message *msg)
{
	if(msg->message_id != MK2::MessageNumberEnum::MED_SPEED_TLM && msg->data_size != 8)
	{
		return false;
	}

	resp->var1 = msg->data[0] | (msg->data[1] << 8);
	resp->var2 = msg->data[2] | (msg->data[3] << 8);
	resp->var3 = msg->data[4] | (msg->data[5] << 8);
	resp->var4 = msg->data[6] | (msg->data[7] << 8);

	return true;
}

bool MK2::process_ls_telemetry(LSTelemetry *resp, Message *msg)
{
	if(msg->message_id != MK2::MessageNumberEnum::LOW_SPEED_TLM && msg->data_size != 7)
	{
		return false;
	}

	resp->sw_state = msg->data[0] & 0xF;
	resp->bit_error_status = (msg->data[0] >> 4) & 0x03;
	resp->debug_enabled = (msg->data[0] >> 6) & 0x03;
	resp->current_limit_reached = (msg->data[1]) & 0x01;
	resp->foc_state = (msg->data[1] >> 1) & 0x07;
	resp->fault_detected = (msg->data[1] >> 4) & 0x0F;

	uint16_t busv_fp = (msg->data[2] | (msg->data[3] << 8)) & 0xFFF;
	uint16_t busc_fp = ((msg->data[3] >> 4) | msg->data[5] << 4) & 0xFFF;
	uint16_t temp_fp = (msg->data[5] | (msg->data[6] << 8)) & 0xFFF;

	resp->bus_voltage = busv_fp * 32 / pow(2.0f, 12.0f);
	resp->bus_current = busc_fp * 32 / pow(2.0f, 12.0f);
	resp->temperature = temp_fp * 128 / pow(2.0f, 12.0f);

	return true;
}

bool MK2::process_bit_response(BITErrorTelemetry *resp, Message *msg)
{
	if(msg->message_id != MK2::MessageNumberEnum::BIT_ERR_TLM && msg->data_size != 5)
	{
		return false;
	}

	resp->context = msg->data[0];
	resp->error_type = msg->data[1] | (msg->data[2] << 8) | (msg->data[3] << 16) | (msg->data[4] << 24);

	return true;
}

bool MK2::process_cmd_ack(CmdAck *resp, Message *msg)
{
	if(msg->message_id != MK2::MessageNumberEnum::CMD_ACK_TLM && msg->data_size != 1)
	{
		return false;
	}

	resp->msg_id = msg->data[0];

	return true;
}

bool MK2::process_cmd_nack(CmdNack *resp, Message *msg)
{
	if(msg->message_id != MK2::MessageNumberEnum::CMD_NACK_TLM && msg->data_size != 3)
	{
		return false;
	}

	resp->msg_id = msg->data[0];
	resp->error_code = msg->data[1] | (msg->data[2] << 8);

	return true;
}





