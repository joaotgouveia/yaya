#pragma once

#include "ast/basic_node.hpp"
#include "ast/expression_node.hpp"
#include "visitors/abstract_ast_visitor.hpp"
#include <memory>
#include <string>

namespace yaya {

class declaration_node : public basic_node {

    std::string _identifier;
    std::unique_ptr<expression_node> _initializer;

  public:
    declaration_node(std::string& identifier,
                     std::unique_ptr<expression_node> initializer = nullptr)
        : basic_node(), _identifier(identifier),
          _initializer(std::move(initializer)) {}

    const std::string& identifier() const { return _identifier; }
    const std::unique_ptr<expression_node>& initializer() const {
        return _initializer;
    }

    void accept(AbstractASTVisitor* visitor) override {
        visitor->do_node(this);
    }
};

} // namespace yaya
