cmake_minimum_required(VERSION 3.5)

project(AVS8086 VERSION 0.1.5 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/../Release)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)


function(recurse_directories var root_dir)
    if (IS_DIRECTORY ${root_dir})
        set(${var} ${${var}} ${root_dir})
        file(GLOB ALL_SUB RELATIVE ${root_dir} ${root_dir}/*)
        foreach(sub ${ALL_SUB})
            recurse_directories(${var} ${root_dir}/${sub})
        endforeach()
        set(${var} ${${var}} PARENT_SCOPE)
    endif()
endfunction()

set(INCLUDE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/widget)
set(RES_PATH     ${CMAKE_CURRENT_SOURCE_DIR}/res)
set(SRC_PATH     ${CMAKE_CURRENT_SOURCE_DIR}/widget)
set(UI_PATH      ${CMAKE_CURRENT_SOURCE_DIR}/ui)

set(BACK_END_MODEL ${CMAKE_CURRENT_SOURCE_DIR}/back_end)
set(WIDGET_UI_MODEL ${CMAKE_CURRENT_SOURCE_DIR}/widget_ui)

recurse_directories(INCLUDE_ALL_DIR ${INCLUDE_PATH})
recurse_directories(UI_ALL_DIR      ${UI_PATH})

recurse_directories(WIDGET_UI_ALL_DIR ${WIDGET_UI_MODEL})

list(APPEND CMAKE_AUTOUIC_SEARCH_PATHS
    ${UI_ALL_DIR}
    ${WIDGET_UI_ALL_DIR}
)

include_directories(
    ${INCLUDE_ALL_DIR}
    ${BACK_END_MODEL}
    ${WIDGET_UI_ALL_DIR}
    settings
)

file(GLOB_RECURSE PROJECT_SOURCES
    settings/*.h settings/*.cpp

    ${BACK_END_MODEL}/*.h ${BACK_END_MODEL}/*.hpp
    ${BACK_END_MODEL}/*.cpp ${BACK_END_MODEL}/*.ui

    ${WIDGET_UI_MODEL}/*.h
    ${WIDGET_UI_MODEL}/*.cpp
    ${WIDGET_UI_MODEL}/*.ui

    ${INCLUDE_PATH}/*.hpp ${INCLUDE_PATH}/*.h
    ${SRC_PATH}/*.cpp     ${SRC_PATH}/*.c

    ${RES_PATH}/*.qrc     ${RES_PATH}/*.rc
    ${UI_PATH}/*.ui
)


if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(${PROJECT_NAME}
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        main.cpp
        widget_ui/Interaction/interactionwidget.h widget_ui/Interaction/interactionwidget.cpp widget_ui/Interaction/interactionwidget.ui
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET power_carrier APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(${PROJECT_NAME} SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(${PROJECT_NAME}
            ${PROJECT_SOURCES}
        )
    endif()
endif()


if(CMAKE_BUILD_TYPE STREQUAL Debug)
    set(FILES_PATH ${CMAKE_CURRENT_SOURCE_DIR}/share)
elseif(CMAKE_BUILD_TYPE STREQUAL Release)
    set(FILES_PATH .)
endif()

target_compile_definitions(${PROJECT_NAME} PRIVATE
    THE_VESION="${PROJECT_VERSION}"
    # HELP_PATH="${FILES_PATH}/help"
    # EXAMPLE_PATH="${FILES_PATH}/example"
    # TEMPLATE_PATH="${FILES_PATH}/template"
    # SHARE_PATH="${FILES_PATH}/share"
    CONFIG_INI="${FILES_PATH}/config.ini"
    CONFIG_UI_INI="${FILES_PATH}/config_ui.ini"
    DOCS_PATH="E:/Project/.Github/AVS8086/docs/"
)
target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
)


# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
    set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.power_carrier)
endif()
set_target_properties(${PROJECT_NAME} PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(${PROJECT_NAME})
endif()
