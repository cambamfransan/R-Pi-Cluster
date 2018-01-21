project(Shared)
set(FILE_DESC "Shared ${PROJECT_NAME}")
set(FILE_DESC)
set(folder ${PROJECT_NAME})
set_property(GLOBAL PROPERTY FOLDER Shared)
##################################

add_subdirectory(Logger/Logger)
add_subdirectory(ProtoFiles/ProtoFiles)
add_subdirectory(UDPSender/UDPSender)
add_subdirectory(TCPSender/TCPSender)
add_subdirectory(Gui/Gui)
