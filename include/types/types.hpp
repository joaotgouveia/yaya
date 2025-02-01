#pragma once

#include <llvm/IR/Type.h>

namespace yaya {

llvm::Type* join_types(llvm::Type* t1, llvm::Type* t2);

bool is_numeric(llvm::Type* t);
bool is_int_t(llvm::Type* t);
bool is_bool_t(llvm::Type* t);
bool is_void_t(llvm::Type* t);
bool is_fn_t(llvm::Type* t);

} // namespace yaya
