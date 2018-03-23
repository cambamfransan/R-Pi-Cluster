set(lib_name Messages)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 COMPONENTS Core Network REQUIRED)

set(${lib_name}_deps ProtoFiles Logger TCPSender)

###########################################
set(bottom_srcs
  Conversation.hpp
  JSONHelpers.cpp
  JSONHelpers.hpp
  JSONParser.cpp
  JSONParser.hpp
  MakeMsgs.cpp
  MakeMsgs.hpp
  MakeJSONMsgs.cpp
  MakeJSONMsgs.hpp
  MapHelpers.hpp
  )
source_group("" FILES ${bottom_srcs})
list(APPEND ${lib_name}_srcs ${bottom_srcs})
###########################################
set(manager_srcs
  Manager/Job.cpp
  Manager/Job.hpp
  Manager/JobManager.cpp
  Manager/JobManager.hpp
  Manager/ServerManager.cpp
  Manager/ServerManager.hpp
  Manager/ClientManager.cpp
  Manager/ClientManager.hpp
  Manager/Results.cpp
  Manager/Results.hpp
  Manager/Tasks.cpp
  Manager/Tasks.hpp
  )
source_group("Manager" FILES ${manager_srcs})
list(APPEND ${lib_name}_srcs ${manager_srcs})
###########################################
set(pi_srcs
  Pi/Pi.cpp
  Pi/Pi.hpp
  Pi/PiServerManager.cpp
  Pi/PiServerManager.hpp
  Pi/PiClientManager.cpp
  Pi/PiClientManager.hpp
  )
source_group("Pi" FILES ${pi_srcs})
list(APPEND ${lib_name}_srcs ${pi_srcs})
###########################################

include_directories(${RAPIDJSON_INCLUDE_DIRS})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${RAPIDJSON_CXX_FLAGS}")
add_library(${lib_name} ${${lib_name}_srcs})
target_link_libraries(${lib_name} ${${lib_name}_deps} Qt5::Core Qt5::Network)
set_property(TARGET ${lib_name} PROPERTY FOLDER ${folder})
target_include_directories(${lib_name} PUBLIC ${CMAKE_SOURCE_DIR}/Shared/Messages)

add_subdirectory(Test)
