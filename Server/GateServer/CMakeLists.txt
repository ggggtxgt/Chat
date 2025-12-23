cmake_minimum_required(VERSION 4.0)
project(GateServer)

set(CMAKE_CXX_STANDARD 14)

# 项目头文件、源文件
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
file(GLOB SRC_LIST "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

# boost 头文件
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../../application/boost)
# glog
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../../application/glog/lib)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../../application/glog/include)

# jsoncpp
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../../application/jsoncpp/libjson/lib)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../../application/jsoncpp/libjson/include)

# 生成目标文件
add_executable(${PROJECT_NAME} ${SRC_LIST})

# jsoncpp 库文件
target_link_libraries(${PROJECT_NAME} PRIVATE json_vc71_libmtd.lib)
# glog 库文件
target_link_libraries(${PROJECT_NAME} PRIVATE glogd.dll DbgHelp)