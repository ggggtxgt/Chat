cmake_minimum_required(VERSION 4.0)
project(Client)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_PREFIX_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../../application/Qt/5.14.2/mingw73_64")

find_package(Qt5 COMPONENTS
        Core
        Gui
        Widgets
        Network
        REQUIRED)

# 应用图标
set(ICO "${CMAKE_CURRENT_SOURCE_DIR}/../../../study/Chat/Client/res/qrc/ico.rc")
# 资源文件
set(QRC "${CMAKE_CURRENT_SOURCE_DIR}/../../../study/Chat/Client/res/qrc/rc.qrc")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/source/headers)         # 包含头文件
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/source/cpp CPP_LIST)   # 搜索源文件
file(GLOB FORM_LIST "${CMAKE_CURRENT_SOURCE_DIR}/source/forms/*.ui")    # 搜索 ui 文件
file(GLOB HEADER_LIST "${CMAKE_CURRENT_SOURCE_DIR}/source/headers/*.h") # 搜索头文件

add_executable(${PROJECT_NAME}
        ${ICO}
        ${QRC}
        ${CPP_LIST}
        ${FORM_LIST}
        ${HEADER_LIST}
)

target_link_libraries(${PROJECT_NAME}
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        Qt5::Network
)

if(WIN32)
    # 在配置阶段就复制资源
    if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/res/static)
        file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/res/static/
                DESTINATION ${CMAKE_BINARY_DIR}/static
                FILES_MATCHING PATTERN "*")
        message(STATUS "复制 static 目录到构建目录")
    else()
        message(WARNING "目录不存在: ${CMAKE_CURRENT_SOURCE_DIR}/res/static")
        # 创建空目录避免构建失败
        file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/static)
    endif()

    # 复制 config.ini 到输出目标目录
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_CURRENT_SOURCE_DIR}/config.ini"
            $<TARGET_FILE_DIR:${PROJECT_NAME}>
    )
endif()

if (WIN32 AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(DEBUG_SUFFIX)
    if (MSVC AND CMAKE_BUILD_TYPE MATCHES "Debug")
        set(DEBUG_SUFFIX "d")
    endif ()
    set(QT_INSTALL_PATH "${CMAKE_PREFIX_PATH}")
    if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
        set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
            set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        endif ()
    endif ()
    if (EXISTS "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
    endif ()
    foreach (QT_LIB Core Gui Widgets)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/bin/Qt5${QT_LIB}${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
    endforeach (QT_LIB)
endif ()
