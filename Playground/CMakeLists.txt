set(exe_name Playground)

set(${exe_name}_deps)


set(main_srcs
  main.cpp
  )

source_group("" FILES ${main_srcs})

include_directories(${RAPIDJSON_INCLUDE_DIRS})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${RAPIDJSON_CXX_FLAGS}")
add_executable(${exe_name} ${main_srcs})
target_link_libraries(${exe_name} ${${exe_name}_deps})
set_property(TARGET ${exe_name} PROPERTY FOLDER ${folder})

install(TARGETS ${exe_name} DESTINATION bin)
