FILE(REMOVE_RECURSE
  "../msg_gen"
  "../srv_gen"
  "../src/dasl_mocap/msg"
  "../src/dasl_mocap/srv"
  "../msg_gen"
  "../srv_gen"
  "CMakeFiles/ROSBUILD_gensrv_py"
  "../src/dasl_mocap/srv/__init__.py"
  "../src/dasl_mocap/srv/_Spawn.py"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/ROSBUILD_gensrv_py.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
