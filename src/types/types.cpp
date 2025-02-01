#include "types/types.hpp"

namespace yaya {

llvm::Type* join_types(llvm::Type* t1, llvm::Type* t2) {
    // TODO
    return nullptr;
}

bool is_numeric(llvm::Type* t) {
    if (!t) {
        return false;
    }
    return t->isIntegerTy(64);
}
bool is_int_t(llvm::Type* t) {
    if (!t) {
        return false;
    }
    return t->isIntegerTy(64);
}
bool is_bool_t(llvm::Type* t) {
    if (!t) {
        return false;
    }
    return t->isIntegerTy(1);
}
bool is_void_t(llvm::Type* t) {
    if (!t) {
        return false;
    }
    return t->isVoidTy();
}
bool is_fn_t(llvm::Type* t) {
    if (!t) {
        return false;
    }
    return t->isFunctionTy();
}

} // namespace yaya
