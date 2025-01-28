#pragma once

#include "ast/basic_node.hpp"
#include "visitors/abstract_ast_visitor.hpp"
#include <memory>
#include <vector>

namespace yaya {

class group_node : public basic_node {

    std::vector<std::unique_ptr<basic_node>> _elements;

  public:
    group_node(std::unique_ptr<basic_node> element,
               std::unique_ptr<group_node> group = nullptr)
        : basic_node() {
        if (group != nullptr) {
            _elements = std::move(group->_elements);
        }
        _elements.push_back(std::move(element));
    }

    const std::vector<std::unique_ptr<basic_node>>& elements() const {
        return _elements;
    }
    size_t length() const { return _elements.size(); }

    void accept(AbstractASTVisitor* visitor) override {
        visitor->do_node(this);
    }
};

} // namespace yaya
