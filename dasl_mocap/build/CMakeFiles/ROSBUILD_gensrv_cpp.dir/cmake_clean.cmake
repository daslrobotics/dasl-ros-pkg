FILE(REMOVE_RECURSE
  "../msg_gen"
  "../srv_gen"
  "../src/dasl_mocap/msg"
  "../src/dasl_mocap/srv"
  "../msg_gen"
  "../srv_gen"
  "CMakeFiles/ROSBUILD_gensrv_cpp"
  "../srv_gen/cpp/include/dasl_mocap/Spawn.h"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/ROSBUILD_gensrv_cpp.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
