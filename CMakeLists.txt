#CMake 最低版本号要求
cmake_minimum_required (VERSION 2.8)
set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -g")
project (webserver2.0)

aux_source_directory(. DIR_SRCS)


add_subdirectory(src)


add_executable(a.out web.cc)


target_link_libraries(a.out Source -pthread)
