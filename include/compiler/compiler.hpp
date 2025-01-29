#pragma once

#include "ast/basic_node.hpp"
#include "frontend/lexer.hpp"
#include "parser.tab.hh"
#include "visitors/abstract_ast_visitor.hpp"
#include <fstream>
#include <memory>
#include <string>

namespace yaya {

class Compiler {
    std::ifstream _input;
    std::unique_ptr<AbstractASTVisitor> _visitor;
    std::unique_ptr<Lexer> _lexer = nullptr;
    std::unique_ptr<Parser> _parser = nullptr;
    std::unique_ptr<basic_node> _ast = nullptr;

    bool parse();

  public:
    Compiler(const std::string& input,
             std::unique_ptr<AbstractASTVisitor> visitor)
        : _input(input), _visitor(std::move(visitor)) {}

    bool compile();

    const std::unique_ptr<basic_node>& ast() const { return _ast; }
    void ast(std::unique_ptr<basic_node> ast) { _ast = std::move(ast); }
};

} // namespace yaya
