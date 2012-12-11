; Auto-generated. Do not edit!


(cl:in-package dasl_mocap-msg)


;//! \htmlinclude pc2quadcopter.msg.html

(cl:defclass <pc2quadcopter> (roslisp-msg-protocol:ros-message)
  ((position
    :reader position
    :initarg :position
    :type dasl_mocap-msg:Vector3F
    :initform (cl:make-instance 'dasl_mocap-msg:Vector3F))
   (speed
    :reader speed
    :initarg :speed
    :type dasl_mocap-msg:Vector3F
    :initform (cl:make-instance 'dasl_mocap-msg:Vector3F))
   (orientation
    :reader orientation
    :initarg :orientation
    :type dasl_mocap-msg:Vector3F
    :initform (cl:make-instance 'dasl_mocap-msg:Vector3F))
   (roll
    :reader roll
    :initarg :roll
    :type cl:float
    :initform 0.0)
   (pitch
    :reader pitch
    :initarg :pitch
    :type cl:float
    :initform 0.0)
   (yaw
    :reader yaw
    :initarg :yaw
    :type cl:float
    :initform 0.0)
   (thrust
    :reader thrust
    :initarg :thrust
    :type cl:float
    :initform 0.0))
)

(cl:defclass pc2quadcopter (<pc2quadcopter>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <pc2quadcopter>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'pc2quadcopter)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name dasl_mocap-msg:<pc2quadcopter> is deprecated: use dasl_mocap-msg:pc2quadcopter instead.")))

(cl:ensure-generic-function 'position-val :lambda-list '(m))
(cl:defmethod position-val ((m <pc2quadcopter>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader dasl_mocap-msg:position-val is deprecated.  Use dasl_mocap-msg:position instead.")
  (position m))

(cl:ensure-generic-function 'speed-val :lambda-list '(m))
(cl:defmethod speed-val ((m <pc2quadcopter>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader dasl_mocap-msg:speed-val is deprecated.  Use dasl_mocap-msg:speed instead.")
  (speed m))

(cl:ensure-generic-function 'orientation-val :lambda-list '(m))
(cl:defmethod orientation-val ((m <pc2quadcopter>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader dasl_mocap-msg:orientation-val is deprecated.  Use dasl_mocap-msg:orientation instead.")
  (orientation m))

(cl:ensure-generic-function 'roll-val :lambda-list '(m))
(cl:defmethod roll-val ((m <pc2quadcopter>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader dasl_mocap-msg:roll-val is deprecated.  Use dasl_mocap-msg:roll instead.")
  (roll m))

(cl:ensure-generic-function 'pitch-val :lambda-list '(m))
(cl:defmethod pitch-val ((m <pc2quadcopter>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader dasl_mocap-msg:pitch-val is deprecated.  Use dasl_mocap-msg:pitch instead.")
  (pitch m))

(cl:ensure-generic-function 'yaw-val :lambda-list '(m))
(cl:defmethod yaw-val ((m <pc2quadcopter>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader dasl_mocap-msg:yaw-val is deprecated.  Use dasl_mocap-msg:yaw instead.")
  (yaw m))

(cl:ensure-generic-function 'thrust-val :lambda-list '(m))
(cl:defmethod thrust-val ((m <pc2quadcopter>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader dasl_mocap-msg:thrust-val is deprecated.  Use dasl_mocap-msg:thrust instead.")
  (thrust m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <pc2quadcopter>) ostream)
  "Serializes a message object of type '<pc2quadcopter>"
  (roslisp-msg-protocol:serialize (cl:slot-value msg 'position) ostream)
  (roslisp-msg-protocol:serialize (cl:slot-value msg 'speed) ostream)
  (roslisp-msg-protocol:serialize (cl:slot-value msg 'orientation) ostream)
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'roll))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'pitch))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'yaw))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'thrust))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <pc2quadcopter>) istream)
  "Deserializes a message object of type '<pc2quadcopter>"
  (roslisp-msg-protocol:deserialize (cl:slot-value msg 'position) istream)
  (roslisp-msg-protocol:deserialize (cl:slot-value msg 'speed) istream)
  (roslisp-msg-protocol:deserialize (cl:slot-value msg 'orientation) istream)
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'roll) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'pitch) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'yaw) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'thrust) (roslisp-utils:decode-single-float-bits bits)))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<pc2quadcopter>)))
  "Returns string type for a message object of type '<pc2quadcopter>"
  "dasl_mocap/pc2quadcopter")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'pc2quadcopter)))
  "Returns string type for a message object of type 'pc2quadcopter"
  "dasl_mocap/pc2quadcopter")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<pc2quadcopter>)))
  "Returns md5sum for a message object of type '<pc2quadcopter>"
  "eb0ec6ee65ae149f4b3914a1dece2323")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'pc2quadcopter)))
  "Returns md5sum for a message object of type 'pc2quadcopter"
  "eb0ec6ee65ae149f4b3914a1dece2323")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<pc2quadcopter>)))
  "Returns full string definition for message of type '<pc2quadcopter>"
  (cl:format cl:nil "Vector3F position~%Vector3F speed~%Vector3F orientation~%float32 roll~%float32 pitch~%float32 yaw~%float32 thrust~%~%================================================================================~%MSG: dasl_mocap/Vector3F~%float32 x~%float32 y~%float32 z~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'pc2quadcopter)))
  "Returns full string definition for message of type 'pc2quadcopter"
  (cl:format cl:nil "Vector3F position~%Vector3F speed~%Vector3F orientation~%float32 roll~%float32 pitch~%float32 yaw~%float32 thrust~%~%================================================================================~%MSG: dasl_mocap/Vector3F~%float32 x~%float32 y~%float32 z~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <pc2quadcopter>))
  (cl:+ 0
     (roslisp-msg-protocol:serialization-length (cl:slot-value msg 'position))
     (roslisp-msg-protocol:serialization-length (cl:slot-value msg 'speed))
     (roslisp-msg-protocol:serialization-length (cl:slot-value msg 'orientation))
     4
     4
     4
     4
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <pc2quadcopter>))
  "Converts a ROS message object to a list"
  (cl:list 'pc2quadcopter
    (cl:cons ':position (position msg))
    (cl:cons ':speed (speed msg))
    (cl:cons ':orientation (orientation msg))
    (cl:cons ':roll (roll msg))
    (cl:cons ':pitch (pitch msg))
    (cl:cons ':yaw (yaw msg))
    (cl:cons ':thrust (thrust msg))
))
