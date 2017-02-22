
file(GLOB sources *.h *.cpp)

LINK_DIRECTORIES(${DEPENDS_LIB_DIR})
add_executable(GenerateSql ${sources})

set_property(TARGET GenerateSql PROPERTY FOLDER "Tools")
if(MSVC)
	set_property(TARGET GenerateSql PROPERTY LINK_FLAGS "/SAFESEH:NO" ) 
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

TARGET_LINK_LIBRARIES(GenerateSql   
common
EntityDef
ServerCommon
${BOOST_LIBRARY}
${TINYXML2_LIBRARY}
${PYTHON_LIBRARY} 
${OPENSSL_LIBRARY}
${LIBEVENT_LIBRARY}
)

