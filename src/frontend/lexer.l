%{
#include <string>
#include "frontend/lexer.hpp"

#undef  YY_DECL
#define YY_DECL int yaya::Lexer::yylex(yaya::Parser::semantic_type* const lval)

#define YY_USER_INIT yylval = lval;

using token = yaya::Parser::token;
%}

%option debug
%option nodefault
%option yyclass="yaya::Lexer"
%option noyywrap
%option c++

%%

[ \n\r\t]+              ; /* ignore whitespaces */

"#".*                   ; /* ignore comments */

"fn"                    return token::tFUNCTION;

"let"                   return token::tLET;

"return"                return token::tRETURN;

">="                    return token::tGEQ;
"<="                    return token::tLEQ;
"=="                    return token::tEQ;
"!="                    return token::tNEQ;
"&&"                    return token::tAND;
"||"                    return token::tOR;

[[:alpha:]][[:alnum:]]* yylval->build<std::string>(yytext); return token::tIDENTIFIER;

[[:digit:]]+            {
                            try {
                                yylval->build<int>(std::stoi(yytext, nullptr, 10));
                                return token::tINT;
                            } catch (const std::out_of_range&) {
                                // TODO: Improve error messages?
                                /* error */
                            }
                        }

[-+*/%<>();={}]         return *yytext;

.                       return *yytext; /* error */

%%
