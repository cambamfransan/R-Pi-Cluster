set(lib_name Logger)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 COMPONENTS Core Network REQUIRED)

set(${lib_name}_deps)

set(${lib_name}_srcs
	Logger.cpp
	Logger.hpp
  )

source_group("" FILES ${${lib_name}_srcs})

add_library(${lib_name} ${${lib_name}_srcs})
target_link_libraries(${lib_name} ${${lib_name}_deps} Qt5::Core Qt5::Network)
set_property(TARGET ${lib_name} PROPERTY FOLDER ${folder})
target_include_directories(${lib_name} PUBLIC ${CMAKE_SOURCE_DIR}/Shared/Logger)

add_subdirectory(test)
