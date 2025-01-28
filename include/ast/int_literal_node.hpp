#pragma once

#include "ast/expression_node.hpp"
#include "visitors/abstract_ast_visitor.hpp"

namespace yaya {

class int_literal_node : public expression_node {

    int _val;

  public:
    int_literal_node(int val) : expression_node(), _val(val) {}

    int val() const { return _val; }

    void accept(AbstractASTVisitor* visitor) override {
        visitor->do_node(this);
    }
};

} // namespace yaya
