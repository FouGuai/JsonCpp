cmake_minimum_required(VERSION 3.10.0)
project(jsoncpp VERSION 0.1.0 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 20)


add_executable(jsoncpp main.cpp src/Value.cpp src/Parser.cpp src/Reader.cpp src/Config.cpp src/ValueIter.cpp src/ValueAlgorithm.cpp)
# set(CMAKE_BUILD_TYPE Release) 

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2")
endif()

include(CTest)
enable_testing()
include_directories(${PROJECT_SOURCE_DIR}/include)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
