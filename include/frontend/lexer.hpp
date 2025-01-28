#pragma once

#include "parser.tab.hh"
#include <string>

#if !defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif

namespace yaya {

class Lexer : public yyFlexLexer {
    Parser::semantic_type* yylval = nullptr;

  public:
    Lexer(std::istream* istream) : yyFlexLexer(istream) {}
    virtual ~Lexer() {}

    virtual int yylex(Parser::semantic_type* const lval);
};

} // namespace yaya
