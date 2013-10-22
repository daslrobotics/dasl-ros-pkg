/* Auto-generated by genmsg_cpp for file /home/dasl/rosbuild_ws/roscopter/msg/Attitude.msg */
#ifndef ROSCOPTER_MESSAGE_ATTITUDE_H
#define ROSCOPTER_MESSAGE_ATTITUDE_H
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


namespace roscopter
{
template <class ContainerAllocator>
struct Attitude_ {
  typedef Attitude_<ContainerAllocator> Type;

  Attitude_()
  : roll(0.0)
  , pitch(0.0)
  , yaw(0.0)
  , rollspeed(0.0)
  , pitchspeed(0.0)
  , yawspeed(0.0)
  {
  }

  Attitude_(const ContainerAllocator& _alloc)
  : roll(0.0)
  , pitch(0.0)
  , yaw(0.0)
  , rollspeed(0.0)
  , pitchspeed(0.0)
  , yawspeed(0.0)
  {
  }

  typedef float _roll_type;
  float roll;

  typedef float _pitch_type;
  float pitch;

  typedef float _yaw_type;
  float yaw;

  typedef float _rollspeed_type;
  float rollspeed;

  typedef float _pitchspeed_type;
  float pitchspeed;

  typedef float _yawspeed_type;
  float yawspeed;


  typedef boost::shared_ptr< ::roscopter::Attitude_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::roscopter::Attitude_<ContainerAllocator>  const> ConstPtr;
  boost::shared_ptr<std::map<std::string, std::string> > __connection_header;
}; // struct Attitude
typedef  ::roscopter::Attitude_<std::allocator<void> > Attitude;

typedef boost::shared_ptr< ::roscopter::Attitude> AttitudePtr;
typedef boost::shared_ptr< ::roscopter::Attitude const> AttitudeConstPtr;


template<typename ContainerAllocator>
std::ostream& operator<<(std::ostream& s, const  ::roscopter::Attitude_<ContainerAllocator> & v)
{
  ros::message_operations::Printer< ::roscopter::Attitude_<ContainerAllocator> >::stream(s, "", v);
  return s;}

} // namespace roscopter

namespace ros
{
namespace message_traits
{
template<class ContainerAllocator> struct IsMessage< ::roscopter::Attitude_<ContainerAllocator> > : public TrueType {};
template<class ContainerAllocator> struct IsMessage< ::roscopter::Attitude_<ContainerAllocator>  const> : public TrueType {};
template<class ContainerAllocator>
struct MD5Sum< ::roscopter::Attitude_<ContainerAllocator> > {
  static const char* value() 
  {
    return "a0ed90d05663fe955b3f4e85f83050df";
  }

  static const char* value(const  ::roscopter::Attitude_<ContainerAllocator> &) { return value(); } 
  static const uint64_t static_value1 = 0xa0ed90d05663fe95ULL;
  static const uint64_t static_value2 = 0x5b3f4e85f83050dfULL;
};

template<class ContainerAllocator>
struct DataType< ::roscopter::Attitude_<ContainerAllocator> > {
  static const char* value() 
  {
    return "roscopter/Attitude";
  }

  static const char* value(const  ::roscopter::Attitude_<ContainerAllocator> &) { return value(); } 
};

template<class ContainerAllocator>
struct Definition< ::roscopter::Attitude_<ContainerAllocator> > {
  static const char* value() 
  {
    return "#roll                      : Roll angle (rad) (float)     \n\
#pitch                     : Pitch angle (rad) (float) \n\
#yaw                       : Yaw angle (rad) (float)\n\
#rollspeed                 : Roll angular speed (rad/s) (float)\n\
#pitchspeed                : Pitch angular speed (rad/s) (float)\n\
#yawspeed                  : Yaw angular speed (rad/s) (float)    \n\
\n\
float32 roll\n\
float32 pitch\n\
float32 yaw \n\
float32 rollspeed \n\
float32 pitchspeed\n\
float32 yawspeed\n\
\n\
";
  }

  static const char* value(const  ::roscopter::Attitude_<ContainerAllocator> &) { return value(); } 
};

template<class ContainerAllocator> struct IsFixedSize< ::roscopter::Attitude_<ContainerAllocator> > : public TrueType {};
} // namespace message_traits
} // namespace ros

namespace ros
{
namespace serialization
{

template<class ContainerAllocator> struct Serializer< ::roscopter::Attitude_<ContainerAllocator> >
{
  template<typename Stream, typename T> inline static void allInOne(Stream& stream, T m)
  {
    stream.next(m.roll);
    stream.next(m.pitch);
    stream.next(m.yaw);
    stream.next(m.rollspeed);
    stream.next(m.pitchspeed);
    stream.next(m.yawspeed);
  }

  ROS_DECLARE_ALLINONE_SERIALIZER;
}; // struct Attitude_
} // namespace serialization
} // namespace ros

namespace ros
{
namespace message_operations
{

template<class ContainerAllocator>
struct Printer< ::roscopter::Attitude_<ContainerAllocator> >
{
  template<typename Stream> static void stream(Stream& s, const std::string& indent, const  ::roscopter::Attitude_<ContainerAllocator> & v) 
  {
    s << indent << "roll: ";
    Printer<float>::stream(s, indent + "  ", v.roll);
    s << indent << "pitch: ";
    Printer<float>::stream(s, indent + "  ", v.pitch);
    s << indent << "yaw: ";
    Printer<float>::stream(s, indent + "  ", v.yaw);
    s << indent << "rollspeed: ";
    Printer<float>::stream(s, indent + "  ", v.rollspeed);
    s << indent << "pitchspeed: ";
    Printer<float>::stream(s, indent + "  ", v.pitchspeed);
    s << indent << "yawspeed: ";
    Printer<float>::stream(s, indent + "  ", v.yawspeed);
  }
};


} // namespace message_operations
} // namespace ros

#endif // ROSCOPTER_MESSAGE_ATTITUDE_H

