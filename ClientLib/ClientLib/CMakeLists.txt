set(lib_name ClientLib)

set(${lib_name}_deps TCPSender Gui)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

###########################################
set(client_srcs
  Client.cpp
  Client.hpp
  )
source_group("" FILES ${client_srcs})
list(APPEND ${lib_name}_srcs ${client_srcs})
###########################################

add_library(${lib_name} ${${lib_name}_srcs})
target_link_libraries(${lib_name} ${${lib_name}_deps} Qt5::Core Qt5::Network)
set_property(TARGET ${lib_name} PROPERTY FOLDER ${folder})
qt5_use_modules(${lib_name} Widgets Core Network Test)

target_include_directories(${lib_name} PUBLIC ${CMAKE_SOURCE_DIR}/ClientLib)
#add_subdirectory(test)
