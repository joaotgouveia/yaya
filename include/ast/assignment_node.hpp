#pragma once

#include "ast/basic_node.hpp"
#include "ast/expression_node.hpp"
#include "ast/lvalue_node.hpp"
#include "visitors/abstract_ast_visitor.hpp"
#include <memory>
#include <string>

namespace yaya {

class assignment_node : public basic_node {

    std::string _identifier;
    std::unique_ptr<expression_node> _expr;

  public:
    assignment_node(const std::string& identifier,
                    std::unique_ptr<expression_node> expr)
        : basic_node(), _identifier(std::move(identifier)),
          _expr(std::move(expr)) {}

    const std::string& identifier() const { return _identifier; }
    const std::unique_ptr<expression_node>& expr() const { return _expr; }

    void* accept(AbstractASTVisitor* visitor) override {
        return visitor->do_node(this);
    }
};

} // namespace yaya
