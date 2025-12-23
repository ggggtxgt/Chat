cmake_minimum_required(VERSION 4.0)
project(GateServer)

set(CMAKE_CXX_STANDARD 14)

# boost 头文件
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../../application/boost)

# jsoncpp
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../../application/jsoncpp/libjson/lib)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../../application/jsoncpp/libjson/include)

add_executable(${PROJECT_NAME}
        GateServer.cpp
)

# jsoncpp 库文件
target_link_libraries(${PROJECT_NAME} PRIVATE json_vc71_libmtd.lib)