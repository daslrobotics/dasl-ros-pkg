/* Auto-generated by genmsg_cpp for file /home/dasl/rosbuild_ws/dasl-ros-pkg/dasl_mocap/msg/quadcopter2pc.msg */
#ifndef DASL_MOCAP_MESSAGE_QUADCOPTER2PC_H
#define DASL_MOCAP_MESSAGE_QUADCOPTER2PC_H
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


namespace dasl_mocap
{
template <class ContainerAllocator>
struct quadcopter2pc_ {
  typedef quadcopter2pc_<ContainerAllocator> Type;

  quadcopter2pc_()
  {
  }

  quadcopter2pc_(const ContainerAllocator& _alloc)
  {
  }


  typedef boost::shared_ptr< ::dasl_mocap::quadcopter2pc_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::dasl_mocap::quadcopter2pc_<ContainerAllocator>  const> ConstPtr;
  boost::shared_ptr<std::map<std::string, std::string> > __connection_header;
}; // struct quadcopter2pc
typedef  ::dasl_mocap::quadcopter2pc_<std::allocator<void> > quadcopter2pc;

typedef boost::shared_ptr< ::dasl_mocap::quadcopter2pc> quadcopter2pcPtr;
typedef boost::shared_ptr< ::dasl_mocap::quadcopter2pc const> quadcopter2pcConstPtr;


template<typename ContainerAllocator>
std::ostream& operator<<(std::ostream& s, const  ::dasl_mocap::quadcopter2pc_<ContainerAllocator> & v)
{
  ros::message_operations::Printer< ::dasl_mocap::quadcopter2pc_<ContainerAllocator> >::stream(s, "", v);
  return s;}

} // namespace dasl_mocap

namespace ros
{
namespace message_traits
{
template<class ContainerAllocator> struct IsMessage< ::dasl_mocap::quadcopter2pc_<ContainerAllocator> > : public TrueType {};
template<class ContainerAllocator> struct IsMessage< ::dasl_mocap::quadcopter2pc_<ContainerAllocator>  const> : public TrueType {};
template<class ContainerAllocator>
struct MD5Sum< ::dasl_mocap::quadcopter2pc_<ContainerAllocator> > {
  static const char* value() 
  {
    return "d41d8cd98f00b204e9800998ecf8427e";
  }

  static const char* value(const  ::dasl_mocap::quadcopter2pc_<ContainerAllocator> &) { return value(); } 
  static const uint64_t static_value1 = 0xd41d8cd98f00b204ULL;
  static const uint64_t static_value2 = 0xe9800998ecf8427eULL;
};

template<class ContainerAllocator>
struct DataType< ::dasl_mocap::quadcopter2pc_<ContainerAllocator> > {
  static const char* value() 
  {
    return "dasl_mocap/quadcopter2pc";
  }

  static const char* value(const  ::dasl_mocap::quadcopter2pc_<ContainerAllocator> &) { return value(); } 
};

template<class ContainerAllocator>
struct Definition< ::dasl_mocap::quadcopter2pc_<ContainerAllocator> > {
  static const char* value() 
  {
    return "\n\
";
  }

  static const char* value(const  ::dasl_mocap::quadcopter2pc_<ContainerAllocator> &) { return value(); } 
};

template<class ContainerAllocator> struct IsFixedSize< ::dasl_mocap::quadcopter2pc_<ContainerAllocator> > : public TrueType {};
} // namespace message_traits
} // namespace ros

namespace ros
{
namespace serialization
{

template<class ContainerAllocator> struct Serializer< ::dasl_mocap::quadcopter2pc_<ContainerAllocator> >
{
  template<typename Stream, typename T> inline static void allInOne(Stream& stream, T m)
  {
  }

  ROS_DECLARE_ALLINONE_SERIALIZER;
}; // struct quadcopter2pc_
} // namespace serialization
} // namespace ros

namespace ros
{
namespace message_operations
{

template<class ContainerAllocator>
struct Printer< ::dasl_mocap::quadcopter2pc_<ContainerAllocator> >
{
  template<typename Stream> static void stream(Stream& s, const std::string& indent, const  ::dasl_mocap::quadcopter2pc_<ContainerAllocator> & v) 
  {
  }
};


} // namespace message_operations
} // namespace ros

#endif // DASL_MOCAP_MESSAGE_QUADCOPTER2PC_H

