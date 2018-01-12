set(lib_name ServerLib)

set(${lib_name}_deps TCPSender)

###########################################
set(server_srcs
  Server.cpp
  Server.hpp
  )
source_group("" FILES ${server_srcs})
list(APPEND ${lib_name}_srcs ${server_srcs})
###########################################

add_library(${lib_name} ${${lib_name}_srcs})
target_link_libraries(${lib_name} ${${lib_name}_deps})
set_property(TARGET ${lib_name} PROPERTY FOLDER ${folder})
qt5_use_modules(${lib_name} Widgets Core Network Test)

target_include_directories(${lib_name} PUBLIC ${CMAKE_SOURCE_DIR}/ServerLib)
#add_subdirectory(test)