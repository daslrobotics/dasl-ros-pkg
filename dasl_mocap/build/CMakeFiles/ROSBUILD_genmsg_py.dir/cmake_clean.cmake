FILE(REMOVE_RECURSE
  "../msg_gen"
  "../src/dasl_mocap/msg"
  "../msg_gen"
  "CMakeFiles/ROSBUILD_genmsg_py"
  "../src/dasl_mocap/msg/__init__.py"
  "../src/dasl_mocap/msg/_quadcopter2pc.py"
  "../src/dasl_mocap/msg/_pc2quadcopter.py"
  "../src/dasl_mocap/msg/_Vector3F.py"
  "../src/dasl_mocap/msg/_RigidBody.py"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/ROSBUILD_genmsg_py.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
