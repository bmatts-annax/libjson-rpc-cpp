cmake_minimum_required(VERSION 2.8)
project(libjson-rpc-cpp)

# list command no longer ignores empty elements
cmake_policy(SET CMP0007 NEW)
cmake_policy(SET CMP0012 NEW)

if (${CMAKE_MAJOR_VERSION} GREATER 2)
	# old policy do not use MACOSX_RPATH
	cmake_policy(SET CMP0042 OLD)
endif()

#-----------------------------------------------
#---------------ANNAX---------------------------

message("${CMAKE_SOURCE_DIR}/LibMicroHttpd/src/microhttpd/.libs/libmicrohttpd.a")

set(MHD_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/LibMicroHttpd/src/include")
set(MHD_LIBRARY_STATIC "${CMAKE_SOURCE_DIR}/LibMicroHttpd/src/microhttpd/.libs/libmicrohttpd.a")

set(JSONCPP_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/LibJsonCpp/dist/json")
set(JSONCPP_SOURCE "${CMAKE_SOURCE_DIR}/LibJsonCpp/dist/jsoncpp.cpp")


#--------------------------------------------------

set(MAJOR_VERSION 0)
set(MINOR_VERSION 6)
set(PATCH_VERSION 0)
set(SO_VERSION    0)

set(BUILD_SHARED_LIBS YES CACHE BOOL "Build shared libraries")
set(BUILD_STATIC_LIBS NO CACHE BOOL "Build static libraries")
set(LIB_SUFFIX "" CACHE STRING "Suffix for library directory (32/64)")

if (NOT BUILD_SHARED_LIBS AND NOT BUILD_STATIC_LIBS)
	message(FATAL_ERROR "Both BUILD_SHARED_LIBS and BUILD_STATIC_LIBS are disabled")
endif()

# defaults for modules that can be enabled/disabled
if(UNIX)
        set(UNIX_DOMAIN_SOCKET_SERVER NO CACHE BOOL "Include Unix Domain Socket server")
        set(UNIX_DOMAIN_SOCKET_CLIENT NO CACHE BOOL "Include Unix Domain Socket client")
        set(TCP_SERVER YES CACHE BOOL "Include TCP server using sockets")
        set(TCP_CLIENT YES CACHE BOOL "Include TCP client support using sockets")
endif(UNIX)
set(HTTP_SERVER YES CACHE BOOL "Include HTTP server using libmicrohttpd")
set(HTTP_CLIENT YES CACHE BOOL "Include HTTP client support using curl")
set(COMPILE_TESTS NO CACHE BOOL "Compile test framework")
set(COMPILE_STUBGEN NO CACHE BOOL "Compile the stubgenerator")
set(COMPILE_EXAMPLES NO CACHE BOOL "Compile example programs")

# print actual settings
message(STATUS "HTTP_SERVER: ${HTTP_SERVER}")
message(STATUS "HTTP_CLIENT: ${HTTP_CLIENT}")
if(UNIX)
    message(STATUS "UNIXDOMAINSOCKET_SERVER: ${UNIX_DOMAIN_SOCKET_SERVER}")
    message(STATUS "UNIXDOMAINSOCKET_CLIENT: ${UNIX_DOMAIN_SOCKET_CLIENT}")
    message(STATUS "TCP_SERVER: ${TCP_SERVER}")
    message(STATUS "TCP_CLIENT: ${TCP_CLIENT}")
endif(UNIX)
message(STATUS "COMPILE_TESTS: ${COMPILE_TESTS}")
message(STATUS "COMPILE_STUBGEN: ${COMPILE_STUBGEN}")
message(STATUS "COMPILE_EXAMPLES: ${COMPILE_EXAMPLES}")

# setup directory where we should look for cmake files
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

# setup compiler settings && dependencies
include(CMakeCompilerSettings)
include(CMakeDependencies)

# setup output directories
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")

if (WIN32)
	set(CMAKE_INSTALL_PREFIX ${PROJECT_BINARY_DIR}/dist)
endif()

# setup global include_directories (for backwards compatibility)
include_directories("${CMAKE_BINARY_DIR}/gen/")
include_directories("${CMAKE_BINARY_DIR}/gen/jsonrpccpp/common")

# the core framework
add_subdirectory(src/jsonrpccpp)

#the stubgenerator
if (COMPILE_STUBGEN)
	add_subdirectory(src/stubgenerator)
endif ()

# setup examples
if (COMPILE_EXAMPLES)
	add_subdirectory(src/examples)
endif()

# setup test suite
if (COMPILE_TESTS)
        enable_testing(true)
	add_subdirectory(src/test)
endif()

# create documentation
if (DOXYGEN_FOUND)
	file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/doc)
	message(STATUS "Found doxygen: ${DOXYGEN_EXECUTABLE}")
	configure_file("${CMAKE_SOURCE_DIR}/doc/doxyfile.in" "${CMAKE_BINARY_DIR}/Doxyfile" @ONLY)
	add_custom_target(doc ${DOXYGEN_EXECUTABLE} ${CMAKE_BINARY_DIR}/Doxyfile WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/doc COMMENT "Generating API documentation")
endif(DOXYGEN_FOUND)

# setup uninstall target
configure_file(
	"${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in"
	"${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
	IMMEDIATE @ONLY
)

add_custom_target(
	uninstall
	COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake
)

# packaging stuff
include(CMakePackaging)
