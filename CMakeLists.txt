include(cmake/GitUtils.cmake)
cmake_minimum_required(VERSION 3.24)

set(CMAKE_PREFIX_PATH "C:/Qt/6.6.0/mingw_64")
set(WINDEPLOYQT_EXE "${CMAKE_PREFIX_PATH}/bin/windeployqt6.exe")

project(wincap11 VERSION 0.1)
SetGitHash()
configure_file(src/util/Version.h.in src/util/Version.h)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/bin")

find_package(Qt6 REQUIRED COMPONENTS
        Core
        Gui
        Widgets
        )

add_executable(wincap11 src/main.cpp src/ui/mainwindow.cpp src/ui/mainwindow.h src/ui/mainwindow.ui src/util/ScreenshotUtil.cpp src/util/ScreenshotUtil.h src/ui/qpictureview.cpp src/ui/qpictureview.h src/util/Version.h.in src/ui/stylesheet.h)

target_include_directories(wincap11 PUBLIC "${PROJECT_BINARY_DIR}/src")

qt_import_plugins(wincap11
        INCLUDE imageformats
        )

target_link_libraries(wincap11
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        )

if (WIN32)
    set(DEBUG_SUFFIX)
    set(QT_INSTALL_PATH "${CMAKE_PREFIX_PATH}")

    # Release build
    if (CMAKE_BUILD_TYPE STREQUAL "Release")
        # Prevent opening a terminal window
        set_property(TARGET ${PROJECT_NAME} PROPERTY WIN32_EXECUTABLE true)
    endif ()

    # run windeployqt to copy all required .dll files
    execute_process(COMMAND ${WINDEPLOYQT_EXE} "${EXECUTABLE_OUTPUT_PATH}")

    if (MSVC AND CMAKE_BUILD_TYPE MATCHES "Debug")
        set(DEBUG_SUFFIX "d")
    endif ()

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
                "${QT_INSTALL_PATH}/bin/Qt6${QT_LIB}${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
    endforeach (QT_LIB)
endif ()
