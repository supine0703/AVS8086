#[[ AVS8086 - See <https://github.com/supine0703/AVS8086> to know more. 

    Copyright (C) <2024>  <李宗霖>  github <https://github.com/supine0703>.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.]]

###############################################################################

# 项目的基本配置 
cmake_minimum_required(VERSION 3.5)

project(la1 VERSION 0.4.0 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

# 设置 Realse 的 exe 输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/../Release)


#### ...

# set(LA1_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})


set(RES_PATH     ${CMAKE_CURRENT_SOURCE_DIR}/res)
set(UI_PATH      ${CMAKE_CURRENT_SOURCE_DIR}/ui)
set(SRC_PATH     ${CMAKE_CURRENT_SOURCE_DIR}/src)

list(APPEND CMAKE_AUTOUIC_SEARCH_PATHS 
    ${UI_PATH}
)


# 设置需要编译的文件

set(RES_SOURCES
    ${RES_PATH}/app_win32.rc ${RES_PATH}/res_win32.qrc
)



set(GUI_PROJ_SOURCES
    ${SRC_PATH}/main_gui.cpp
    ${UI_PATH}/mainwidget.ui
    ${SRC_PATH}/widget/mainwidget.h
    ${SRC_PATH}/widget/mainwidget.cpp
    ${SRC_PATH}/settings/settings.h
    ${SRC_PATH}/settings/settings.cpp

    ${SRC_PATH}/widget/codeedit.h
    ${SRC_PATH}/widget/codeedit.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(a1
        MANUAL_FINALIZATION
        ${RES_SOURCES}
        ${GUI_PROJ_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET a1 APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(a1 SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(a1
            ${PROJECT_SOURCES}
        )
    endif()
endif()

# ... {
if(CMAKE_BUILD_TYPE STREQUAL Debug)
    set(FILES_PATH ${CMAKE_CURRENT_SOURCE_DIR}/share)
elseif(CMAKE_BUILD_TYPE STREQUAL Release)
    set(FILES_PATH .)
endif()

target_compile_definitions(a1 PRIVATE
    THE_VESION="${PROJECT_VERSION}"
    # HELP_PATH="${FILES_PATH}/help"
    # EXAMPLE_PATH="${FILES_PATH}/example"
    # TEMPLATE_PATH="${FILES_PATH}/template"
    # SHARE_PATH="${FILES_PATH}/share"
    CONFIG_PATH="${FILES_PATH}/config_ini/"
    CONFIG_INI="${FILES_PATH}/config.ini"
    CONFIG_UI_INI="${FILES_PATH}/config_ui.ini"
    DOCS_PATH="E:/Project/.Github/AVS8086/docs/"
)
# }

target_link_libraries(a1 PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.a1)
endif()
set_target_properties(a1 PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS a1
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(a1)
endif()
