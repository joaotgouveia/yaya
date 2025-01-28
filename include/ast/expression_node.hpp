#pragma once

#include "basic_node.hpp"

namespace yaya {

// TODO: Inherit from typed node like in cdk
class expression_node : public basic_node {

  public:
    expression_node() : basic_node() {}
};

} // namespace yaya
