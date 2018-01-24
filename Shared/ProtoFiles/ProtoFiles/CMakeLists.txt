set(lib_name ProtoFiles)

set(${lib_name}_deps ${Protobuf_LIBRARIES})

set(${lib_name}_srcs
	Backup.proto
	BasicMsg.proto
	TaskMsg.proto
  TestMsg.proto
	MsgToSend.proto
  )

source_group(Messages FILES ${${lib_name}_srcs})
list(APPEND ${lib_name}_proto_srcs ${Protobuf_srcs})

PROTOBUF_GENERATE_CPP(${lib_name}_cpp ${lib_name}_hpp ${${lib_name}_srcs})
source_group(${lib_name} FILES ${${lib_name}_cpp} ${${lib_name}_hpp})
if(MSVC)
	set_source_files_properties(${${lib_name}_cpp} ${${lib_name}_hpp} PROPERTIES COMPILE_FLAGS "/wd4267 /wd4244 /wd4800")
elseif(CMAKE_COMPILER_IS_GNUCXX AND (CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 6.0))
	set(noMisIndent "-Wno-misleading-indentation")
#	check_cxx_compiler_flag("${noMisIndent}" has_noMisIndent)
	if(has_noMisIndent)
		set_source_files_properties(${${lib_name}_cpp} ${${lib_name}_hpp} PROPERTIES COMPILE_FLAGS "${noMisIndent}")
	endif()
endif()

add_library(${lib_name} STATIC ${${lib_name}_cpp} ${${lib_name}_hpp} ${${lib_name}_srcs})
target_link_libraries(${lib_name} ${${lib_name}_deps})
target_include_directories(${lib_name} PUBLIC ${CMAKE_BINARY_DIR}/Shared/ProtoFiles ${Protobuf_INCLUDE_DIRS})

set_property(TARGET ${lib_name} PROPERTY FOLDER ${folder})
add_subdirectory(test)
