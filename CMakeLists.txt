cmake_minimum_required(VERSION 2.8)

option(CPPAN_TEST_YAS "Test yas library support" OFF)

set(cppan_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}" CACHE INTERNAL "cppan include dirs")

find_package(Boost 1.52.0 COMPONENTS serialization unit_test_framework REQUIRED)

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

if (CPPAN_TEST_YAS)
	add_definitions(-DCPPAN_TEST_YAS)
endif (CPPAN_TEST_YAS)

include_directories(${Boost_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(sources
  cppan/all.hpp
  cppan/declare_and_annotate.hpp
  cppan/define_member_detector.hpp
  cppan/has_annotations.hpp
  cppan/member.hpp
  cppan/io.hpp
  cppan/detail/metafunctions.hpp
  cppan/detail/fusion_extension.hpp
  cppan/support/boost_hash.hpp
  cppan/support/boost_serialization.hpp
  cppan/support/yas.hpp
)
 
foreach(src ${sources})
	string(REGEX REPLACE "(.*)/.*" "\\1" dir ${src})
	string(REPLACE "/" "\\" msvc_folder ${dir})
	source_group(${msvc_folder} FILES ${src})
endforeach(src ${sources})

add_executable(cppan.unit_test ${sources} tests/test.cpp)
target_link_libraries(cppan.unit_test ${Boost_LIBRARIES})
