#pragma once

#include "ast/group_node.hpp"
#include "ast/typed_node.hpp"
#include "visitors/abstract_ast_visitor.hpp"
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Type.h>
#include <memory>
#include <string>
#include <vector>

namespace yaya {

class function_header_node : public typed_node {

    std::string _identifier;
    std::unique_ptr<group_node> _args;

  public:
    function_header_node(std::string& identifier,
                         std::unique_ptr<group_node> args,
                         llvm::FunctionType* type)
        : typed_node(type), _identifier(identifier), _args(std::move(args)) {}

    const std::string& identifier() const { return _identifier; }
    const std::unique_ptr<group_node>& args() const { return _args; }

    llvm::Type* ret_type() const {
        if (!_type) {
            return nullptr;
        }
        llvm::FunctionType* ftype = static_cast<llvm::FunctionType*>(_type);
        if (!ftype) {
            return nullptr;
        }
        return ftype->getReturnType();
    }

    void* accept(AbstractASTVisitor* visitor) override {
        return visitor->do_node(this);
    }
};

} // namespace yaya
