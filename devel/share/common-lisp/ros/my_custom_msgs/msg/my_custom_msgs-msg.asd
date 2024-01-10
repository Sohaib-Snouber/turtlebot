
(cl:in-package :asdf)

(defsystem "my_custom_msgs-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils )
  :components ((:file "_package")
    (:file "TransformationMatrixMsg" :depends-on ("_package_TransformationMatrixMsg"))
    (:file "_package_TransformationMatrixMsg" :depends-on ("_package"))
  ))