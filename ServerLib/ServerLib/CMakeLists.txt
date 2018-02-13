set(lib_name ServerLib)

set(${lib_name}_deps TCPSender Gui)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

###########################################
set(server_srcs
  Server.cpp
  Server.hpp
  )
source_group("" FILES ${server_srcs})
list(APPEND ${lib_name}_srcs ${server_srcs})
###########################################
set(server_msgs_srcs
  Msgs/ReceiveHeartBeat.cpp
  Msgs/ReceiveHeartBeat.hpp
  )
source_group("Msgs" FILES ${server_msgs_srcs})
list(APPEND ${lib_name}_srcs ${server_msgs_srcs})
###########################################

include_directories(${RAPIDJSON_INCLUDE_DIRS})
add_library(${lib_name} ${${lib_name}_srcs})
target_link_libraries(${lib_name} ${${lib_name}_deps} Qt5::Core Qt5::Network)
set_property(TARGET ${lib_name} PROPERTY FOLDER ${folder})
qt5_use_modules(${lib_name} Widgets Core Network Test)

target_include_directories(${lib_name} PUBLIC ${CMAKE_SOURCE_DIR}/ServerLib)
#add_subdirectory(test)