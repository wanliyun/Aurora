
file(GLOB pre_src pch.*) 
file(GLOB sources *.h *.cpp)
file(GLOB Apps_Src 		Apps/*.h Apps/*.cpp) 
file(GLOB Int_Src 		*Interface.h *Interface.cpp)

file(GLOB Entity_Src 		Entity/*.h Entity/*.cpp)
file(GLOB Scripts_Src 		Scripts/*.h Scripts/*.cpp)
file(GLOB Space_Src 		Space/*.h Space/*.cpp)

SOURCE_GROUP(AppManager FILES ${sources})
SOURCE_GROUP(Apps FILES ${Apps_Src} )
SOURCE_GROUP(SourceFiles FILES ${pre_src})
SOURCE_GROUP(NetworkInterface FILES ${Int_Src})

SOURCE_GROUP(Entity FILES ${Entity_Src})
SOURCE_GROUP(Space FILES ${Space_Src})
SOURCE_GROUP(Scripts FILES ${Scripts_Src})

set(CELL_APP_SRCS ${sources} ${Apps_Src} ${pre_src} ${Entity_Src} ${Scripts_Src} ${Space_Src})

LINK_DIRECTORIES(${DEPENDS_LIB_DIR})
add_executable(CellApp ${CELL_APP_SRCS})

set_property(TARGET CellApp PROPERTY FOLDER "Servers")
if(MSVC)
	#set_property(TARGET CellApp PROPERTY EXCLUDE_FROM_DEFAULT_BUILD TRUE ) 
	set_property(TARGET CellApp PROPERTY LINK_FLAGS "/SAFESEH:NO" ) 
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
ADD_PRECOMPILED_HEADER(CellApp pch.h)

TARGET_LINK_LIBRARIES(CellApp   
EntityDef
common
ServerCommon
${TINYXML2_LIBRARY}
${BOOST_LIBRARY}
${PYTHON_LIBRARY} 
${OPENSSL_LIBRARY}
${LIBEVENT_LIBRARY}
)

