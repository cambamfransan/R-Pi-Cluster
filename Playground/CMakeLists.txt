set(exe_name Playground)

set(${exe_name}_deps)

set(main_srcs
  main.cpp
  )

source_group("" FILES ${main_srcs})


add_executable(${exe_name} ${main_srcs})
target_link_libraries(${exe_name} ${${exe_name}_deps})
set_property(TARGET ${exe_name} PROPERTY FOLDER ${folder})

install(TARGETS ${exe_name} DESTINATION bin)
