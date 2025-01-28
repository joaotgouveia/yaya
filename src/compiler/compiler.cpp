#include "compiler/compiler.hpp"
#include <fstream>
#include <memory>

namespace yaya {

bool Compiler::parse() {
    if (!_input.good()) {
        return false;
    }

    _lexer = std::make_unique<Lexer>(&_input);
    _parser = std::make_unique<Parser>(*_lexer, *this);
    return _parser->parse() == 0;
}

bool Compiler::compile() {
    if (_ast == nullptr && !parse()) {
        return false;
    }
    _visitor->do_node(*_ast);
    return true;
}

} // namespace yaya
