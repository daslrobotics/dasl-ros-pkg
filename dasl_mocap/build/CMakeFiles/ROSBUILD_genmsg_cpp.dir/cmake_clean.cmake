FILE(REMOVE_RECURSE
  "../msg_gen"
  "../src/dasl_mocap/msg"
  "../msg_gen"
  "CMakeFiles/ROSBUILD_genmsg_cpp"
  "../msg_gen/cpp/include/dasl_mocap/quadcopter2pc.h"
  "../msg_gen/cpp/include/dasl_mocap/pc2quadcopter.h"
  "../msg_gen/cpp/include/dasl_mocap/Vector3F.h"
  "../msg_gen/cpp/include/dasl_mocap/RigidBody.h"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/ROSBUILD_genmsg_cpp.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
