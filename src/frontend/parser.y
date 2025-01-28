%require "3.2"
%language "c++"

%defines
%define api.namespace {yaya}
%define api.parser.class {Parser}

%code requires {
    #include "ast/add_node.hpp"
    #include "ast/and_node.hpp"
    #include "ast/assignment_node.hpp"
    #include "ast/call_node.hpp"
    #include "ast/declaration_node.hpp"
    #include "ast/div_node.hpp"
    #include "ast/eq_node.hpp"
    #include "ast/function_node.hpp"
    #include "ast/geq_node.hpp"
    #include "ast/group_node.hpp"
    #include "ast/gt_node.hpp"
    #include "ast/int_literal_node.hpp"
    #include "ast/leq_node.hpp"
    #include "ast/lt_node.hpp"
    #include "ast/lvalue_node.hpp"
    #include "ast/mod_node.hpp"
    #include "ast/mul_node.hpp"
    #include "ast/neq_node.hpp"
    #include "ast/or_node.hpp"
    #include "ast/return_node.hpp"
    #include "ast/sub_node.hpp"
    #include <string>
    #include <memory>

    namespace yaya {
        class Compiler;
        class Lexer;
    }
}

%parse-param { Lexer& lexer } { Compiler& compiler }

%code {
    #include "compiler/compiler.hpp"
    #include "frontend/lexer.hpp"

    #undef yylex
    #define yylex lexer.yylex
}

%define api.value.type variant
%define parse.error detailed
%define parse.assert
%define parse.trace

%token <int> tINT
%token <std::string> tIDENTIFIER
%token tFUNCTION tLET tRETURN
%token tGEQ tLEQ tEQ tNEQ tAND tOR

%type<std::unique_ptr<yaya::basic_node>> program decl inst function
%type<std::unique_ptr<yaya::group_node>> insts exprs functions args
%type<std::unique_ptr<yaya::expression_node>> expr simple_expr call binop
%type<std::unique_ptr<yaya::lvalue_node>> lvalue

%%

program : functions   { compiler.ast(std::move($1)); }
        | /* empty */ { compiler.ast(nullptr); }
        ;

inst : expr ';'            { $$ = std::move($1); }
     | decl ';'            { $$ = std::move($1); }
     | lvalue '=' expr ';' { $$ = std::make_unique<assignment_node>(std::move($1), std::move($3)); }
     | tRETURN expr ';'    { $$ = std::make_unique<return_node>(std::move($2)); }
     ;

insts : inst       { $$ = std::make_unique<group_node>(std::move($1)); }
      | insts inst { $$ = std::make_unique<group_node>(std::move($2), std::move($1)); }
      ;

decl :  tLET tIDENTIFIER          { $$ = std::make_unique<declaration_node>($2); }
     |  tLET tIDENTIFIER '=' expr { $$ = std::make_unique<declaration_node>($2, std::move($4)); }
     ;

expr : simple_expr { $$ = std::move($1); }
     | binop       { $$ = std::move($1); }
     ;

simple_expr : tINT   { $$ = std::make_unique<int_literal_node>($1); }
            | lvalue { $$ = std::move($1); }
            | call   { $$ = std::move($1); }
            ;

binop : simple_expr '*' simple_expr  { $$ = std::make_unique<mul_node>(std::move($1), std::move($3)); }
      | simple_expr '/' simple_expr  { $$ = std::make_unique<div_node>(std::move($1), std::move($3)); }
      | simple_expr '%' simple_expr  { $$ = std::make_unique<mod_node>(std::move($1), std::move($3)); }
      | simple_expr '+' simple_expr  { $$ = std::make_unique<add_node>(std::move($1), std::move($3)); }
      | simple_expr '-' simple_expr  { $$ = std::make_unique<sub_node>(std::move($1), std::move($3)); }
      | simple_expr '<' simple_expr  { $$ = std::make_unique<lt_node>(std::move($1), std::move($3)); }
      | simple_expr '>' simple_expr  { $$ = std::make_unique<gt_node>(std::move($1), std::move($3)); }
      | simple_expr tGEQ simple_expr { $$ = std::make_unique<geq_node>(std::move($1), std::move($3)); }
      | simple_expr tLEQ simple_expr { $$ = std::make_unique<leq_node>(std::move($1), std::move($3)); }
      | simple_expr tEQ simple_expr  { $$ = std::make_unique<eq_node>(std::move($1), std::move($3)); }
      | simple_expr tNEQ simple_expr { $$ = std::make_unique<neq_node>(std::move($1), std::move($3)); }
      | simple_expr tAND simple_expr { $$ = std::make_unique<and_node>(std::move($1), std::move($3)); }
      | simple_expr tOR simple_expr  { $$ = std::make_unique<or_node>(std::move($1), std::move($3)); }
      ;

function : tFUNCTION tIDENTIFIER '(' ')' '{' insts '}'      { $$ = std::make_unique<function_node>($2, nullptr, std::move($6)); }
         | tFUNCTION tIDENTIFIER '(' ')' '{' '}'            { $$ = std::make_unique<function_node>($2, nullptr, nullptr); }
         | tFUNCTION tIDENTIFIER '(' args ')' '{' insts '}' { $$ = std::make_unique<function_node>($2, std::move($4), std::move($7)); }
         | tFUNCTION tIDENTIFIER '(' args ')' '{' '}'       { $$ = std::make_unique<function_node>($2, std::move($4), nullptr); }
         ;

functions : function           { $$ = std::make_unique<group_node>(std::move($1)); }
          | functions function { $$ = std::make_unique<group_node>(std::move($2), std::move($1)); }
          ;

args : lvalue          { $$ = std::make_unique<group_node>(std::move($1)); }
     | args ',' lvalue { $$ = std::make_unique<group_node>(std::move($3), std::move($1)); }
     ;

call : tIDENTIFIER '(' ')'       { $$ = std::make_unique<call_node>($1); }
     | tIDENTIFIER '(' exprs ')' { $$ = std::make_unique<call_node>($1, std::move($3)); }
     ;

exprs : expr           { $$ = std::make_unique<group_node>(std::move($1)); }
      | exprs ',' expr { $$ = std::make_unique<group_node>(std::move($3), std::move($1)); }
      ;

lvalue : tIDENTIFIER { $$ = std::make_unique<lvalue_node>($1); }
       ;

%%

namespace yaya {
    void Parser::error(const std::string& msg) {
        std::cout << "Error: " << msg << "\n";
    }
} // namespace yaya
