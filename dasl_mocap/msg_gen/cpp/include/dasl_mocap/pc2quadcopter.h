/* Auto-generated by genmsg_cpp for file /home/dasl/ros/dasl-ros-pkg/dasl_mocap/msg/pc2quadcopter.msg */
#ifndef DASL_MOCAP_MESSAGE_PC2QUADCOPTER_H
#define DASL_MOCAP_MESSAGE_PC2QUADCOPTER_H
#include <string>
#include <vector>
#include <map>
#include <ostream>
#include "ros/serialization.h"
#include "ros/builtin_message_traits.h"
#include "ros/message_operations.h"
#include "ros/time.h"

#include "ros/macros.h"

#include "ros/assert.h"

#include "dasl_mocap/Vector3F.h"
#include "dasl_mocap/Vector3F.h"
#include "dasl_mocap/Vector3F.h"

namespace dasl_mocap
{
template <class ContainerAllocator>
struct pc2quadcopter_ {
  typedef pc2quadcopter_<ContainerAllocator> Type;

  pc2quadcopter_()
  : position()
  , speed()
  , orientation()
  , roll(0.0)
  , pitch(0.0)
  , yaw(0.0)
  , thrust(0.0)
  {
  }

  pc2quadcopter_(const ContainerAllocator& _alloc)
  : position(_alloc)
  , speed(_alloc)
  , orientation(_alloc)
  , roll(0.0)
  , pitch(0.0)
  , yaw(0.0)
  , thrust(0.0)
  {
  }

  typedef  ::dasl_mocap::Vector3F_<ContainerAllocator>  _position_type;
   ::dasl_mocap::Vector3F_<ContainerAllocator>  position;

  typedef  ::dasl_mocap::Vector3F_<ContainerAllocator>  _speed_type;
   ::dasl_mocap::Vector3F_<ContainerAllocator>  speed;

  typedef  ::dasl_mocap::Vector3F_<ContainerAllocator>  _orientation_type;
   ::dasl_mocap::Vector3F_<ContainerAllocator>  orientation;

  typedef float _roll_type;
  float roll;

  typedef float _pitch_type;
  float pitch;

  typedef float _yaw_type;
  float yaw;

  typedef float _thrust_type;
  float thrust;


  typedef boost::shared_ptr< ::dasl_mocap::pc2quadcopter_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::dasl_mocap::pc2quadcopter_<ContainerAllocator>  const> ConstPtr;
  boost::shared_ptr<std::map<std::string, std::string> > __connection_header;
}; // struct pc2quadcopter
typedef  ::dasl_mocap::pc2quadcopter_<std::allocator<void> > pc2quadcopter;

typedef boost::shared_ptr< ::dasl_mocap::pc2quadcopter> pc2quadcopterPtr;
typedef boost::shared_ptr< ::dasl_mocap::pc2quadcopter const> pc2quadcopterConstPtr;


template<typename ContainerAllocator>
std::ostream& operator<<(std::ostream& s, const  ::dasl_mocap::pc2quadcopter_<ContainerAllocator> & v)
{
  ros::message_operations::Printer< ::dasl_mocap::pc2quadcopter_<ContainerAllocator> >::stream(s, "", v);
  return s;}

} // namespace dasl_mocap

namespace ros
{
namespace message_traits
{
template<class ContainerAllocator> struct IsMessage< ::dasl_mocap::pc2quadcopter_<ContainerAllocator> > : public TrueType {};
template<class ContainerAllocator> struct IsMessage< ::dasl_mocap::pc2quadcopter_<ContainerAllocator>  const> : public TrueType {};
template<class ContainerAllocator>
struct MD5Sum< ::dasl_mocap::pc2quadcopter_<ContainerAllocator> > {
  static const char* value() 
  {
    return "eb0ec6ee65ae149f4b3914a1dece2323";
  }

  static const char* value(const  ::dasl_mocap::pc2quadcopter_<ContainerAllocator> &) { return value(); } 
  static const uint64_t static_value1 = 0xeb0ec6ee65ae149fULL;
  static const uint64_t static_value2 = 0x4b3914a1dece2323ULL;
};

template<class ContainerAllocator>
struct DataType< ::dasl_mocap::pc2quadcopter_<ContainerAllocator> > {
  static const char* value() 
  {
    return "dasl_mocap/pc2quadcopter";
  }

  static const char* value(const  ::dasl_mocap::pc2quadcopter_<ContainerAllocator> &) { return value(); } 
};

template<class ContainerAllocator>
struct Definition< ::dasl_mocap::pc2quadcopter_<ContainerAllocator> > {
  static const char* value() 
  {
    return "Vector3F position\n\
Vector3F speed\n\
Vector3F orientation\n\
float32 roll\n\
float32 pitch\n\
float32 yaw\n\
float32 thrust\n\
\n\
================================================================================\n\
MSG: dasl_mocap/Vector3F\n\
float32 x\n\
float32 y\n\
float32 z\n\
\n\
";
  }

  static const char* value(const  ::dasl_mocap::pc2quadcopter_<ContainerAllocator> &) { return value(); } 
};

template<class ContainerAllocator> struct IsFixedSize< ::dasl_mocap::pc2quadcopter_<ContainerAllocator> > : public TrueType {};
} // namespace message_traits
} // namespace ros

namespace ros
{
namespace serialization
{

template<class ContainerAllocator> struct Serializer< ::dasl_mocap::pc2quadcopter_<ContainerAllocator> >
{
  template<typename Stream, typename T> inline static void allInOne(Stream& stream, T m)
  {
    stream.next(m.position);
    stream.next(m.speed);
    stream.next(m.orientation);
    stream.next(m.roll);
    stream.next(m.pitch);
    stream.next(m.yaw);
    stream.next(m.thrust);
  }

  ROS_DECLARE_ALLINONE_SERIALIZER;
}; // struct pc2quadcopter_
} // namespace serialization
} // namespace ros

namespace ros
{
namespace message_operations
{

template<class ContainerAllocator>
struct Printer< ::dasl_mocap::pc2quadcopter_<ContainerAllocator> >
{
  template<typename Stream> static void stream(Stream& s, const std::string& indent, const  ::dasl_mocap::pc2quadcopter_<ContainerAllocator> & v) 
  {
    s << indent << "position: ";
s << std::endl;
    Printer< ::dasl_mocap::Vector3F_<ContainerAllocator> >::stream(s, indent + "  ", v.position);
    s << indent << "speed: ";
s << std::endl;
    Printer< ::dasl_mocap::Vector3F_<ContainerAllocator> >::stream(s, indent + "  ", v.speed);
    s << indent << "orientation: ";
s << std::endl;
    Printer< ::dasl_mocap::Vector3F_<ContainerAllocator> >::stream(s, indent + "  ", v.orientation);
    s << indent << "roll: ";
    Printer<float>::stream(s, indent + "  ", v.roll);
    s << indent << "pitch: ";
    Printer<float>::stream(s, indent + "  ", v.pitch);
    s << indent << "yaw: ";
    Printer<float>::stream(s, indent + "  ", v.yaw);
    s << indent << "thrust: ";
    Printer<float>::stream(s, indent + "  ", v.thrust);
  }
};


} // namespace message_operations
} // namespace ros

#endif // DASL_MOCAP_MESSAGE_PC2QUADCOPTER_H

