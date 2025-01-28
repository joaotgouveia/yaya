#pragma once

#include "ast/expression_node.hpp"
#include "ast/group_node.hpp"
#include <memory>
#include <string>

namespace yaya {

class call_node : public expression_node {

    std::string _identifier;
    std::unique_ptr<group_node> _args;

  public:
    call_node(std::string& identifier,
              std::unique_ptr<group_node> args = nullptr)
        : expression_node(), _identifier(identifier), _args(std::move(args)) {}

    const std::string& identifier() const { return _identifier; }
    const std::unique_ptr<group_node>& args() const { return _args; }

    void accept(AbstractASTVisitor* visitor) override {
        visitor->do_node(this);
    }
};

} // namespace yaya
