cmake_minimum_required(VERSION 2.8)
cmake_policy(VERSION 2.8)

project(Eraser)

if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")
endif(CMAKE_COMPILER_IS_GNUCXX)

if(ACTION)
	add_definitions(-DACTION)
	message(STATUS "Allow Action or Phi Function")
else()
	message(STATUS "Not Allow Action or Phi Function")
endif()

## Whether print Results WITH color
option(ALLOW_COLOR_OUTPUT "Print Results With Color" OFF)
if(ALLOW_COLOR_OUTPUT)
	add_definitions(-DALLOW_COLOR_OUTPUT)
	message(STATUS "Print Results With Color")
else()
	message(STATUS "Not Print Results With Color")
endif()


include_directories (./include)

file(GLOB SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

add_library(${PROJECT_NAME} ${SRC})  #执行编译时包含哪些源文件

add_subdirectory(test) #添加一个子目录并构建该子目录

