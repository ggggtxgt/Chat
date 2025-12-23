cmake_minimum_required(VERSION 4.0)
project(GateServer)

set(CMAKE_CXX_STANDARD 14)

# boost 头文件
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../../application/boost)

add_executable(${PROJECT_NAME}
        GateServer.cpp
)