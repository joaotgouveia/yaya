#pragma once

#include "ast/expression_node.hpp"
#include <memory>

namespace yaya {

class binop_node : public expression_node {

    std::unique_ptr<expression_node> _lhs;
    std::unique_ptr<expression_node> _rhs;

  public:
    binop_node(std::unique_ptr<expression_node> lhs,
               std::unique_ptr<expression_node> rhs)
        : expression_node(), _lhs(std::move(lhs)), _rhs(std::move(rhs)) {}

    const std::unique_ptr<expression_node>& lhs() const { return _lhs; }
    const std::unique_ptr<expression_node>& rhs() const { return _rhs; }
};

} // namespace yaya
