// DrexelExample.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "messages.h"
#include "gateway.h"
#include "can_ifc.h"

#include <pthread.h> // <- added


#define MILIHERTZ 1000
#define DEFAULTPOSITION 1.0f  
#define CONTROLVELOCITY 0.5f//0.2f
#define DEFAULTDEVIATION 0.01f//0.01f;


#define BACK_FORTH_EXAMPLE
//#define TERMINAL_EXAMPLE
#define GATEWAY_DUAL_PORT

MK2::MK2Drive Drives[64];

typedef unsigned int DWORD;  //<-- added
typedef struct sockaddr * LPSOCKADDR; //<-- added
typedef void * LPVOID; //<-- added

pthread_t recvThreadID;		// changed
pthread_t controlThreadID;	// chagned
//DWORD recvThreadID;
//DWORD controlThreadID;

//WSADATA wsaData;  //<-- commented out
int clientSock = -1; 
struct sockaddr_in clientAddr; 
struct sockaddr_in serverAddr; /*server address*/
 
bool req_ls_tlm[64];
bool req_ms_tlm[64];
bool req_hs_tlm[64];
bool wait_some_more = false;

void req_all_tlm(bool req[])
{
	for(int i = 0; i < 64; i++)
	{
		req[i] = true;
	}
}

//callback to send messages
void send_via_udp(MK2::Message *msg)
{
	uint8_t udp_data[512];
	uint8_t udp_data_idx = 0;
	MK2::CanPacket OutPackets[32];

	//convert the message into can packets suitable for CAN bus
	uint16_t packets = MK2::convertMsg2Can(OutPackets, 32, msg);

	if(packets > 1 || packets <= 0) return; //we can handle more, but we should only be using messages that are 1 can packet

	//convert the can packets into a udp message
	for(int i = 0; i < packets; i++)
	{		
#ifdef GATEWAY_DUAL_PORT
		if(Drives[msg->destination].port == 3)
		{
			//send to both ports
			OutPackets[i].port = 0;
			MK2::Can2Gateway(&udp_data[(udp_data_idx++)*16], 16, &OutPackets[i]); //right port of the gateway is connected to the outside.
			OutPackets[i].port = 1;
			MK2::Can2Gateway(&udp_data[(udp_data_idx++)*16], 16, &OutPackets[i]); //right port of the gateway is connected to the outside.
		}else

		{
			OutPackets[i].port = Drives[msg->destination].port & 0x01;
			MK2::Can2Gateway(&udp_data[(udp_data_idx++)*16], 16, &OutPackets[i]); //right port of the gateway is connected to the outside.
		}
#else
			OutPackets[i].port = 0;
			MK2::Can2Gateway(&udp_data[(udp_data_idx++)*16], 16, &OutPackets[i]); //right port of the gateway is connected to the outside.
#endif

	}

	uint16_t len = udp_data_idx * 16;

	//the gateway can only handle 32 can packets per udp message, anymore and a buffer mechanism is needed
	sendto(clientSock, (char *)udp_data,len,0,(LPSOCKADDR)&serverAddr,sizeof(serverAddr));

}



//thread to read input from gateway
//DWORD WINAPI receive_via_udp(LPVOID ptr )
void *receive_via_udp(void * ptr )   //<-- changed
{
	char udp_data[512];
	int recv_len = 0;
	MK2::CanPacket can_packet;
	MK2::Message msg;
	MK2::MK2Drive *drive;
	struct sockaddr_in recvAddr;
	
	recvAddr.sin_addr.s_addr=INADDR_ANY;
	recvAddr.sin_port = htons(0);
	recvAddr.sin_family = AF_INET;
	socklen_t recvAdddr_len = sizeof(recvAddr);  //<-- changed
	//int recvAdddr_len = sizeof(recvAddr);
	int error = 0;

	while(1)
	{
		recv_len = recvfrom(clientSock,udp_data,512,0,(LPSOCKADDR)&recvAddr, &recvAdddr_len);

		if(recv_len == -1)
		{
			error = -1; // changed
			//error = WSAGetLastError();
		}
	if(recv_len % 16 != 0) continue; //must be multiples of 16 to be valid

	for(int i = 0; i < recv_len/16; i++)
	{		 
		MK2::Gateway2Can(&can_packet, (uint8_t *) &udp_data[16 * i], 16);

		if(MK2::convertCan2Msg(&msg, &can_packet) <= 0) continue;
		if(msg.source >= 64) continue;

		drive = &Drives[msg.source];
		
		drive->port = can_packet.port;

		switch(msg.message_id)
		{
			case MK2::MessageNumberEnum::HIGH_SPEED_TLM:
				if(MK2::process_hs_telemetry(&drive->hs, &msg) && req_hs_tlm[msg.source])
				{
					printf("\r\n  %s ID:%d  ",MK2::GetMessageNameString(msg.message_id), msg.source);
					printf("POS:%.2f VEL:%.2f TRQ:%.2f I:%.2f L:%d", drive->hs.position, drive->hs.velocity, drive->hs.torque, drive->hs.inst_current, drive->hs.position_limit);	
					req_hs_tlm[msg.source] = false;
				}
				break;

			case MK2::MessageNumberEnum::MED_SPEED_TLM:
				if(MK2::process_ms_telemetry(&drive->ms, &msg) && req_ms_tlm[msg.source])
				{
					printf("\r\n  %s ID:%d  ",MK2::GetMessageNameString(msg.message_id), msg.source);
					printf("V1:%d V2:%d V3:%d V4:%d",  drive->ms.var1, drive->ms.var2, drive->ms.var3, drive->ms.var4);	
					req_ms_tlm[msg.source] = false;
				}
				break;

			case MK2::MessageNumberEnum::LOW_SPEED_TLM:
				if(MK2::process_ls_telemetry(&drive->ls, &msg) && req_ls_tlm[msg.source])
				{
					printf("\r\n  %s ID:%d  ",MK2::GetMessageNameString(msg.message_id), msg.source);
					printf("SW:%d FOC:%d V:%dV I:%dA T:%dC",  drive->ls.sw_state, drive->ls.foc_state, (int)drive->ls.bus_voltage, (int)drive->ls.bus_current, (int)drive->ls.temperature);
				req_ls_tlm[msg.source] = false;
				}
				break;

			case MK2::MessageNumberEnum::BIT_ERR_TLM:

				if(MK2::process_bit_response(&drive->bit, &msg))
				{
					printf("\r\n  %s ID:%d  ",MK2::GetMessageNameString(msg.message_id), msg.source);
					drive->status.sw_state = MK2::SWStates::FS; //should send status request, but state is already FS due to BIT
					drive->ls.sw_state = MK2::SWStates::FS;
					printf("STATE:%s", MK2::GetSwStateString(drive->status.sw_state));					
					wait_some_more = true;
				}
				break;

			case MK2::MessageNumberEnum::STATUS_TLM:
				if(MK2::process_status_response(&drive->status, &msg))
				{
					printf("\r\n  %s ID:%d  ",MK2::GetMessageNameString(msg.message_id), msg.source);
					printf("STATE:%s", MK2::GetSwStateString(drive->status.sw_state));
					wait_some_more = true;
				}
				break;
			case MK2::MessageNumberEnum::CMD_ACK_TLM:
				if(MK2::process_cmd_ack(&drive->ack, &msg))
				{
					printf("\r\n  %s ID:%d  ",MK2::GetMessageNameString(msg.message_id), msg.source);
					printf("MSG:%s", MK2::GetMessageNameString(drive->ack.msg_id));
					if(drive->ack.msg_id == MK2::MessageNumberEnum::CHANGE_STATE_CMD) //see what the new state is
					{
						MK2::send_status_req(msg.source);
					}
					wait_some_more = true;
				}
				break;
			case MK2::MessageNumberEnum::CMD_NACK_TLM:
				if(MK2::process_cmd_nack(&drive->nack, &msg))
				{
					printf("\r\n  %s ID:%d  ",MK2::GetMessageNameString(msg.message_id), msg.source);
					printf("MSG:%s ERROR:%x",  MK2::GetMessageNameString(drive->nack.msg_id), drive->nack.error_code);
					wait_some_more = true;
				}
				break;

		}

	}
	}

	std::cout << std::endl;

	return 0;


}

//thread to run at 50 Hz message
//DWORD WINAPI control_cmd_thread(LPVOID ptr )
void *control_cmd_thread(void * ptr )
{
	MK2::MK2Drive *drive;

	while(1)
	{
		for(int i = 0 ; i < 64; i++)
		{

			drive = &Drives[i];
			if(drive->control_enable == true)
			{
				if(drive->status.sw_state == MK2::SWStates::NOS_CONTROL || drive->status.sw_state == MK2::SWStates::NOS_IDLE || drive->status.sw_state == MK2::SWStates::NOS_SLEEP)
				{
					MK2::send_control_cmd(i, &drive->control);
				}
			}
		 
		}
		//50 Hz
		//Sleep(20);  
		usleep(20 *MILIHERTZ);   //<-- changed
	}
	return 0;
}

unsigned int split(const std::string &txt, std::vector<std::string> &strs, char ch)
{
    unsigned int pos = txt.find( ch );
    unsigned int initialPos = 0;
    strs.clear();

    // Decompose statement
    while( pos != std::string::npos ) {
        strs.push_back( txt.substr( initialPos, pos - initialPos ) );
        initialPos = pos + 1;

        pos = txt.find( ch, initialPos );
    }

	unsigned int len = txt.size();
	unsigned int min_pos;// = min( pos, len) ;          <-------------------------------- need to fix
    // Add the last one
    strs.push_back( txt.substr( initialPos, min_pos - initialPos + 1 ) );

    return strs.size();
}

void set_reset_status_to_all_drives(void)
{
	for(int i = 0; i < 64; i++)
	{
		Drives[i].status.sw_state = MK2::SWStates::UNKNOWN;
		Drives[i].ls.sw_state = MK2::SWStates::UNKNOWN;
		Drives[i].control_enable = false;
		req_hs_tlm[i] = false;
		req_ls_tlm[i] = false;
		req_ms_tlm[i] = false;
	}

}

void init_udp(void)
{
	int s;			
	if((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1)		// <-------------- big change
	//if(WSAStartup(MAKEWORD(2, 2),&wsaData) != 0) /*windows socket startup */
	{
		printf("WSAStartup Failed");
		system("PAUSE");
		exit(1);
	}
	 
	memset((char*)&clientAddr,0,sizeof(clientAddr));
	clientAddr.sin_family=AF_INET; /*set client address protocol family*/
	clientAddr.sin_addr.s_addr=INADDR_ANY;
	clientAddr.sin_port=htons((u_short)GATEWAY_IP_PORT_NUM);  

	serverAddr.sin_addr.s_addr=inet_addr(GATEWAY_IP_ADDRESS_STRING); 
    	serverAddr.sin_family=AF_INET;
	serverAddr.sin_port=htons((u_short)GATEWAY_IP_PORT_NUM);  
	 

	if (serverAddr.sin_addr.s_addr==INADDR_NONE)
	{
		printf("Unable to obtain Gateway IP (bad IP)");
		system("PAUSE");
		exit(1);
    }
	  
	clientSock = socket(PF_INET,SOCK_DGRAM,0);/*create a socket*/

	if (clientSock<0)
	{
		printf("Socket creation failed\n");
		system("PAUSE");
		exit(1);
	}

	/*bind a client address and port*/
	if (bind(clientSock,(LPSOCKADDR)&clientAddr,sizeof(struct sockaddr))<0)
	{
		printf("Socket bind failed\n");
		system("PAUSE");
		exit(1);
	}


}

int main(int argc, char** argv)
//int _tmain(int argc, _TCHAR* argv[])
{
	std::vector<std::string> lines;
	std::cout << "MK2 Example Application" << std::endl;

	init_udp(); //initialize udp socket to talk to the gateway
	 
	MK2::set_source_id(9);				//this id is sent to all drives, 9 is typically chosen as the controller
	MK2::set_output_cb(send_via_udp);	//all the send commands call the send_via_udp callback to output to udp

	pthread_create(&recvThreadID, NULL, receive_via_udp, NULL);		//<--- added
	pthread_create(&controlThreadID, NULL, control_cmd_thread, NULL); 

	//CreateThread(NULL, 0, receive_via_udp, NULL, 0, &recvThreadID);				//receive thread & message input processing
	//CreateThread(NULL,  0,   control_cmd_thread,NULL, 0, &controlThreadID);		// 50 Hz control command thread
	 
	set_reset_status_to_all_drives();				//reset values in structures that hold drive data	
	MK2::send_reset_cmd(MK2::NodeID::ALL_DEVICES);			//reset drive to a known state

	//a little hack for the send status request to send on both ports
	Drives[15].port = 3;
	Drives[14].port = 3;

	usleep(100 *MILIHERTZ);	//allow some time for the drives to initialize
	//Sleep(100);	//allow some time for the drives to initialize  <-- changed

	while(wait_some_more) //allow the console output to settle down
	{
		wait_some_more = false;
		usleep(100 *MILIHERTZ); 						// <-- changed
		//Sleep(100); 
	}

#ifdef BACK_FORTH_EXAMPLE

	int drive_id = -1;

	//scan for all available drives
	while(drive_id == -1)
	{
		MK2::send_status_req(MK2::NodeID::ALL_DEVICES); //request all devices to report status		
		usleep(300 *MILIHERTZ);  // changed
		//Sleep(300);
		for(int i = 0; i < 64; i++)
		{
			//pick the first that responds with INIT STATE
			if(Drives[i].status.sw_state == MK2::SWStates::INIT)
			{
				
				std::cout << "\r\n\r\nSelected Drive ID: " << i << std::endl;
				drive_id = i;
				break;
			}
		}
	}

	drive_id = 57;

	//change state to NOS_CONTROL in order to accept motor control commands	
	MK2::send_change_state_cmd(drive_id, MK2::SWStates::NOS_CONTROL);		

	//wait for drive to respond with new state
	//while(Drives[drive_id].status.sw_state != MK2::SWStates::NOS_CONTROL); //NOS_IDLE & NOS_SLEEP are valid as well and should be checked (not done for example clarity)

	std::string resp;
	float default_pos = DEFAULTPOSITION;
	float dev = DEFAULTDEVIATION;//0.01f;
	float alt = 1.0f;

	Drives[drive_id].control.position = default_pos;
	Drives[drive_id].control.velocity = CONTROLVELOCITY;
	Drives[drive_id].control.torque = 0; //not used
	Drives[drive_id].control.inst_current_limit = 8;
	Drives[drive_id].control_enable = true;

	std::cout << std::endl << std::endl;	

	while(Drives[drive_id].status.sw_state != MK2::SWStates::FS) //if fail safe triggered, exit
	{
		std::cout << "Moving Drive " << drive_id << " To " << alt * default_pos << " rad.    ";
		std::cout << "(Pos: " << Drives[drive_id].hs.position << " rad." << " Temp: " << Drives[drive_id].ls.temperature << "C)" << std::endl;
		
		Drives[drive_id].control.position = alt * default_pos;		
		while((Drives[drive_id].hs.position < (alt * default_pos - dev)) || (Drives[drive_id].hs.position > (alt * default_pos + dev))) usleep(MILIHERTZ);
  
		alt *= -1.0f;
	}
	 

#elif defined TERMINAL_EXAMPLE
	while(true)
	{
		//wait for user input
		std::string input_line;
		std::cout << "\r\nINPUT COMMAND >> ";
		std::getline(std::cin, input_line);
		std::transform(input_line.begin(), input_line.end(), input_line.begin(), ::toupper);

		split(input_line, lines, ' ');
 
		if(lines[0] == "STATUS")
		{
			MK2::send_status_req(MK2::NodeID::ALL_DEVICES);
		
		}else if(lines[0] == "RESET")
		{
			if(lines.size() == 1) //reset command alone
			{
				set_reset_status_to_all_drives();
				MK2::send_reset_cmd(MK2::NodeID::ALL_DEVICES);
			}else if(lines.size() == 2)
			{
				if(lines[1] == "ALL") //RESET ALL
				{
					set_reset_status_to_all_drives();
					MK2::send_reset_cmd(MK2::NodeID::ALL_DEVICES);
				}else //RESET #n  (where n is 0 - 63)
				{
					int n = std::stoi(lines[1]);

					if(n >= 0 && n <= 63)
					{
						Drives[n].status.sw_state = MK2::SWStates::INIT;
						Drives[n].ls.sw_state = MK2::SWStates::INIT;
						MK2::send_reset_cmd(n);
					}else
					{
						std::cout << "Invalid Parameter" << std::endl;
					}

				}
			}else
			{
				std::cout << "Invalid Parameter" << std::endl;
			}
		}else if(lines[0] == "MOVE") //MOVE #n POS VEL
		{
			if(lines.size() < 4) 
			{
				std::cout << "Invalid Parameters" << std::endl;
			}else
			{
				int sel = std::stoi(lines[1]); //which drive selected

				float pos = std::stof(lines[2]);
				float vel = std::stof(lines[3]);

				if(Drives[sel].status.sw_state == MK2::SWStates::FS) //check if drive is in fail safe
				{
					Drives[sel].control.inst_current_limit = 0;
					std::cout << "Drive in Fail Safe, Reset first" << std::endl;
					continue;
				}

				//range check on values
				if(pos > -MK2_PI && pos < MK2_PI && vel > -2*MK2_PI && vel < 2*MK2_PI)
				{
					Drives[sel].control.position = pos;
					Drives[sel].control.velocity = vel;
					Drives[sel].control.inst_current_limit = 6;
					Drives[sel].control_enable = true;
					//drive is in init state, change to nos
					if(Drives[sel].status.sw_state == MK2::SWStates::INIT)
					{
						MK2::send_change_state_cmd(sel,MK2::SWStates::NOS_CONTROL);						
					} 

				}else
				{
					std::cout << "Out of Range Position/Velocity parameters" << std::endl;
					Drives[sel].control.velocity = 0;
					Drives[sel].control.inst_current_limit = 0;

				}

			}
	
		}else if(lines[0] == "IMPEDANCE") //impedance #n inertia damping stiffness
		{
			if(lines.size() < 5) 
			{
				std::cout << "Invalid Parameters" << std::endl;
			}else
			{
				int sel = std::stoi(lines[1]); //which drive selected

				float inertia = std::stof(lines[2]);
				float damping = std::stof(lines[3]);
				float stiffness = std::stof(lines[3]);

				if(Drives[sel].status.sw_state == MK2::SWStates::FS) //check if drive is in fail safe
				{
					Drives[sel].control.inst_current_limit = 0;
					std::cout << "Drive in Fail Safe, Reset first" << std::endl;
					continue;
				}

				//range check on values
				if(Drives[sel].hs.position > -MK2_PI && Drives[sel].hs.position < MK2_PI && Drives[sel].hs.velocity > -2*MK2_PI && Drives[sel].hs.velocity < 2*MK2_PI)
				{
					Drives[sel].impedance.inertia = inertia;
					Drives[sel].impedance.damping = damping;
					Drives[sel].impedance.stiffness = stiffness;
					Drives[sel].control_enable = true;
					//drive is in init state, change to nos
					if(Drives[sel].status.sw_state == MK2::SWStates::INIT)
					{
						MK2::send_change_state_cmd(sel,MK2::SWStates::NOS_CONTROL);						
					} 

					MK2::send_impedance_cmd(sel, Drives[sel].impedance);

				}else
				{
					std::cout << "Out of Range Position/Velocity parameters" << std::endl;
					Drives[sel].control.velocity = 0;
					Drives[sel].control.inst_current_limit = 0;

				}

			}
		}else if(lines[0] == "HS") 
		{
			 req_all_tlm(req_hs_tlm);
		}else if(lines[0] == "LS")
		{
			req_all_tlm(req_ls_tlm);
			usleep(1000 *MILIHERTZ);  // changed
			//Sleep(1000); //ls gets updated every 1 sec, so we add a delay
		}else if(lines[0] == "STATE")
		{
			if(lines.size() == 3)
			{
				int sel = std::stoi(lines[1]); //which drive selected
				int new_state = std::stoi(lines[2]);
				 
				if(new_state >= 0 && new_state <= 6 && sel >= 0 && sel <= 63)
				{
					MK2::send_change_state_cmd(sel, new_state);
				}else
				{
					std::cout << "Invalid State" << std::endl;
				}

			}else
			{
				std::cout << "Invalid Parameters" << std::endl;
			}
		}else
		{
			std::cout << "COMMAND NOT FOUND" << std::endl;
		}
		usleep(300 *MILIHERTZ);  // changed
		//Sleep(300);

		while(wait_some_more)
		{
			wait_some_more = false;
			usleep(100 *MILIHERTZ);  // changed
			//Sleep(100);
			 
		}

	}
#endif

	pthread_join( recvThreadID, NULL);		// <-- added
	pthread_join( controlThreadID, NULL);

	close(clientSock);
	//closesocket(clientSock);
	//WSACleanup();

	return 0;
}
