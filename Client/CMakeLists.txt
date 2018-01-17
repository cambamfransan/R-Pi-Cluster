set(exe_name Client)

set(${exe_name}_deps ClientLib TCPSender)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 COMPONENTS Core Network REQUIRED)

set(${exe_name}_srcs
  main.cpp
  )

source_group("" FILES ${${exe_name}_srcs})

add_executable(${exe_name} ${${exe_name}_srcs})
target_link_libraries(${exe_name} ${${exe_name}_deps} Qt5::Core Qt5::Network)
install(TARGETS ${exe_name} DESTINATION bin)
qt5_use_modules(${exe_name} Network Core)
