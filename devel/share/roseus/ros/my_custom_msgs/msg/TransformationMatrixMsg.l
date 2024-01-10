;; Auto-generated. Do not edit!


(when (boundp 'my_custom_msgs::TransformationMatrixMsg)
  (if (not (find-package "MY_CUSTOM_MSGS"))
    (make-package "MY_CUSTOM_MSGS"))
  (shadow 'TransformationMatrixMsg (find-package "MY_CUSTOM_MSGS")))
(unless (find-package "MY_CUSTOM_MSGS::TRANSFORMATIONMATRIXMSG")
  (make-package "MY_CUSTOM_MSGS::TRANSFORMATIONMATRIXMSG"))

(in-package "ROS")
;;//! \htmlinclude TransformationMatrixMsg.msg.html


(defclass my_custom_msgs::TransformationMatrixMsg
  :super ros::object
  :slots (_data _rows _cols ))

(defmethod my_custom_msgs::TransformationMatrixMsg
  (:init
   (&key
    ((:data __data) (make-array 0 :initial-element 0.0 :element-type :float))
    ((:rows __rows) 0)
    ((:cols __cols) 0)
    )
   (send-super :init)
   (setq _data __data)
   (setq _rows (round __rows))
   (setq _cols (round __cols))
   self)
  (:data
   (&optional __data)
   (if __data (setq _data __data)) _data)
  (:rows
   (&optional __rows)
   (if __rows (setq _rows __rows)) _rows)
  (:cols
   (&optional __cols)
   (if __cols (setq _cols __cols)) _cols)
  (:serialization-length
   ()
   (+
    ;; float32[] _data
    (* 4    (length _data)) 4
    ;; uint8 _rows
    1
    ;; uint8 _cols
    1
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; float32[] _data
     (write-long (length _data) s)
     (dotimes (i (length _data))
       (sys::poke (elt _data i) (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
       )
     ;; uint8 _rows
       (write-byte _rows s)
     ;; uint8 _cols
       (write-byte _cols s)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; float32[] _data
   (let (n)
     (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4)
     (setq _data (instantiate float-vector n))
     (dotimes (i n)
     (setf (elt _data i) (sys::peek buf ptr- :float)) (incf ptr- 4)
     ))
   ;; uint8 _rows
     (setq _rows (sys::peek buf ptr- :char)) (incf ptr- 1)
   ;; uint8 _cols
     (setq _cols (sys::peek buf ptr- :char)) (incf ptr- 1)
   ;;
   self)
  )

(setf (get my_custom_msgs::TransformationMatrixMsg :md5sum-) "4d0b94d1560ce83cc042f419edb24b18")
(setf (get my_custom_msgs::TransformationMatrixMsg :datatype-) "my_custom_msgs/TransformationMatrixMsg")
(setf (get my_custom_msgs::TransformationMatrixMsg :definition-)
      "float32[] data
uint8 rows
uint8 cols

")



(provide :my_custom_msgs/TransformationMatrixMsg "4d0b94d1560ce83cc042f419edb24b18")


