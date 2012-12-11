; Auto-generated. Do not edit!


(cl:in-package dasl_mocap-msg)


;//! \htmlinclude quadcopter2pc.msg.html

(cl:defclass <quadcopter2pc> (roslisp-msg-protocol:ros-message)
  ()
)

(cl:defclass quadcopter2pc (<quadcopter2pc>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <quadcopter2pc>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'quadcopter2pc)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name dasl_mocap-msg:<quadcopter2pc> is deprecated: use dasl_mocap-msg:quadcopter2pc instead.")))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <quadcopter2pc>) ostream)
  "Serializes a message object of type '<quadcopter2pc>"
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <quadcopter2pc>) istream)
  "Deserializes a message object of type '<quadcopter2pc>"
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<quadcopter2pc>)))
  "Returns string type for a message object of type '<quadcopter2pc>"
  "dasl_mocap/quadcopter2pc")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'quadcopter2pc)))
  "Returns string type for a message object of type 'quadcopter2pc"
  "dasl_mocap/quadcopter2pc")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<quadcopter2pc>)))
  "Returns md5sum for a message object of type '<quadcopter2pc>"
  "d41d8cd98f00b204e9800998ecf8427e")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'quadcopter2pc)))
  "Returns md5sum for a message object of type 'quadcopter2pc"
  "d41d8cd98f00b204e9800998ecf8427e")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<quadcopter2pc>)))
  "Returns full string definition for message of type '<quadcopter2pc>"
  (cl:format cl:nil "~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'quadcopter2pc)))
  "Returns full string definition for message of type 'quadcopter2pc"
  (cl:format cl:nil "~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <quadcopter2pc>))
  (cl:+ 0
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <quadcopter2pc>))
  "Converts a ROS message object to a list"
  (cl:list 'quadcopter2pc
))
