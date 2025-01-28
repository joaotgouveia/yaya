#pragma once

#include "ast/basic_node.hpp"
#include "ast/expression_node.hpp"
#include "ast/lvalue_node.hpp"
#include "visitors/abstract_ast_visitor.hpp"
#include <memory>
#include <string>

namespace yaya {

class assignment_node : public basic_node {

    std::unique_ptr<lvalue_node> _lvalue;
    std::unique_ptr<expression_node> _expr;

  public:
    assignment_node(std::unique_ptr<lvalue_node> lvalue,
                    std::unique_ptr<expression_node> expr)
        : basic_node(), _lvalue(std::move(lvalue)), _expr(std::move(expr)) {}

    const std::unique_ptr<lvalue_node>& lvalue() const { return _lvalue; }
    const std::unique_ptr<expression_node>& expr() const { return _expr; }

    void accept(AbstractASTVisitor* visitor) override {
        visitor->do_node(this);
    }
};

} // namespace yaya
