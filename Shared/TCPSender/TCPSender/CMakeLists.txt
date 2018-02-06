set(lib_name TCPSender)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 COMPONENTS Core Network REQUIRED)

set(${lib_name}_deps ProtoFiles Logger Messages)

set(${lib_name}_srcs
	TCPSenderClient.cpp
	TCPSenderClient.hpp
	TCPSenderServer.cpp
	TCPSenderServer.hpp
  TCPSenderWeb.cpp
	TCPSenderWeb.hpp
  )

source_group("" FILES ${${lib_name}_srcs})

include_directories(${RAPIDJSON_INCLUDE_DIRS})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${RAPIDJSON_CXX_FLAGS}")
add_library(${lib_name} ${${lib_name}_srcs})
target_link_libraries(${lib_name} PUBLIC ${${lib_name}_deps} Qt5::Core Qt5::Network)
set_property(TARGET ${lib_name} PROPERTY FOLDER ${folder})
target_include_directories(${lib_name} PUBLIC ${CMAKE_SOURCE_DIR}/Shared/TCPSender ${RAPIDJSON_INCLUDE_DIRS})

#add_subdirectory(Test)
