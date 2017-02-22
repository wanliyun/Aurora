
file(GLOB pre_src pch.*) 
file(GLOB sources *.h *.cpp)

SOURCE_GROUP(Common FILES ${sources})
SOURCE_GROUP(SourceFiles FILES ${pre_src})

set(SERVER_COMMON_SRCS ${sources} ${Apps_Src} ${Entity_Src} ${Scripts_Src})

add_library(ServerCommon ${SERVER_COMMON_SRCS})

if(MSVC)
	#set_property(TARGET ServerCommon PROPERTY EXCLUDE_FROM_DEFAULT_BUILD TRUE ) 
endif()

set_property(TARGET ServerCommon PROPERTY FOLDER "Servers")

include_directories(
./
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
ADD_PRECOMPILED_HEADER(ServerCommon pch.h)
