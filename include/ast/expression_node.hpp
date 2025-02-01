#pragma once

#include "typed_node.hpp"
#include <llvm/IR/Type.h>

namespace yaya {

class expression_node : public typed_node {

  public:
    expression_node() : typed_node() {}
    expression_node(llvm::Type* type) : typed_node(type) {}
};

} // namespace yaya
