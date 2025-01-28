#pragma once

#include "ast/basic_node.hpp"
#include "ast/group_node.hpp"
#include "visitors/abstract_ast_visitor.hpp"
#include <memory>
#include <string>

namespace yaya {

// TODO: Inherit from typed node like in cdk
class function_node : public basic_node {

    std::string _identifier;
    std::unique_ptr<group_node> _args;
    std::unique_ptr<group_node> _body;

  public:
    function_node(std::string& identifier, std::unique_ptr<group_node> args,
                  std::unique_ptr<group_node> body)
        : basic_node(), _identifier(identifier), _args(std::move(args)),
          _body(std::move(body)) {}

    const std::string& identifier() const { return _identifier; }
    const std::unique_ptr<group_node>& args() const { return _args; }
    const std::unique_ptr<group_node>& body() const { return _body; }

    void accept(AbstractASTVisitor* visitor) override {
        visitor->do_node(this);
    }
};

} // namespace yaya
