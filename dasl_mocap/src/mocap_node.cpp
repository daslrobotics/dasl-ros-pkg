/// \author <a href="mailto:graeve@ais.uni-bonn.de">Kathrin Gräve</a>
///
/// ROS node that translates motion capture data from an OptiTrack rig to tf transforms.
/// The node receives the binary packages that are streamed by the Arena software,
/// decodes them and broadcasts the poses of rigid bodies as tf transforms.
///
/// Currently, this node supports the NatNet streaming protocol v1.4.

// Local includes
//#include "Socket.h"
#include "MOCAPSocket.h"
//#include "mocap_datapackets.h"
#include "mocap/RigidBody.h"
#include "skeletons.h"

// ROS includes
#include <ros/ros.h>
#include <tf/transform_broadcaster.h>

// System includes
#include <string>
#include <unistd.h>

////////////////////////////////////////////////////////////////////////
// Constants

// ip on multicast group - cannot be changed in Arena
const std::string MULTICAST_IP = "239.255.42.99";

const std::string MOCAP_MODEL_KEY = "mocap_model";
const char ** DEFAULT_MOCAP_MODEL = OBJECT;

const int LOCAL_PORT = 1001;

const char * MOCAP_CHILD[] = {"quadrotor"};
//const char * MOCAP_CHILD[] = {"gantry_base", "target"};

////////////////////////////////////////////////////////////////////////

void processMocapData( const char** mocap_model )
{
  //UdpMulticastSocket multicast_client_socket( LOCAL_PORT, MULTICAST_IP );
		
  MOCAPSocket Socket;

  static tf::TransformBroadcaster br;

  //ushort payload;
  //MoCapDataFormat format;
  while( true )
  {	
    //int numberOfPackets = 0;
    //bool packetread = false;

    //int trials = 0;

    //while( !packetread && trials < 100 )
    //{
      //int numBytes = 0;

      //do
      //{
	// Receive data form mocap device
	//numBytes = multicast_client_socket.recv();

	// Parse mocap data
	//if( numBytes > 0 )
	//{
	  //const char* buffer = multicast_client_socket.getBuffer();
	  //unsigned short header = *((unsigned short*)(&buffer[0]));
					
	  // Look for the beginning of a NatNet package
	  //if (header == 7)
	  //{
	    //payload = *((ushort*) &buffer[2]);
	    //format.parse(buffer,payload);
	    //packetread = true;
	    //numberOfPackets++;
	  //}
	  // else skip packet
	//}


      //} while( numBytes > 0 );

      // Don't try again immediately
      //if( !packetread )
      //{
	//usleep( 10 );
	//trials++;
      //}
    //}
		
    // Once a mocap package has been received and parsed, publish the data using tf
    if(Socket.Read()>0)
    //if( packetread )
    {
      ros::Time timestamp( ros::Time::now() );
			
      //for( int i = 0; i < format.model[0].numRigidBodies; i++ )
      //for( int i = 0; i < 2; i++ )
      for( int i = 0; i < 1; i++ )
      {

	tf::Transform transform;
	// Translate mocap data from mm --> m to be compatible with rviz

	// Change y and z due to mocap calibration

	transform.setOrigin( tf::Vector3(Socket.rigidBody[i].x / 1000.0f,-Socket.rigidBody[i].z / 1000.0f,Socket.rigidBody[i].y / 1000.0f ) );

	// Change y and z due to mocap calibration
			    
	tf::Quaternion q( Socket.rigidBody[i].qx, Socket.rigidBody[i].qz, Socket.rigidBody[i].qy, Socket.rigidBody[i].qw ) ;
			    
	transform.setRotation(q.inverse());		// Handle 

	int rigid_body_id = abs(Socket.rigidBody[i].ID);

	const char* rigid_body_name = MOCAP_CHILD[i]; //"gantry_base"; //mocap_model[rigid_body_id];
	br.sendTransform(tf::StampedTransform(transform, timestamp, "mocap_origin", std::string( rigid_body_name ) ));

	//printf("Rigid body id and name: %x %s \n", rigid_body_id, rigid_body_name);

      }
    }
  }
}

////////////////////////////////////////////////////////////////////////

int main( int argc, char* argv[] )
{ 
  
  // Initialize ROS node
  ros::init(argc, argv, "mocap_node", ros::init_options::NoSigintHandler );
  ros::NodeHandle n("~");

  // Get configuration from ROS parameter server  
  const char** mocap_model( DEFAULT_MOCAP_MODEL );
  if( n.hasParam( MOCAP_MODEL_KEY ) )
  {    std::string tmp;
    if( n.getParam( MOCAP_MODEL_KEY, tmp ) )
    {
      if( tmp == "SKELETON_WITH_TOES" )
	mocap_model = SKELETON_WITH_TOES;
      else if( tmp == "SKELETON_WITHOUT_TOES" )
	mocap_model = SKELETON_WITHOUT_TOES;
      else if( tmp == "OBJECT" )
	mocap_model = OBJECT;
    }
  }
  
  // Process mocap data until SIGINT
  processMocapData( mocap_model );

  return 0;
}
