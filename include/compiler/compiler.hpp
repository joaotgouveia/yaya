#pragma once

#include "ast/basic_node.hpp"
#include "frontend/lexer.hpp"
#include "parser.tab.hh"
#include "visitors/abstract_ast_visitor.hpp"
#include <fstream>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Type.h>
#include <memory>
#include <string>
#include <vector>

namespace yaya {

class Compiler {
    std::ifstream _input;
    std::unique_ptr<Lexer> _lexer = nullptr;
    std::unique_ptr<Parser> _parser = nullptr;
    std::unique_ptr<basic_node> _ast = nullptr;
    std::vector<std::unique_ptr<AbstractASTVisitor>> _visitors;
    std::unique_ptr<llvm::LLVMContext> _context =
        std::make_unique<llvm::LLVMContext>();

    bool parse();

  public:
    Compiler(const std::string& input) : _input(input) {}

    bool compile();

    const std::unique_ptr<basic_node>& ast() const { return _ast; }
    void ast(std::unique_ptr<basic_node> ast) { _ast = std::move(ast); }

    void visitors(std::vector<std::unique_ptr<AbstractASTVisitor>> visitors) {
        _visitors = std::move(visitors);
    }

    llvm::LLVMContext& context() { return *_context; }

    llvm::Type* get_int_t() { return llvm::Type::getInt64Ty(*_context); }
    llvm::Type* get_bool_t() { return llvm::Type::getInt1Ty(*_context); }
    llvm::Type* get_void_t() { return llvm::Type::getVoidTy(*_context); }

    llvm::FunctionType*
    get_fn_t(llvm::Type* ret,
             std::vector<llvm::Type*> args = std::vector<llvm::Type*>()) {
        return llvm::FunctionType::get(ret, args, false);
    }
};

} // namespace yaya
