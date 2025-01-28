#pragma once

#include "ast/binop_node.hpp"
#include "visitors/abstract_ast_visitor.hpp"
#include <memory>

namespace yaya {

class mod_node : public binop_node {
  public:
    mod_node(std::unique_ptr<expression_node> lhs,
             std::unique_ptr<expression_node> rhs)
        : binop_node(std::move(lhs), std::move(rhs)) {}

    void accept(AbstractASTVisitor* visitor) override {
        visitor->do_node(this);
    }
};

} // namespace yaya
