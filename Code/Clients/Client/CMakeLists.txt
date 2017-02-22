
file(GLOB pre_src pch.*) 
file(GLOB sources *.h *.cpp)
file(GLOB Apps_Src 		Apps/*.h Apps/*.cpp) 
file(GLOB Entity_Src 	Entity/*.h Entity/*.cpp) 
file(GLOB Scripts_Src Scripts/*.h Scripts/*.cpp) 

SOURCE_GROUP(ClientApp FILES ${sources})

SOURCE_GROUP(Apps FILES ${Apps_Src} )
SOURCE_GROUP(Entity FILES ${Entity_Src} )
SOURCE_GROUP(Scripts FILES ${Scripts_Src} )

SOURCE_GROUP(SourceFiles FILES ${pre_src})
SOURCE_GROUP(NetworkInterface FILES ClientInterface.h ClientInterface.cpp)


set(CLIENT_APP_SRCS ${sources} ${Apps_Src} ${Entity_Src} ${Scripts_Src})

LINK_DIRECTORIES(${DEPENDS_LIB_DIR})
add_executable(Client ${CLIENT_APP_SRCS})

set_property(TARGET Client PROPERTY FOLDER "Clients")

if(MSVC)
	set_property(TARGET Client PROPERTY LINK_FLAGS "/SAFESEH:NO" ) 
endif()

include_directories(
./
${CODE_DIR}/Common 
${CODE_DIR}/Common/common
${THIRD_PARTY_DIR}
${LIBEVENT_INCLUDE_DIR}
${TINYXML2_INCLUDE_DIRS}
${OPENSSL_INCLUDE_DIRS}
${PYTHON_INCLUDE_DIR}
${Boost_INCLUDE_DIRS}
)

INCLUDE(UtilityMacros)
ADD_PRECOMPILED_HEADER(Client pch.h)


TARGET_LINK_LIBRARIES(Client
EntityDef
common
${TINYXML2_LIBRARY}
${BOOST_LIBRARY}
${PYTHON_LIBRARY} 
${OPENSSL_LIBRARY}
${LIBEVENT_LIBRARY}
)



