set(exe_name Server)

set(${exe_name}_deps TCPSender)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(${exe_name}_srcs
  main.cpp
  )

source_group("" FILES ${${exe_name}_srcs})

add_executable(${exe_name} ${${exe_name}_srcs})
target_link_libraries(${exe_name} ${${exe_name}_deps})
install(TARGETS ${exe_name} DESTINATION bin)
qt5_use_modules(${exe_name} Widgets Core Network Test)
