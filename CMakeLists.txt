cmake_minimum_required(VERSION 3.5.1)
set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR} CACHE PATH
  "Locaction for installation"
  FORCE)
set(CMAKE_BUILD_TYPE "Release" CACHE STRING
  "Choose the type of build, options are: Debug Release"
  FORCE)
SET(TESTING_GUIS 0 CACHE INTEGER "If Testing Guis should be used")
add_definitions( -DTESTING_GUIS=${TESTING_GUIS} )
project(RPi)
include (version.cmake)
enable_testing()

set(CMAKE_CXX_STANDARD 14)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if(CMAKE_COMPILER_IS_GNUCXX)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -pedantic -Wextra")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -pedantic -Wextra -O3")
else()
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MT") #TODO
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
endif(CMAKE_COMPILER_IS_GNUCXX)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
FIND_PACKAGE( RapidJson REQUIRED)
FIND_PACKAGE( Protobuf REQUIRED)
find_package(Qt5 COMPONENTS Widgets Core Test REQUIRED)

add_subdirectory(Shared)
add_subdirectory(ServerLib/ServerLib)
add_subdirectory(Server)
add_subdirectory(ClientLib/ClientLib)
add_subdirectory(Client)
add_subdirectory(Playground)
