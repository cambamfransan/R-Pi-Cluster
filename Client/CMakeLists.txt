set(exe_name Client)

set(${exe_name}_deps ProtoFiles)

set(${exe_name}_srcs
  main.cpp
  )

source_group("" FILES ${${exe_name}_srcs})

add_executable(${exe_name} ${${exe_name}_srcs})
target_link_libraries(${exe_name} ${${exe_name}_deps})
install(TARGETS ${exe_name} DESTINATION bin)
