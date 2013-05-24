#include "MOCAPSocket.h"

using namespace std;

MOCAPSocket::MOCAPSocket()
{
	DataSocket = socket( AF_INET, SOCK_DGRAM, 0 );
	int value = 1;
	int retvalue = setsockopt(DataSocket,SOL_SOCKET,SO_REUSEADDR,(char*)&value,sizeof(value));
	if (retvalue == SOCKET_ERROR)
	{
		cout << "ERROR setting socket options" <<endl;
		return;
	}
	memset(&SocketAddrIn,0,sizeof(SocketAddrIn));
	SocketAddrIn.sin_family = AF_INET;
	SocketAddrIn.sin_port = htons(PORT_DATA);
	SocketAddrIn.sin_addr.s_addr= htonl( INADDR_ANY );

	if (bind(DataSocket,(struct sockaddr *)&SocketAddrIn,sizeof(struct sockaddr))== SOCKET_ERROR)
	{
		cout << "ERROR binding a socket" <<endl;
		return;
	}
	cout << SocketAddrIn.sin_addr.s_addr <<endl;
	Mreq.imr_multiaddr.s_addr = inet_addr(MULTICAST_ADDRESS);
	Mreq.imr_interface = SocketAddrIn.sin_addr;
	retvalue = setsockopt(DataSocket, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&Mreq, sizeof(Mreq));
	if (retvalue == -1) return;
	//Nonblocking port options
	int flags = fcntl( DataSocket, F_GETFL , 0 );
	retvalue = fcntl(DataSocket, F_SETFL, flags | O_NONBLOCK);
}

MOCAPSocket::~MOCAPSocket() {
        // TODO Auto-generated destructor stub
}

int MOCAPSocket::Read()
{
	//This function tries for 100 times to read the data from the Optitrack PC
	int numberOfPackets = 0;
	bool packetread = false;
	int trials = 0;
	while( !packetread && trials < 100 )
	{
		int numBytes = 0;
		do
		{
		  // Receive data from Optitrack PC
		  numBytes = Receive();
		  // Parse Optitrack data
		  if( numBytes > 0 )
		  {
			  unsigned short header = *((unsigned short*)(&buffer[0]));
			  // Look for the beginning of a NatNet package
			  if (header == 7)
			  {
					  packetread = true;
					  numberOfPackets++;
					  ParseData(buffer);
			  }
			// else skip packet
			}
		} while( numBytes > 0 );
	}
	return packetread;
}
int MOCAPSocket::Receive(void)
{
	//function that receives the data from the Socket
	//returns the number of the received bytes
  sockaddr_in remote_addr;
  int addr_len = sizeof(struct sockaddr);
  int status = recvfrom(
    DataSocket,
    buffer,
    MAXRECV,
    0,
    (sockaddr *)&remote_addr,
    (socklen_t*)&addr_len);
  return status;
}

void MOCAPSocket::ParseData(char* pData)
{
	// Parses the received data
    char *ptr = pData;
    // message ID 2 bytes
    int MessageID = 0;
    memcpy(&MessageID, ptr, 2); ptr += 2;
    // size 2 bytes
    int nBytes = 0;
    memcpy(&nBytes, ptr, 2); ptr += 2;
    //cByte count
    //Frame Nubmer 4 bytes
    memcpy(&frameNmbr, ptr, 4); ptr += 4;

    //Number of known Marker sets
    //4 bytes
    int KnwMarkerSetNmbr = 0;
    memcpy(&KnwMarkerSetNmbr , ptr, 4); ptr += 4;

    for(int i=0;i<KnwMarkerSetNmbr;i++)
    {
    	//For each know marker parse but forget
		//Markerset name
		char szName[256];
		strcpy(szName, ptr);//name n bytes
		int nDataBytes = (int) strlen(szName) + 1;//move for name length + EOL
		ptr += nDataBytes;


		// Marker Data
		int nMarkers = 0; memcpy(&nMarkers, ptr, 4); ptr += 4;
		//Marker Count 4 bytes
		for(int j=0;j<nMarkers;j++)
		{
			ptr += 12;//X Y Z data each 4 bytes = 12 bytes
		}
    }
    //Number of Unknown Marker sets
    int UKnwMarkerNmbr = 0;
	memcpy(&UKnwMarkerNmbr , ptr, 4); ptr += 4;
	//Unknown Marker Set Number 4 bytes
	for(int j=0;j<UKnwMarkerNmbr;j++)
	{
			ptr += 12;//X Y Z data each 4 bytes = 12 bytes
	}
	//Rigid bodies ->Trackables
	//Number of rigid bodies, 4 bytes
	int RigidBodyNmbr = 0;
	memcpy(&RigidBodyNmbr , ptr, 4); ptr += 4;
	//Rigid body number
	for(int i=0;i<RigidBodyNmbr;i++)
	{
		// For each rigid body
		// read rigid body data position/orientation
		// ID is the only available info for the rigid body
		int ID = 0; memcpy(&ID, ptr, 4); ptr += 4;
		//Save old data to calculate speed
		rigidBody[i].x_old=rigidBody[i].x;
		rigidBody[i].y_old=rigidBody[i].y;
		rigidBody[i].z_old=rigidBody[i].z;
		// Copy position [mm] and orientation (quaternion) data
		// This data is saved as Rigid bodies
		rigidBody[i].x = 0.0f; memcpy(&rigidBody[i].x, ptr, 4); ptr += 4;
		rigidBody[i].y = 0.0f; memcpy(&rigidBody[i].y, ptr, 4); ptr += 4;
		rigidBody[i].z = 0.0f; memcpy(&rigidBody[i].z, ptr, 4); ptr += 4;
		rigidBody[i].qx = 0; memcpy(&rigidBody[i].qx, ptr, 4); ptr += 4;
		rigidBody[i].qy = 0; memcpy(&rigidBody[i].qy, ptr, 4); ptr += 4;
		rigidBody[i].qz = 0; memcpy(&rigidBody[i].qz, ptr, 4); ptr += 4;
		rigidBody[i].qw = 0; memcpy(&rigidBody[i].qw, ptr, 4); ptr += 4;
		// Calculate Euler data
		Q2Euler(&rigidBody[i]);
		//All the markers that make up the Rigid Body
		int nRigidBodyMarkers = 0;memcpy(&nRigidBodyMarkers , ptr, 4); ptr += 4;
		int nBytes = nRigidBodyMarkers*3*sizeof(float);ptr += nBytes;//XYZ
		nBytes = nRigidBodyMarkers*sizeof(int);ptr += nBytes;//MarkerIDs
		nBytes = nRigidBodyMarkers*sizeof(float);ptr += nBytes;//MarkerSize
		ptr += 4;
	}
}
void MOCAPSocket::Q2Euler(RigidBodyObject* RB)
{
	//Calculate Euler angles form Quaternions
	float test = RB[0].qx*RB[0].qy+RB[0].qz*RB[0].qw;
	float yaw,pitch,roll;
	if (test>0.499)
	{
		yaw = (float) 2.0f + atan2(RB[0].qx,RB[0].qw);
		pitch = (float) PI/2.0f;
		roll = 0;
	}
	else if (test<-0.499)
	{
		yaw = (float)-2.0f + atan2(RB[0].qx,RB[0].qw);
		pitch = (float)-PI/2.0f;
		roll = 0;
	}
	else
	{
		float sqx = RB[0].qx*RB[0].qx;
		float sqy = RB[0].qy*RB[0].qy;
		float sqz = RB[0].qz*RB[0].qz;
		yaw = atan2(2.0*RB[0].qy*RB[0].qw-2.0*RB[0].qx*RB[0].qz,1-2.0*sqy-2.0*sqz);
		pitch = asin(2.0*test);
		roll =atan2(2.0*RB[0].qx*RB[0].qw-2.0*RB[0].qy*RB[0].qz,1-2.0*sqx-2.0*sqz);
	}
	RB[0].yaw=Rad2Deg(yaw);
	RB[0].pitch=Rad2Deg(pitch);
	RB[0].roll=Rad2Deg(roll);
}
float MOCAPSocket::Rad2Deg(float angle)
{
	//Radians to degrees
    return (float)(angle*180.0f/PI);
}
