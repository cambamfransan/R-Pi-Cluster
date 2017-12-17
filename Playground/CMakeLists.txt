set(exe_name Playground)

FIND_PACKAGE(Boost COMPONENTS filesystem REQUIRED)
set(${exe_name}_deps ${Boost_LIBRARIES})

set(main_srcs
  main.cpp
  )

source_group("" FILES ${main_srcs})


add_executable(${exe_name} ${main_srcs})
target_link_libraries(${exe_name} ${${exe_name}_deps})
set_property(TARGET ${exe_name} PROPERTY FOLDER ${folder})
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

install(TARGETS ${exe_name} DESTINATION bin)
