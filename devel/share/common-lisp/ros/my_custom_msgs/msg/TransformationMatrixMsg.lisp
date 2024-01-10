; Auto-generated. Do not edit!


(cl:in-package my_custom_msgs-msg)


;//! \htmlinclude TransformationMatrixMsg.msg.html

(cl:defclass <TransformationMatrixMsg> (roslisp-msg-protocol:ros-message)
  ((data
    :reader data
    :initarg :data
    :type (cl:vector cl:float)
   :initform (cl:make-array 0 :element-type 'cl:float :initial-element 0.0))
   (rows
    :reader rows
    :initarg :rows
    :type cl:fixnum
    :initform 0)
   (cols
    :reader cols
    :initarg :cols
    :type cl:fixnum
    :initform 0))
)

(cl:defclass TransformationMatrixMsg (<TransformationMatrixMsg>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <TransformationMatrixMsg>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'TransformationMatrixMsg)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name my_custom_msgs-msg:<TransformationMatrixMsg> is deprecated: use my_custom_msgs-msg:TransformationMatrixMsg instead.")))

(cl:ensure-generic-function 'data-val :lambda-list '(m))
(cl:defmethod data-val ((m <TransformationMatrixMsg>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader my_custom_msgs-msg:data-val is deprecated.  Use my_custom_msgs-msg:data instead.")
  (data m))

(cl:ensure-generic-function 'rows-val :lambda-list '(m))
(cl:defmethod rows-val ((m <TransformationMatrixMsg>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader my_custom_msgs-msg:rows-val is deprecated.  Use my_custom_msgs-msg:rows instead.")
  (rows m))

(cl:ensure-generic-function 'cols-val :lambda-list '(m))
(cl:defmethod cols-val ((m <TransformationMatrixMsg>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader my_custom_msgs-msg:cols-val is deprecated.  Use my_custom_msgs-msg:cols instead.")
  (cols m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <TransformationMatrixMsg>) ostream)
  "Serializes a message object of type '<TransformationMatrixMsg>"
  (cl:let ((__ros_arr_len (cl:length (cl:slot-value msg 'data))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_arr_len) ostream))
  (cl:map cl:nil #'(cl:lambda (ele) (cl:let ((bits (roslisp-utils:encode-single-float-bits ele)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream)))
   (cl:slot-value msg 'data))
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'rows)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'cols)) ostream)
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <TransformationMatrixMsg>) istream)
  "Deserializes a message object of type '<TransformationMatrixMsg>"
  (cl:let ((__ros_arr_len 0))
    (cl:setf (cl:ldb (cl:byte 8 0) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) __ros_arr_len) (cl:read-byte istream))
  (cl:setf (cl:slot-value msg 'data) (cl:make-array __ros_arr_len))
  (cl:let ((vals (cl:slot-value msg 'data)))
    (cl:dotimes (i __ros_arr_len)
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:aref vals i) (roslisp-utils:decode-single-float-bits bits))))))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'rows)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'cols)) (cl:read-byte istream))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<TransformationMatrixMsg>)))
  "Returns string type for a message object of type '<TransformationMatrixMsg>"
  "my_custom_msgs/TransformationMatrixMsg")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'TransformationMatrixMsg)))
  "Returns string type for a message object of type 'TransformationMatrixMsg"
  "my_custom_msgs/TransformationMatrixMsg")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<TransformationMatrixMsg>)))
  "Returns md5sum for a message object of type '<TransformationMatrixMsg>"
  "4d0b94d1560ce83cc042f419edb24b18")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'TransformationMatrixMsg)))
  "Returns md5sum for a message object of type 'TransformationMatrixMsg"
  "4d0b94d1560ce83cc042f419edb24b18")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<TransformationMatrixMsg>)))
  "Returns full string definition for message of type '<TransformationMatrixMsg>"
  (cl:format cl:nil "float32[] data~%uint8 rows~%uint8 cols~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'TransformationMatrixMsg)))
  "Returns full string definition for message of type 'TransformationMatrixMsg"
  (cl:format cl:nil "float32[] data~%uint8 rows~%uint8 cols~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <TransformationMatrixMsg>))
  (cl:+ 0
     4 (cl:reduce #'cl:+ (cl:slot-value msg 'data) :key #'(cl:lambda (ele) (cl:declare (cl:ignorable ele)) (cl:+ 4)))
     1
     1
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <TransformationMatrixMsg>))
  "Converts a ROS message object to a list"
  (cl:list 'TransformationMatrixMsg
    (cl:cons ':data (data msg))
    (cl:cons ':rows (rows msg))
    (cl:cons ':cols (cols msg))
))
