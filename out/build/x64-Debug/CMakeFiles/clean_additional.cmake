# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\KuGouApp_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\KuGouApp_autogen.dir\\ParseCache.txt"
  "KuGouApp_autogen"
  )
endif()
