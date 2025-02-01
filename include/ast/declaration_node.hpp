#pragma once

#include "ast/expression_node.hpp"
#include "ast/typed_node.hpp"
#include "visitors/abstract_ast_visitor.hpp"
#include <llvm/IR/Type.h>
#include <memory>
#include <string>

namespace yaya {

class declaration_node : public typed_node {

    std::string _identifier;
    std::unique_ptr<expression_node> _initializer;

  public:
    declaration_node(std::string& identifier, llvm::Type* type,
                     std::unique_ptr<expression_node> initializer = nullptr)
        : typed_node(type), _identifier(identifier),
          _initializer(std::move(initializer)) {}

    const std::string& identifier() const { return _identifier; }
    const std::unique_ptr<expression_node>& initializer() const {
        return _initializer;
    }

    void* accept(AbstractASTVisitor* visitor) override {
        return visitor->do_node(this);
    }
};

} // namespace yaya
