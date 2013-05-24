FILE(REMOVE_RECURSE
  "msg_gen"
  "srv_gen"
  "src/dasl_mocap/msg"
  "src/dasl_mocap/srv"
  "CMakeFiles/test-results"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/test-results.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
