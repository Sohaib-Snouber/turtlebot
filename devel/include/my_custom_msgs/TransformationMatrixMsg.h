// Generated by gencpp from file my_custom_msgs/TransformationMatrixMsg.msg
// DO NOT EDIT!


#ifndef MY_CUSTOM_MSGS_MESSAGE_TRANSFORMATIONMATRIXMSG_H
#define MY_CUSTOM_MSGS_MESSAGE_TRANSFORMATIONMATRIXMSG_H


#include <string>
#include <vector>
#include <memory>

#include <ros/types.h>
#include <ros/serialization.h>
#include <ros/builtin_message_traits.h>
#include <ros/message_operations.h>


namespace my_custom_msgs
{
template <class ContainerAllocator>
struct TransformationMatrixMsg_
{
  typedef TransformationMatrixMsg_<ContainerAllocator> Type;

  TransformationMatrixMsg_()
    : data()
    , rows(0)
    , cols(0)  {
    }
  TransformationMatrixMsg_(const ContainerAllocator& _alloc)
    : data(_alloc)
    , rows(0)
    , cols(0)  {
  (void)_alloc;
    }



   typedef std::vector<float, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<float>> _data_type;
  _data_type data;

   typedef uint8_t _rows_type;
  _rows_type rows;

   typedef uint8_t _cols_type;
  _cols_type cols;





  typedef boost::shared_ptr< ::my_custom_msgs::TransformationMatrixMsg_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::my_custom_msgs::TransformationMatrixMsg_<ContainerAllocator> const> ConstPtr;

}; // struct TransformationMatrixMsg_

typedef ::my_custom_msgs::TransformationMatrixMsg_<std::allocator<void> > TransformationMatrixMsg;

typedef boost::shared_ptr< ::my_custom_msgs::TransformationMatrixMsg > TransformationMatrixMsgPtr;
typedef boost::shared_ptr< ::my_custom_msgs::TransformationMatrixMsg const> TransformationMatrixMsgConstPtr;

// constants requiring out of line definition



template<typename ContainerAllocator>
std::ostream& operator<<(std::ostream& s, const ::my_custom_msgs::TransformationMatrixMsg_<ContainerAllocator> & v)
{
ros::message_operations::Printer< ::my_custom_msgs::TransformationMatrixMsg_<ContainerAllocator> >::stream(s, "", v);
return s;
}


template<typename ContainerAllocator1, typename ContainerAllocator2>
bool operator==(const ::my_custom_msgs::TransformationMatrixMsg_<ContainerAllocator1> & lhs, const ::my_custom_msgs::TransformationMatrixMsg_<ContainerAllocator2> & rhs)
{
  return lhs.data == rhs.data &&
    lhs.rows == rhs.rows &&
    lhs.cols == rhs.cols;
}

template<typename ContainerAllocator1, typename ContainerAllocator2>
bool operator!=(const ::my_custom_msgs::TransformationMatrixMsg_<ContainerAllocator1> & lhs, const ::my_custom_msgs::TransformationMatrixMsg_<ContainerAllocator2> & rhs)
{
  return !(lhs == rhs);
}


} // namespace my_custom_msgs

namespace ros
{
namespace message_traits
{





template <class ContainerAllocator>
struct IsMessage< ::my_custom_msgs::TransformationMatrixMsg_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::my_custom_msgs::TransformationMatrixMsg_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct IsFixedSize< ::my_custom_msgs::TransformationMatrixMsg_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct IsFixedSize< ::my_custom_msgs::TransformationMatrixMsg_<ContainerAllocator> const>
  : FalseType
  { };

template <class ContainerAllocator>
struct HasHeader< ::my_custom_msgs::TransformationMatrixMsg_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct HasHeader< ::my_custom_msgs::TransformationMatrixMsg_<ContainerAllocator> const>
  : FalseType
  { };


template<class ContainerAllocator>
struct MD5Sum< ::my_custom_msgs::TransformationMatrixMsg_<ContainerAllocator> >
{
  static const char* value()
  {
    return "4d0b94d1560ce83cc042f419edb24b18";
  }

  static const char* value(const ::my_custom_msgs::TransformationMatrixMsg_<ContainerAllocator>&) { return value(); }
  static const uint64_t static_value1 = 0x4d0b94d1560ce83cULL;
  static const uint64_t static_value2 = 0xc042f419edb24b18ULL;
};

template<class ContainerAllocator>
struct DataType< ::my_custom_msgs::TransformationMatrixMsg_<ContainerAllocator> >
{
  static const char* value()
  {
    return "my_custom_msgs/TransformationMatrixMsg";
  }

  static const char* value(const ::my_custom_msgs::TransformationMatrixMsg_<ContainerAllocator>&) { return value(); }
};

template<class ContainerAllocator>
struct Definition< ::my_custom_msgs::TransformationMatrixMsg_<ContainerAllocator> >
{
  static const char* value()
  {
    return "float32[] data\n"
"uint8 rows\n"
"uint8 cols\n"
;
  }

  static const char* value(const ::my_custom_msgs::TransformationMatrixMsg_<ContainerAllocator>&) { return value(); }
};

} // namespace message_traits
} // namespace ros

namespace ros
{
namespace serialization
{

  template<class ContainerAllocator> struct Serializer< ::my_custom_msgs::TransformationMatrixMsg_<ContainerAllocator> >
  {
    template<typename Stream, typename T> inline static void allInOne(Stream& stream, T m)
    {
      stream.next(m.data);
      stream.next(m.rows);
      stream.next(m.cols);
    }

    ROS_DECLARE_ALLINONE_SERIALIZER
  }; // struct TransformationMatrixMsg_

} // namespace serialization
} // namespace ros

namespace ros
{
namespace message_operations
{

template<class ContainerAllocator>
struct Printer< ::my_custom_msgs::TransformationMatrixMsg_<ContainerAllocator> >
{
  template<typename Stream> static void stream(Stream& s, const std::string& indent, const ::my_custom_msgs::TransformationMatrixMsg_<ContainerAllocator>& v)
  {
    s << indent << "data[]" << std::endl;
    for (size_t i = 0; i < v.data.size(); ++i)
    {
      s << indent << "  data[" << i << "]: ";
      Printer<float>::stream(s, indent + "  ", v.data[i]);
    }
    s << indent << "rows: ";
    Printer<uint8_t>::stream(s, indent + "  ", v.rows);
    s << indent << "cols: ";
    Printer<uint8_t>::stream(s, indent + "  ", v.cols);
  }
};

} // namespace message_operations
} // namespace ros

#endif // MY_CUSTOM_MSGS_MESSAGE_TRANSFORMATIONMATRIXMSG_H