
(cl:in-package :asdf)

(defsystem "dasl_mocap-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils )
  :components ((:file "_package")
    (:file "quadcopter2pc" :depends-on ("_package_quadcopter2pc"))
    (:file "_package_quadcopter2pc" :depends-on ("_package"))
    (:file "pc2quadcopter" :depends-on ("_package_pc2quadcopter"))
    (:file "_package_pc2quadcopter" :depends-on ("_package"))
    (:file "Vector3F" :depends-on ("_package_Vector3F"))
    (:file "_package_Vector3F" :depends-on ("_package"))
    (:file "RigidBody" :depends-on ("_package_RigidBody"))
    (:file "_package_RigidBody" :depends-on ("_package"))
  ))