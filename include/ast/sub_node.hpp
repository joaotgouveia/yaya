#pragma once

#include "ast/binop_node.hpp"
#include "visitors/abstract_ast_visitor.hpp"
#include <memory>

namespace yaya {

class sub_node : public binop_node {
  public:
    sub_node(std::unique_ptr<expression_node> lhs,
             std::unique_ptr<expression_node> rhs)
        : binop_node(std::move(lhs), std::move(rhs)) {}

    void* accept(AbstractASTVisitor* visitor) override {
        return visitor->do_node(this);
    }
};

} // namespace yaya
