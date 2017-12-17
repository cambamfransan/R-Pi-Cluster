set(exe_name ProtoFilesTest)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 COMPONENTS Core Test REQUIRED)

set(${lib_name}_deps ProtoFiles)

set(ProtobufTest_srcs
  ProtoFilesTestMain.cpp
  )
source_group("" FILES ${ProtobufTest_srcs})
list(APPEND ${exe_name}_proto_srcs ${ProtobufTest_srcs})


add_executable(${exe_name} ${${exe_name}_proto_srcs})
target_link_libraries( ${exe_name} ${${lib_name}_deps} )
add_test(NAME ${exe_name} COMMAND ${exe_name})
qt5_use_modules(${exe_name} Widgets Core Test)

set_property(TARGET ${exe_name} PROPERTY FOLDER ${folder}/UnitTests)
