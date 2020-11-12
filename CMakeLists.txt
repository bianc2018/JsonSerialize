# CMakeList.txt: 顶层 CMake 项目文件，在此处执行全局配置
# 并包含子项目。
#
cmake_minimum_required (VERSION 3.8)

project ("JsonSerialize")

# 开启单元测试，make test
enable_testing() 
#关闭cjson 的单元测试
set(ENABLE_CJSON_TEST OFF)

if(WIN32)
      message("WIN32 编译设置")
      ADD_DEFINITIONS(-D_CRT_SECURE_NO_WARNINGS=1)
      #set(DEFAULT_LIB Crypt32)
      #set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
      #set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
elseif(UNIX)
     message("UNIX 编译设置")
     #通用的底层库 default lib
     set(DEFAULT_LIB pthread )
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Bin)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/3rdpart/cJSON)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/3rdpart/googletest/googletest/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/3rdpart/googletest/googlemock/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

SET( CMAKE_VERBOSE_MAKEFILE on ) 

# 包含子项目。
add_subdirectory ("3rdpart/cJSON")
add_subdirectory ("3rdpart/googletest")
add_subdirectory ("example")
add_subdirectory ("test")
