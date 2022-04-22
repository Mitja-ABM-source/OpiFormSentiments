CMAKE_MINIMUM_REQUIRED(VERSION 3.1)

PROJECT(OpiformSentiments)

SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)

SET(CMAKE_DEBUG_POSTFIX "D")

# Warn in the case of in-source build
if("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_CURRENT_BINARY_DIR}")
  message(WARNING "The source directory is the same as binary directory.")
endif()

#set install directory
set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/../install CACHE STRING
	  "Redirect install dir to default" FORCE)
set(PROJECT_INSTALL_INCLUDE_DIR ${CMAKE_INSTALL_PREFIX}/include)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL "UNIX")
	add_cxx_compiler_flag(-std=c++11)
	if (NOT HAVE_CXX_FLAG_STD_CXX11)
		add_cxx_compiler_flag(-std=c++0x)
	endif()

	if (CYGWIN)
		add_definitions(-D_GNU_SOURCE=1)
	endif()
endif()

include(cmake/OpiformSentiments.cmake)

add_subdirectory(OpiformSentiments)
add_subdirectory(Demo)