cmake_minimum_required(VERSION 3.9)
project(yuihex)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_PREFIX_PATH ~/Qt/5.11.1/clang_64/)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOMOC ON)
# Create code from a list of Qt designer ui files
set(CMAKE_AUTOUIC ON)
set(AUTOUIC ON)

find_package(Qt5 COMPONENTS Core Widgets REQUIRED)

add_executable(yuihex
        main.cpp
        easylogging++.cc
        easylogging++.h
        QHexEditor.cpp
        QHexEditor.h
        MainWindow.cpp
        MainWindow.h QHexCursor.cpp QHexCursor.h QAsciiCursor.cpp QAsciiCursor.h)

find_library(MAGIC_LIB magic)
target_link_libraries(yuihex ${MAGIC_LIB})

include_directories(${Qt5Widgets_INCLUDE_DIRS})
target_link_libraries(yuihex Qt5::Widgets)