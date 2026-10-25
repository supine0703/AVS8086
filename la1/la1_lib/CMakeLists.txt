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

project(${LZL_PROJ_ID}_lib VERSION 0.4.0)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
)

set(SOURCES_FILES
    utils/map_and_set.h

    common/position.h
    common/infos.cpp
    common/infos.h
    common/identifier.cpp
    common/identifier.h
    common/json.cpp
    common/json.h

    token/types_name.cpp
    token/mnemonics.cpp
    token/symbols.cpp
    token/token.cpp
    token/token.h

    lexer/lexer.cpp
    lexer/lexer.h

    ast/expr_headers.h
    ast/header.h
    ast/json.cpp
    ast/node.cpp
    ast/node.h
    ast/nodes_map.cpp
    ast/stmt_headers.h
    ast/type_names.cpp
    ast/value.cpp
    ast/value.h
    ast/exprs/address.h
    ast/exprs/array.h
    ast/exprs/assign.h
    ast/exprs/comma.h
    ast/exprs/dup.h
    ast/exprs/illegal.h
    ast/exprs/infix.h
    ast/exprs/load_x.h
    ast/exprs/make_x.h
    ast/exprs/operator.h
    ast/exprs/register.h
    ast/exprs/value.h

    # ast/nodes/address.hpp
    # ast/nodes/colon.hpp
    # ast/nodes/comma.hpp
    # ast/nodes/expression_statement.hpp
    # ast/nodes/float.hpp
    # ast/nodes/hlt.hpp
    # ast/nodes/illegal.hpp
    # ast/nodes/infix.hpp
    # ast/nodes/integer.hpp
    # ast/nodes/mov.hpp
    # ast/nodes/multiple_statement.hpp
    # ast/nodes/prefix.hpp
    # ast/nodes/program.hpp
    # ast/nodes/register.hpp
    # ast/nodes/single.hpp
    # ast/nodes/string.hpp
    # ast/nodes/well.hpp
    ast/stmts/define.h
    ast/stmts/expression_statement.h
    ast/stmts/instruction.h
    ast/stmts/instruction_extend.h
    ast/stmts/jmp.h
    ast/stmts/jmp_address.h
    ast/stmts/lxx.h
    ast/stmts/mov.h
    ast/stmts/program.h
    ast/stmts/push_pop.h
    ast/stmts/shift.h
    ast/stmts/single.h
    ast/stmts/well.h
    ast/stmts/xchg.h

    parser/parser.cpp
    parser/parser.h
    parser/parse_fns.cpp
    parser/precedences.cpp
    parser/parse/expression.cpp
    parser/parse/identifiers.cpp
    parser/parse/program.cpp
    parser/parse/statement.cpp

    # parser/parse/expressions/address.cpp
    # parser/parse/expressions/colon.cpp
    # parser/parse/expressions/comma.cpp
    # parser/parse/expressions/float.cpp
    # parser/parse/expressions/group.cpp
    # parser/parse/expressions/illegal.cpp
    # parser/parse/expressions/infix.cpp
    # parser/parse/expressions/integer.cpp
    # parser/parse/expressions/not_end.cpp
    # parser/parse/expressions/prefix.cpp
    # parser/parse/expressions/register.cpp
    # parser/parse/expressions/reserved_word.cpp
    # parser/parse/expressions/string.cpp
    # parser/parse/expressions/well.cpp
    parser/parse/exprs/address.cpp
    parser/parse/exprs/colon.cpp
    parser/parse/exprs/comma_array.cpp
    parser/parse/exprs/dup.cpp
    parser/parse/exprs/group.cpp
    parser/parse/exprs/illegal.cpp
    parser/parse/exprs/operator.cpp
    parser/parse/exprs/value.cpp

    # parser/parse/statement/mov.cpp
    # parser/parse/statement/single.cpp
    parser/parse/stmts/allocate.cpp
    parser/parse/stmts/define.cpp
    parser/parse/stmts/jmp.cpp
    parser/parse/stmts/logical.cpp
    parser/parse/stmts/lxx.cpp
    parser/parse/stmts/mov.cpp
    parser/parse/stmts/push_pop.cpp
    parser/parse/stmts/shift.cpp
    parser/parse/stmts/single.cpp
    parser/parse/stmts/well.cpp
    parser/parse/stmts/xchg.cpp
)

# 生成动态库
add_library(${PROJECT_NAME} SHARED
    ${SOURCES_FILES}
)

# 链接库
target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
)

# 针对不同平台的设置
if(WIN32)
    # 生成DLL时需要导出所有符号
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
elseif(UNIX)
    # 为了在Linux下能够正确加载动态库，需要设置RPATH
    set(CMAKE_SKIP_BUILD_RPATH TRUE)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        BUILD_WITH_INSTALL_RPATH TRUE
        INSTALL_RPATH "$ORIGIN;$ORIGIN/lib;$ORIGIN/../lib"
    )
endif()

# set_target_properties(${PROJECT_NAME} PROPERTIES
# RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_PLUGIN_PATH}
# ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_PLUGIN_PATH}
# LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_PLUGIN_PATH}
# )
install(TARGETS ${PROJECT_NAME}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
