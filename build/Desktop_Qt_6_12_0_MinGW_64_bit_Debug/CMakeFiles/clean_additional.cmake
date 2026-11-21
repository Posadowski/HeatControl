# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\HeatControl_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\HeatControl_autogen.dir\\ParseCache.txt"
  "HeatControl_autogen"
  )
endif()
