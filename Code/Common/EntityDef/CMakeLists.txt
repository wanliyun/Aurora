
file(GLOB pre_src pch.*) 
file(GLOB sources *.h *.cpp)
file(GLOB DataTypes_Src DataTypes/*.h DataTypes/*.cpp) 

SOURCE_GROUP(EntityDef FILES ${sources})
SOURCE_GROUP(DataTypes FILES ${DataTypes_Src} )
SOURCE_GROUP(SourceFiles FILES ${pre_src})

set(EntityDef_LIB_SRCS ${sources} ${DataTypes_Src})

add_library(EntityDef STATIC ${EntityDef_LIB_SRCS})

set_property(TARGET EntityDef PROPERTY FOLDER "Common")

include_directories(
. 
${CODE_DIR}/Common 
${CODE_DIR}/Common/common
${THIRD_PARTY_DIR}
${LIBEVENT_INCLUDE_DIR}
${TINYXML2_INCLUDE_DIRS}
${OPENSSL_INCLUDE_DIRS}
${Boost_INCLUDE_DIRS}
${PYTHON_INCLUDE_DIR}
)

INCLUDE(UtilityMacros)
ADD_PRECOMPILED_HEADER(EntityDef pch.h)
