#pragma once

#include "ast/expression_node.hpp"
#include "visitors/abstract_ast_visitor.hpp"
#include <string>

namespace yaya {

class lvalue_node : public expression_node {

    std::string _identifier;

  public:
    lvalue_node(std::string& identifier)
        : expression_node(), _identifier(identifier) {}

    const std::string& identifier() const { return _identifier; }

    void* accept(AbstractASTVisitor* visitor) override {
        return visitor->do_node(this);
    }
};

} // namespace yaya
