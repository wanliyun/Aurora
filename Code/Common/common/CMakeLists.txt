
file(GLOB pre_src pch.* targetver.h) 
file(GLOB sources *.h *.cpp)
file(GLOB entity_src 		entity/*.h entity/*.cpp)
file(GLOB interface_src interface/*.h interface/*.cpp)
file(GLOB network_src 	network/*.h network/*.cpp)
file(GLOB pyscript_src 	pyscript/*.h pyscript/*.cpp)

SOURCE_GROUP(Utils FILES ${sources})
SOURCE_GROUP(entity FILES ${entity_src} )
SOURCE_GROUP(interface FILES ${interface_src} )
SOURCE_GROUP(network FILES ${network_src} )
SOURCE_GROUP(pyscript FILES ${pyscript_src} )

SOURCE_GROUP(SourceFiles FILES ${pre_src})

set(Common_LIB_SRCS ${sources} ${entity_src} ${interface_src} ${network_src} ${pyscript_src} )

add_library(common STATIC ${Common_LIB_SRCS})

include_directories(
. 
${THIRD_PARTY_DIR}
${LIBEVENT_INCLUDE_DIR}
${TINYXML2_INCLUDE_DIRS}
${OPENSSL_INCLUDE_DIRS}
${PYTHON_INCLUDE_DIR}
${Boost_INCLUDE_DIRS}
${SQLITE3_INCLUDE_DIR}
)
set_property(TARGET common PROPERTY FOLDER "Common")

INCLUDE(UtilityMacros)
ADD_PRECOMPILED_HEADER(common pch.h)

TARGET_LINK_LIBRARIES(common
${SQLITE3_LIBRARY}
)

