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

# ##############################################################################

project(${LZL_PROJ_ID}_gui VERSION 0.4.0)

set(RES_PATH ${CMAKE_CURRENT_SOURCE_DIR}/res)
set(SRC_PATH ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(UI_PATH ${CMAKE_CURRENT_SOURCE_DIR}/ui)
list(APPEND CMAKE_AUTOUIC_SEARCH_PATHS ${UI_PATH})

include_directories(${SRC_PATH}/settings ${SRC_PATH}/widget)

# 设置需要编译的文件
set(RES_SOURCES ${RES_PATH}/app_win32.rc ${RES_PATH}/res_win32.qrc)

set(GUI_PROJ_SOURCES
    ${SRC_PATH}/main_gui.cpp
    ${SRC_PATH}/settings/settings.h ${SRC_PATH}/settings/settings.cpp
    ${SRC_PATH}/widget/mainwidget.h ${SRC_PATH}/widget/mainwidget.cpp
    ${SRC_PATH}/widget/codeedit.h ${SRC_PATH}/widget/codeedit.cpp
    ${SRC_PATH}/widget/searchedit.h ${SRC_PATH}/widget/searchedit.cpp
    ${UI_PATH}/mainwidget.ui
)

qt_add_executable(${PROJECT_NAME}
    MANUAL_FINALIZATION
    ${RES_SOURCES}
    ${GUI_PROJ_SOURCES}
)

# ...
if(CMAKE_BUILD_TYPE STREQUAL Debug)
    set(FILES_PATH ${CMAKE_SOURCE_DIR}/share)
elseif(CMAKE_BUILD_TYPE STREQUAL Release)
    set(FILES_PATH .)
endif()

target_compile_definitions(${PROJECT_NAME} PRIVATE
    THE_VERSION="${PROJECT_VERSION}"
    CONFIG_INI="${FILES_PATH}/config.ini"
    DOCS_PATH="${CMAKE_SOURCE_DIR}/../docs"
)

# ...
target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
    set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.${PROJECT_NAME})
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
