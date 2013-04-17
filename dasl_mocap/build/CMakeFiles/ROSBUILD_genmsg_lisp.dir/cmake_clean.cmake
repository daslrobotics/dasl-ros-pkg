FILE(REMOVE_RECURSE
  "../msg_gen"
  "../srv_gen"
  "../src/dasl_mocap/msg"
  "../src/dasl_mocap/srv"
  "../msg_gen"
  "../srv_gen"
  "CMakeFiles/ROSBUILD_genmsg_lisp"
  "../msg_gen/lisp/quadcopter2pc.lisp"
  "../msg_gen/lisp/_package.lisp"
  "../msg_gen/lisp/_package_quadcopter2pc.lisp"
  "../msg_gen/lisp/Velocity.lisp"
  "../msg_gen/lisp/_package.lisp"
  "../msg_gen/lisp/_package_Velocity.lisp"
  "../msg_gen/lisp/pc2quadcopter.lisp"
  "../msg_gen/lisp/_package.lisp"
  "../msg_gen/lisp/_package_pc2quadcopter.lisp"
  "../msg_gen/lisp/RigidBody.lisp"
  "../msg_gen/lisp/_package.lisp"
  "../msg_gen/lisp/_package_RigidBody.lisp"
  "../msg_gen/lisp/Vector3F.lisp"
  "../msg_gen/lisp/_package.lisp"
  "../msg_gen/lisp/_package_Vector3F.lisp"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/ROSBUILD_genmsg_lisp.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
