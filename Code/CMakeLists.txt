

add_subdirectory(Common)

if(NOT SERVER_ONLY)
add_subdirectory(Clients)
endif()

add_subdirectory(Servers)
add_subdirectory(Tools)