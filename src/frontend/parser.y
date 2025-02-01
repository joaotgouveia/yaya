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
    #include "ast/function_header_node.hpp"
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
%token tTINT tTVOID tTBOOL
%token tFUNCTION tLET tRETURN
%token tGEQ tLEQ tEQ tNEQ tAND tOR

%type<std::unique_ptr<yaya::basic_node>> program decl inst function arg
%type<std::unique_ptr<yaya::group_node>> insts exprs functions args
%type<std::unique_ptr<yaya::expression_node>> expr simple_expr call binop
%type<llvm::Type*> type ret_type

%%

program : functions   { compiler.ast(std::move($1)); }
        | /* empty */ { compiler.ast(std::make_unique<group_node>()); }
        ;

inst : expr ';'                 { $$ = std::move($1); }
     | decl ';'                 { $$ = std::move($1); }
     | tIDENTIFIER '=' expr ';' { $$ = std::make_unique<assignment_node>(std::move($1), std::move($3)); }
     | tRETURN expr ';'         { $$ = std::make_unique<return_node>(std::move($2)); }
     | tRETURN ';'              { $$ = std::make_unique<return_node>(); }
     ;

insts : inst       { $$ = std::make_unique<group_node>(std::move($1)); }
      | insts inst { $$ = std::make_unique<group_node>(std::move($2), std::move($1)); }
      ;

decl :  tLET type tIDENTIFIER          { $$ = std::make_unique<declaration_node>($3, $2); }
     |  tLET type tIDENTIFIER '=' expr { $$ = std::make_unique<declaration_node>($3, $2, std::move($5)); }
     ;

expr : simple_expr { $$ = std::move($1); }
     | binop       { $$ = std::move($1); }
     ;

simple_expr : tINT        { $$ = std::make_unique<int_literal_node>($1); }
            | tIDENTIFIER { $$ = std::make_unique<lvalue_node>($1); }
            | call        { $$ = std::move($1); }
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

function : tFUNCTION tIDENTIFIER ':' ret_type '(' args ')' '{' insts '}' { $$ = std::make_unique<function_node>($2, std::move($6), std::move($9), compiler.get_fn_t($4)); }
         | tFUNCTION tIDENTIFIER ':' ret_type '(' ')' '{' insts '}'      { $$ = std::make_unique<function_node>($2, std::make_unique<group_node>(), std::move($8), compiler.get_fn_t($4)); }
         | tFUNCTION tIDENTIFIER ':' ret_type '(' args ')' '{' '}'       { $$ = std::make_unique<function_node>($2, std::move($6), std::make_unique<group_node>(), compiler.get_fn_t($4)); }
         | tFUNCTION tIDENTIFIER ':' ret_type '(' ')' '{' '}'            { $$ = std::make_unique<function_node>($2, std::make_unique<group_node>(), std::make_unique<group_node>(), compiler.get_fn_t($4)); }
         | tFUNCTION tIDENTIFIER '(' args ')' '{' insts '}'              { $$ = std::make_unique<function_node>($2, std::move($4), std::move($7), compiler.get_fn_t(compiler.get_void_t())); }
         | tFUNCTION tIDENTIFIER '(' ')' '{' insts '}'                   { $$ = std::make_unique<function_node>($2, std::make_unique<group_node>(), std::move($6), compiler.get_fn_t(compiler.get_void_t())); }
         | tFUNCTION tIDENTIFIER '(' args ')' '{' '}'                    { $$ = std::make_unique<function_node>($2, std::move($4), std::make_unique<group_node>(), compiler.get_fn_t(compiler.get_void_t())); }
         | tFUNCTION tIDENTIFIER '(' ')' '{' '}'                         { $$ = std::make_unique<function_node>($2, std::make_unique<group_node>(), std::make_unique<group_node>(), compiler.get_fn_t(compiler.get_void_t())); }
         | tFUNCTION tIDENTIFIER ':' ret_type '(' args ')' ';'           { $$ = std::make_unique<function_header_node>($2, std::move($6), compiler.get_fn_t($4)); }
         | tFUNCTION tIDENTIFIER ':' ret_type '(' ')' ';'                { $$ = std::make_unique<function_header_node>($2, std::make_unique<group_node>(), compiler.get_fn_t($4)); }
         | tFUNCTION tIDENTIFIER '(' args ')' ';'                        { $$ = std::make_unique<function_header_node>($2, std::move($4), compiler.get_fn_t(compiler.get_void_t())); }
         | tFUNCTION tIDENTIFIER '(' ')' ';'                             { $$ = std::make_unique<function_header_node>($2, std::make_unique<group_node>(), compiler.get_fn_t(compiler.get_void_t())); }
         ;

functions : function           { $$ = std::make_unique<group_node>(std::move($1)); }
          | functions function { $$ = std::make_unique<group_node>(std::move($2), std::move($1)); }
          ;

ret_type : type   { $$ = $1; }
         | tTVOID { $$ = compiler.get_void_t(); }
         ;

type : tTINT  { $$ = compiler.get_int_t(); }
     | tTBOOL { $$ = compiler.get_bool_t(); }
     ;

arg : type tIDENTIFIER { $$ = std::make_unique<declaration_node>($2, $1); }
    ;

args : arg          { $$ = std::make_unique<group_node>(std::move($1)); }
     | args ',' arg { $$ = std::make_unique<group_node>(std::move($3), std::move($1)); }
     ;

call : tIDENTIFIER '(' ')'       { $$ = std::make_unique<call_node>($1); }
     | tIDENTIFIER '(' exprs ')' { $$ = std::make_unique<call_node>($1, std::move($3)); }
     ;

exprs : expr           { $$ = std::make_unique<group_node>(std::move($1)); }
      | exprs ',' expr { $$ = std::make_unique<group_node>(std::move($3), std::move($1)); }
      ;

%%

namespace yaya {
    void Parser::error(const std::string& msg) {
        std::cout << "Error: " << msg << "\n";
    }
} // namespace yaya
