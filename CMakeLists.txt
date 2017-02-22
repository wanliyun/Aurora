cmake_minimum_required(VERSION 2.8.10)

cmake_policy(SET CMP0015 NEW)
if(CMP0021)
	cmake_policy(SET CMP0021 OLD)
endif()

set(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS TRUE)
SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)

project(Aurora C CXX)

set(CODE_DIR 				"${Aurora_SOURCE_DIR}/Code")
set(DEPENDS_ROOT_DIR 		"${CODE_DIR}/Depends")
set(DEPENDS_INCLUDE_DIR0 	"${DEPENDS_ROOT_DIR}/include")


SET(CMAKE_MODULE_PATH "${Aurora_SOURCE_DIR}/CMakeModules;${CMAKE_MODULE_PATH}")

#-----------------------------------------------------------------------------
# Redirect output files
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin CACHE INTERNAL "" FORCE)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib CACHE INTERNAL "" FORCE)

# C/C++ flags
set(PLATFORM_CFLAGS)

# these are added to later on.
set(C_WARNINGS)
set(CXX_WARNINGS)

# for gcc -Wno-blah-blah
set(CC_REMOVE_STRICT_FLAGS)

# Added to linker flags in setup_liblinks
# - CMAKE_EXE_LINKER_FLAGS
# - CMAKE_EXE_LINKER_FLAGS_DEBUG
set(PLATFORM_LINKFLAGS "")
set(PLATFORM_LINKFLAGS_DEBUG "")

# Configure target directories
set(BIN_DIR       "${CMAKE_INSTALL_PREFIX}/bin")

if(WIN32)
    set(CONF_DIR  ".")
    set(LIBS_DIR  "${CMAKE_INSTALL_PREFIX}/bin")
else()
    set(LIBS_DIR  "${CMAKE_INSTALL_PREFIX}/lib")
endif()

# For Unix systems set the rpath so that libraries are found
set(CMAKE_INSTALL_RPATH             ${LIBS_DIR})
set(CMAKE_INSTALL_NAME_DIR          ${LIBS_DIR})



#-----------------------------------------------------------------------------
# Platform specifics

if(UNIX AND NOT APPLE)
	set(DEPENDS_ROOT_DIR "${DEPENDS_ROOT_DIR}/linux")
	
    # OpenSuse needs lutil, ArchLinux not, for now keep, can avoid by using --as-needed
    set(PLATFORM_LINKLIBS "-lutil -lc -lm -lpthread -lstdc++")

    set(PLATFORM_LINKFLAGS "${PLATFORM_LINKFLAGS} -pthread")

    # lfs on glibc, all compilers should use
    add_definitions(-D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64 -D_LARGEFILE64_SOURCE)

    # GNU Compiler
    if(CMAKE_COMPILER_IS_GNUCC)
        set(PLATFORM_CFLAGS "-pipe -fPIC -funsigned-char -fno-strict-aliasing")
    # CLang is the same as GCC for now.
    elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        set(PLATFORM_CFLAGS "-pipe -fPIC -funsigned-char -fno-strict-aliasing")
    # Solaris CC
    elseif(CMAKE_CXX_COMPILER_ID MATCHES "SunPro")
        set(PLATFORM_CFLAGS "-pipe -features=extensions -fPIC -D__FUNCTION__=__func__")

    # Intel C++ Compiler
    elseif(CMAKE_C_COMPILER_ID MATCHES "Intel")
        # think these next two are broken
        find_program(XIAR xiar)
        if(XIAR)
            set(CMAKE_AR "${XIAR}")
        endif()
        mark_as_advanced(XIAR)

        find_program(XILD xild)
        if(XILD)
            set(CMAKE_LINKER "${XILD}")
        endif()
        mark_as_advanced(XILD)

        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fp-model precise -prec_div -parallel")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fp-model precise -prec_div -parallel")

        # set(PLATFORM_CFLAGS "${PLATFORM_CFLAGS} -diag-enable sc3")
        set(PLATFORM_CFLAGS "-pipe -fPIC -funsigned-char -fno-strict-aliasing")
        set(PLATFORM_LINKFLAGS "${PLATFORM_LINKFLAGS} -static-intel")
    endif()

elseif(WIN32)
	set(DEPENDS_ROOT_DIR "${DEPENDS_ROOT_DIR}/win")
	
    # this file is included anyway when building under Windows with cl.exe
    #  include(${CMAKE_ROOT}/Modules/Platform/Windows-cl.cmake)

    if(CMAKE_COMPILER_IS_GNUCC)
        include(CheckCSourceCompiles)
        # Setup 64bit and 64bit windows systems
        CHECK_C_SOURCE_COMPILES("
            #ifndef __MINGW64__
            #error
            #endif
            int main(void) { return 0; }
            "
            WITH_MINGW64)

        if(WITH_MINGW64)
            message(STATUS "Compiling for 64 bit with MinGW-w64.")
        else()
            message(STATUS "Compiling for 32 bit with MinGW-w32.")
        endif()
    else()
        # Setup 64bit and 64bit windows systems
        if(CMAKE_CL_64)
            message(STATUS "64 bit compiler detected.")
            if(MSVC11)
                message(STATUS "Visual C++ 2012 detected.")
            else()
                message(STATUS "Visual C++ detected.")
            endif()
        else()
        # Setup 32bit windows systems
            if(MSVC11)
                message(STATUS "Visual C++ 2012 detected.")
            else()
                message(STATUS "Visual C++ detected.")
            endif()
        endif()
    endif()

    add_definitions(-DWIN32)

    if(MSVC)
        set(PLATFORM_LINKLIBS ws2_32)

        add_definitions(/D_CRT_NONSTDC_NO_DEPRECATE /D_CRT_SECURE_NO_DEPRECATE /D_CRT_SECURE_NO_WARNINGS /D_SCL_SECURE_NO_DEPRECATE)

        # MSVC11 needs _ALLOW_KEYWORD_MACROS to build
        if(MSVC11)
            add_definitions(/D_ALLOW_KEYWORD_MACROS)
        endif()

        set(CMAKE_CXX_FLAGS "/nologo /J /Gd /EHsc" CACHE STRING "MSVC MT C++ flags " FORCE)
        set(CMAKE_C_FLAGS   "/nologo /J /Gd"       CACHE STRING "MSVC MT C++ flags " FORCE)

        if(CMAKE_CL_64)
            set(CMAKE_CXX_FLAGS_DEBUG "/Od /MP /RTC1 /MDd /Zi" CACHE STRING "MSVC MT flags " FORCE)
        else()
            set(CMAKE_CXX_FLAGS_DEBUG "/Od /MP /RTC1 /MDd /ZI" CACHE STRING "MSVC MT flags " FORCE)
        endif()

        set(CMAKE_CXX_FLAGS_RELEASE "/O2 /Ob2 /MD" CACHE STRING "MSVC MT flags " FORCE)

        if(CMAKE_CL_64)
            set(CMAKE_C_FLAGS_DEBUG "/Od /MP /RTC1 /MDd /Zi" CACHE STRING "MSVC MT flags " FORCE)
        else()
            set(CMAKE_C_FLAGS_DEBUG "/Od /MP /RTC1 /MDd /ZI" CACHE STRING "MSVC MT flags " FORCE)
        endif()
        set(CMAKE_C_FLAGS_RELEASE "/O2 /Ob2 /MD" CACHE STRING "MSVC MT flags " FORCE)

        # most msvc warnings are C & C++
        set(_WARNINGS "/W3")
        set(C_WARNINGS "${_WARNINGS}")
        set(CXX_WARNINGS "${_WARNINGS}")
        unset(_WARNINGS)

        set(PLATFORM_LINKFLAGS "/SUBSYSTEM:CONSOLE /STACK:2097152 /INCREMENTAL:NO /NODEFAULTLIB:msvcrt.lib /NODEFAULTLIB:msvcmrt.lib /NODEFAULTLIB:msvcurt.lib /NODEFAULTLIB:msvcrtd.lib")

        # MSVC only, Mingw doesnt need
        if(CMAKE_CL_64)
            set(PLATFORM_LINKFLAGS "/MACHINE:X64 /OPT:NOREF ${PLATFORM_LINKFLAGS}")
        else()
            set(PLATFORM_LINKFLAGS "/MACHINE:IX86 /LARGEADDRESSAWARE ${PLATFORM_LINKFLAGS}")
        endif()

        set(PLATFORM_LINKFLAGS_DEBUG "/NODEFAULTLIB:libcmt.lib /NODEFAULTLIB:libc.lib")

    elseif(CMAKE_COMPILER_IS_GNUCC)
    # keep GCC specific stuff here
        set(PLATFORM_LINKLIBS "-lshell32 -lshfolder -lgdi32 -lmsvcrt -lwinmm -lmingw32 -lm -lws2_32 -lz -lstdc++ -lole32 -luuid -lwsock32 -lpsapi")
        set(PLATFORM_CFLAGS "-pipe -funsigned-char -fno-strict-aliasing")

        if(WITH_MINGW64)
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fpermissive")
            set(PLATFORM_LINKLIBS "${PLATFORM_LINKLIBS} -lpthread")

            add_definitions(-DFREE_WINDOWS64 -DMS_WIN64)
            # Turn off OpenMP since it causes crashes on render for subsurfed/multiresolution meshes
            set(WITH_OPENMP OFF)
        endif()

        add_definitions(-D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64 -D_LARGEFILE64_SOURCE)

        add_definitions(-DFREE_WINDOWS)

        set(PLATFORM_LINKFLAGS "-Xlinker --stack=2097152")

        ## DISABLE - causes linking errors
        ## for re-distribution, so users dont need mingw installed
        # set(PLATFORM_LINKFLAGS "${PLATFORM_LINKFLAGS} -static-libgcc -static-libstdc++")

    endif()
endif()

#-----------------------------------------------------------------------------
# Git and build information extraction
set(GIT_REFSPEC     "unknown branch")
set(GIT_REVISION    "unknown revision")
set(BUILD_DATE      "*")
set(BUILD_TIME      "*")


# BUILD_PLATFORM and BUILD_PLATFORM are taken from CMake but BUILD_DATE and
# BUILD_TIME are platform dependant.
if(UNIX)
    execute_process(COMMAND date "+%Y-%m-%d" OUTPUT_VARIABLE BUILD_DATE OUTPUT_STRIP_TRAILING_WHITESPACE)
    execute_process(COMMAND date "+%H:%M:%S" OUTPUT_VARIABLE BUILD_TIME OUTPUT_STRIP_TRAILING_WHITESPACE)
endif()
if(WIN32)
    execute_process(COMMAND cmd /c date /t OUTPUT_VARIABLE BUILD_DATE OUTPUT_STRIP_TRAILING_WHITESPACE)
    execute_process(COMMAND cmd /c time /t OUTPUT_VARIABLE BUILD_TIME OUTPUT_STRIP_TRAILING_WHITESPACE)
endif()


#-----------------------------------------------------------------------------
# Build dependencies
set(DEPENDS_INCLUDE_DIR1 "${DEPENDS_ROOT_DIR}/include")
if(CMAKE_SIZEOF_VOID_P EQUAL 4)
	set(DEPENDS_ROOT_DIR "${DEPENDS_ROOT_DIR}/x86")
else()
	set(DEPENDS_ROOT_DIR "${DEPENDS_ROOT_DIR}/x64")
endif()

set(DEPENDS_ROOT_DIR "${DEPENDS_ROOT_DIR}/${CMAKE_BUILD_TYPE}" "${DEPENDS_ROOT_DIR}" )

set(DEPENDS_LIB_DIR "${DEPENDS_ROOT_DIR}/")
set(DEPENDS_BIN_DIR "${DEPENDS_ROOT_DIR}/")

macro(SET_DEPENDS_INCLUDE_DIRS varName relativePath)
	set(${varName}  "${DEPENDS_INCLUDE_DIR1}/${relativePath}"  "${DEPENDS_INCLUDE_DIR0}/${relativePath}")
endmacro(SET_DEPENDS_INCLUDE_DIRS)

find_package(PythonLibs REQUIRED)

add_definitions(-DBOOST_PYTHON_STATIC_LIB)

SET_DEPENDS_INCLUDE_DIRS(Boost_INCLUDE_DIRS boost_1_56_0)
SET_DEPENDS_INCLUDE_DIRS(OPENSSL_INCLUDE_DIRS openssl-1.0.0n/inc32)
SET_DEPENDS_INCLUDE_DIRS(LIBEVENT_INCLUDE_DIR libevent-2.0.21-stable/include )
SET_DEPENDS_INCLUDE_DIRS(TINYXML2_INCLUDE_DIRS tinyxml2-master)
SET_DEPENDS_INCLUDE_DIRS(LIBPQXX_INCLUDE_DIR libpqxx-4.0.1/include)
SET_DEPENDS_INCLUDE_DIRS(SQLITE3_INCLUDE_DIR sqlite3.8.6)

if(WIN32)
set(LIBEVENT_LIBRARY	libevent.lib)
set(OPENSSL_LIBRARY		libeay32.lib)
set(TINYXML2_LIBRARY	tinyxml2.lib)
set(LIBPQXX_LIBRARY		libpqxx.lib)
set(SQLITE3_LIBRARY		sqlite.lib)
else()
set(LIBEVENT_LIBRARY	libevent.a dl libboost_python.a libboost_python3.a)
set(OPENSSL_LIBRARY		libcrypto.a)
set(TINYXML2_LIBRARY	tinyxml2.a)
set(LIBPQXX_LIBRARY		pqxx pq pthread)
set(SQLITE3_LIBRARY		libsqlite.a)
endif()




#-----------------------------------------------------------------------------
# Build our own applications
add_subdirectory(Code)
