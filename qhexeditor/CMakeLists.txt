project(qhexeditor)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_PREFIX_PATH ~/Qt/5.11.1/clang_64/)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOMOC ON)
# Create code from a list of Qt designer ui files
set(CMAKE_AUTOUIC ON)
set(AUTOUIC ON)

find_package(Qt5 COMPONENTS Core Widgets Designer REQUIRED)

add_library(qhexeditor SHARED
        qhexeditor.cpp
        qhexeditorplugin.cpp
        stream.h
        file_stream.cpp
        file_stream.h
        easylogging++.h
        easylogging++.cc selection.cpp selection.h types.h cursor.cpp cursor.h cursor.cpp cursor.h)

if(MSVC)
    target_compile_options(qhexeditor PRIVATE /W4 /WX)
else(MSVC)
    target_compile_options(qhexeditor PRIVATE -Wall -Wextra)
endif(MSVC)

include_directories(${Qt5Widgets_INCLUDE_DIRS})
target_link_libraries(qhexeditor Qt5::Widgets Qt5::Designer)
