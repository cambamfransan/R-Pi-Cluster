set(exe_name UDPSenderTest)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5Widgets REQUIRED)

set(${exe_name}_deps UDPSender)

set(srcs
	UDPSenderTest.cpp
)

source_group("" FILES ${srcs})
list(APPEND ${exe_name}_srcs ${srcs})

add_executable(${exe_name} ${${exe_name}_srcs})
target_link_libraries( ${exe_name} ${${exe_name}_deps})
add_test(NAME ${exe_name} COMMAND ${exe_name})
qt5_use_modules(${exe_name} Widgets Core Test Network)

set_property(TARGET ${exe_name} PROPERTY FOLDER ${folder}/UnitTests)