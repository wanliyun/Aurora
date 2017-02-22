

file(GLOB pre_src pch.*) 
file(GLOB sources *.h *.cpp)
file(GLOB Apps_Src 		Apps/*.h Apps/*.cpp) 
file(GLOB Int_Src 		*Interface.h *Interface.cpp)

SOURCE_GROUP(DBManager FILES ${sources})
SOURCE_GROUP(Apps FILES ${Apps_Src} )
SOURCE_GROUP(SourceFiles FILES ${pre_src})
SOURCE_GROUP(NetworkInterface FILES ${Int_Src})

set(DB_MANAGER_SRCS ${sources} ${Apps_Src} ${pre_src} )

LINK_DIRECTORIES(${DEPENDS_LIB_DIR})
add_executable(DBManager ${DB_MANAGER_SRCS})

set_property(TARGET DBManager PROPERTY FOLDER "Servers")
if(MSVC)
	#set_property(TARGET DBManager PROPERTY EXCLUDE_FROM_DEFAULT_BUILD TRUE ) 
	set_property(TARGET DBManager PROPERTY LINK_FLAGS "/SAFESEH:NO" ) 
endif()
include_directories(
./
${CODE_DIR}/Servers/ServerCommon 
${CODE_DIR}/Common 
${CODE_DIR}/Common/common
${THIRD_PARTY_DIR} 
${TINYXML2_INCLUDE_DIRS}
${OPENSSL_INCLUDE_DIRS}
${CODE_DIR}/EntityDef
${PYTHON_INCLUDE_DIR}
${Boost_INCLUDE_DIRS}
${LIBEVENT_INCLUDE_DIR}
)

INCLUDE(UtilityMacros)
ADD_PRECOMPILED_HEADER(DBManager pch.h)


TARGET_LINK_LIBRARIES(DBManager
common
EntityDef
ServerCommon
${TINYXML2_LIBRARY}
${BOOST_LIBRARY}
${PYTHON_LIBRARY} 
${OPENSSL_LIBRARY}
${LIBEVENT_LIBRARY}
)

