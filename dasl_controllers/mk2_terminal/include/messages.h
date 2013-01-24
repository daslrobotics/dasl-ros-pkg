

#ifndef MESSAGES_H
#define MESSAGES_H


#include "data_types.h"
#include <stdint.h>
#include <vector>

#define MK2_PI 3.14159265359

namespace MK2
{
 
struct Message
{ 
	uint8_t source;
	uint8_t destination;
	uint8_t message_id;
	uint8_t data[512];
	uint16_t data_size;	 
};
   
struct ControlCmd
{
	float position;
	float velocity;
	float torque;
	float inst_current_limit;
};

struct ImpedanceCmd
{
	float inertia;
	float damping;
	float stiffness;
};

struct StatusReponse
{
	uint8_t sw_state;
	uint8_t bit_error_status;
	uint8_t debug_enabled;
};

struct HSTelemetry
{
	float position;
	float velocity;
	float torque;
	float inst_current;
	uint8_t position_limit;

};

struct MSTelemetry
{
	uint16_t var1;
	uint16_t var2;
	uint16_t var3;
	uint16_t var4;
};

struct LSTelemetry
{
	uint8_t sw_state;
	uint8_t bit_error_status;
	uint8_t debug_enabled;
	uint8_t current_limit_reached;
	uint8_t foc_state;
	uint8_t fault_detected;

	float bus_voltage;
	float bus_current;
	float temperature;	 
};

struct BITErrorTelemetry
{
	uint8_t context;
	uint32_t error_type;
};

struct CmdAck
{
	uint8_t msg_id;
};

struct CmdNack
{
	uint8_t msg_id;
	uint16_t error_code;
};

struct MK2Drive
{
	uint8_t port;
	bool control_enable;
	ControlCmd control;
	ImpedanceCmd impedance;
	StatusReponse status;
	HSTelemetry hs;
	MSTelemetry ms;
	LSTelemetry ls;
	BITErrorTelemetry bit;
	CmdAck ack;
	CmdNack nack;
};



typedef void (*sender_callback)(Message *msg);

void set_source_id(uint8_t source);
void set_output_cb(sender_callback sender);

void send_reset_cmd(uint8_t destination);
void send_status_req(uint8_t destination);
void send_change_state_cmd(uint8_t destination, uint8_t new_state);

void send_control_cmd(uint8_t destination, ControlCmd *cmd);

void send_impedance_cmd(uint8_t destination, ImpedanceCmd cmd);

bool process_status_response(StatusReponse *resp, Message *msg);
bool process_hs_telemetry(HSTelemetry *resp, Message *msg);
bool process_ms_telemetry(MSTelemetry *resp, Message *msg);
bool process_ls_telemetry(LSTelemetry *resp, Message *msg);
bool process_bit_response(BITErrorTelemetry *resp, Message *msg);
bool process_cmd_ack(CmdAck *resp, Message *msg);
bool process_cmd_nack(CmdNack *resp, Message *msg);


}

#endif
