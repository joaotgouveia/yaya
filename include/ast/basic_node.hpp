#pragma once

#include "visitors/abstract_ast_visitor.hpp"

namespace yaya {

class basic_node {
    // TODO: Add locations, improve errors
  public:
    basic_node() = default;

    virtual void* accept(AbstractASTVisitor* visitor) = 0;
};

} // namespace yaya
