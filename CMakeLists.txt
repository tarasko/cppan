cmake_minimum_required(VERSION 2.8)

set(cppan_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}" CACHE INTERNAL "cppan include dirs")

find_package(Boost 1.52.0 REQUIRED)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
	message("cppan - STANDALONE mode")
	set(CPPAN_STANDALONE_BUILD 1)
else()
	message("cppan - INCLUSIVE mode")
	set(CPPAN_STANDALONE_BUILD 0)
endif()

if (WIN32)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS -D_SCL_SECURE_NO_WARNINGS -DBOOST_ALL_NO_LIB)
endif (WIN32)

include_directories(${Boost_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(sources
  cppan/cppan.hpp
  cppan/detail/declare_and_annotate.hpp
  cppan/support/std_hash.hpp
  cppan/support/boost_hash.hpp
  cppan/support/boost_serialization.hpp
)

foreach(src ${sources})
	string(REGEX REPLACE "(.*)/.*" "\\1" dir ${src})
	string(REPLACE "/" "\\" msvc_folder ${dir})
	source_group(${msvc_folder} FILES ${src})
endforeach(src ${sources})

add_executable(cppan.unit_test ${sources} tests/test.cpp)
