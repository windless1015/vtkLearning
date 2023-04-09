cmake_minimum_required(VERSION 3.5)

project(displayIPRItem VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

#set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)


find_package(VTK REQUIRED)

#include(${VTK_USE_FILE})
if("${VTK_QT_VERSION}" STREQUAL "")
  message(FATAL_ERROR "VTK was not built with Qt")
endif()

# 设置头文件路径
include_directories(D:\\Librarys\\vtk9.1.0\\VTK\\include\\vtk-9.1)
# 添加动态库路径
link_directories(D:\\Librarys\\vtk9.1.0\\VTK\\lib)

find_package(QT NAMES Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
        main.cpp
        ipritem.cpp
        ipritem.h
        graphitem.ui
        vtkipritem.cpp
        vtkipritem.h
)




if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(GraphItem
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET TeethCrownGeneration APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(GraphItem SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(displayIPRItem
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(displayIPRItem PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
# 链接vtk lib
target_link_libraries(displayIPRItem PRIVATE ${VTK_LIBRARIES})

set_target_properties(displayIPRItem PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS displayIPRItem
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

