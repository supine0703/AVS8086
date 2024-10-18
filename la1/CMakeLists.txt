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

# 项目的基本配置
cmake_minimum_required(VERSION 3.5)

set(LZL_PROJ_ID la1)
project(${LZL_PROJ_ID} VERSION 0.4.0 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

# 设置 Realse 的 exe 输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/../Release)

# ### ...

# if(WIN32)
# set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON) # 生成DLL时需要导出所有符号

# # 设置编译选项，为了在Windows下能够正确显示中文
# if(MSVC)
# string(APPEND CMAKE_CXX_FLAGS " /source-charset:utf-8 /execution-charset:GBK")
# elseif(CMAKE_COMPILER_IS_GNUCXX)
# string(APPEND CMAKE_CXX_FLAGS " -fexec-charset=GBK")
# endif()

# elseif(UNIX)
# # 为了在Linux下能够正确加载动态库，需要设置RPATH
# set(CMAKE_SKIP_BUILD_RPATH TRUE)
# string(APPEND CMAKE_CXX_FLAGS "-Wl,-z,origin,-rpath,'$ORIGIN'")
# endif()
add_subdirectory(${LZL_PROJ_ID}_gui)
