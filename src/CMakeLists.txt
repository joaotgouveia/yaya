set(
    HEADER_LIST
    "../include/ast/basic_node.hpp"
    "../include/ast/group_node.hpp"
    "../include/ast/expression_node.hpp"
    "../include/ast/lvalue_node.hpp"
    "../include/ast/call_node.hpp"
    "../include/ast/eq_node.hpp"
    "../include/ast/neq_node.hpp"
    "../include/ast/or_node.hpp"
    "../include/ast/and_node.hpp"
    "../include/ast/geq_node.hpp"
    "../include/ast/leq_node.hpp"
    "../include/ast/lt_node.hpp"
    "../include/ast/gt_node.hpp"
    "../include/ast/add_node.hpp"
    "../include/ast/sub_node.hpp"
    "../include/ast/mul_node.hpp"
    "../include/ast/mod_node.hpp"
    "../include/ast/div_node.hpp"
    "../include/ast/int_literal_node.hpp"
    "../include/ast/function_node.hpp"
    "../include/ast/assignment_node.hpp"
    "../include/ast/declaration_node.hpp"
    "../include/ast/return_node.hpp"
    "../include/compiler/compiler.hpp"
    "../include/frontend/lexer.hpp"
    "../include/visitors/abstract_ast_visitor.hpp"
    "../include/visitors/print_visitor.hpp"
)


bison_target(Parser frontend/parser.y ${CMAKE_CURRENT_BINARY_DIR}/parser.cpp
             DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/parser.tab.hh)
flex_target(Lexer frontend/lexer.l ${CMAKE_CURRENT_BINARY_DIR}/lexer.cpp)
add_flex_bison_dependency(Lexer Parser)

add_executable(yaya
    main.cpp
    compiler/compiler.cpp
    visitors/print_visitor.cpp
    ${HEADER_LIST}
    ${BISON_Parser_OUTPUTS}
    ${FLEX_Lexer_OUTPUTS}
)
target_include_directories(yaya
    PUBLIC
    ../include
    PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}
    ${FLEX_INCLUDE_DIRS}
)
