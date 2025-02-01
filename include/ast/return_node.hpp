#pragma once

#include "ast/expression_node.hpp"
#include "ast/typed_node.hpp"
#include "visitors/abstract_ast_visitor.hpp"
#include <memory>

namespace yaya {

class return_node : public yaya::typed_node {

    std::unique_ptr<expression_node> _expression;

  public:
    return_node(std::unique_ptr<expression_node> expression = nullptr)
        : typed_node(), _expression(std::move(expression)) {}

    const std::unique_ptr<expression_node>& expression() const {
        return _expression;
    }

    void* accept(AbstractASTVisitor* visitor) override {
        return visitor->do_node(this);
    }
};
} // namespace yaya
