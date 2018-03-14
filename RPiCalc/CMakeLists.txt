project (CalcPi)
set(exe_name CalcPi)

set(${exe_name}_srcs
  ComputePi.hpp
  ComputePi.cpp
  main.cpp
  )
  
source_group("" FILES ${${exe_name}_srcs})

add_executable(${exe_name} ${${exe_name}_srcs})
install(TARGETS ${exe_name} DESTINATION bin)
