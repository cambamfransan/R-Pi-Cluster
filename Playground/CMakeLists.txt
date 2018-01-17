set(exe_name Playground)

set(${exe_name}_deps TCPSender)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(main_srcs
  main.cpp
  )

source_group("" FILES ${main_srcs})

include_directories(${RAPIDJSON_INCLUDE_DIRS})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${RAPIDJSON_CXX_FLAGS}")
add_executable(${exe_name} ${main_srcs})

qt5_use_modules(${exe_name} Widgets Core Network Test)
target_link_libraries(${exe_name} ${${exe_name}_deps})
set_property(TARGET ${exe_name} PROPERTY FOLDER ${folder})

install(TARGETS ${exe_name} DESTINATION bin)
