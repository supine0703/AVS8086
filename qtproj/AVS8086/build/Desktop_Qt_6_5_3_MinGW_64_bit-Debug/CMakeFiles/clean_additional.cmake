# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "AVS8086_autogen"
  "CMakeFiles\\AVS8086_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\AVS8086_autogen.dir\\ParseCache.txt"
  )
endif()
