#pragma once

#include "ast/basic_node.hpp"
#include "types/types.hpp"
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Type.h>
#include <string>

namespace yaya {

class typed_node : public basic_node {

  protected:
    llvm::Type* _type;

  public:
    typed_node() = default;
    typed_node(llvm::Type* type) : _type(type) {}

    void type(llvm::Type* type) { _type = type; }
    llvm::Type* type() const { return _type; }

    std::string type_str() const {
        if (!_type) {
            return "\"notset\"";
        } else if (is_int_t(_type)) {
            return "\"int\"";
        } else if (is_bool_t(_type)) {
            return "\"bool\"";
        } else if (is_fn_t(_type)) {
            std::string type = "\"";
            llvm::FunctionType* ftype = static_cast<llvm::FunctionType*>(_type);

            type += "(";
            for (const auto& t : ftype->params()) {
                if (!t) {
                    type += "notset, ";
                } else if (is_int_t(t)) {
                    type += "int, ";
                } else if (is_bool_t(t)) {
                    type += "bool, ";
                } else if (is_void_t(t)) {
                    type += "void, ";
                } else {
                    type += "unknown, ";
                }
            }

            if (ftype->getNumParams() != 0) {
                type.pop_back();
                type.pop_back();
            }

            type += ") -> ";

            if (is_int_t(ftype->getReturnType())) {
                type += "int";
            } else if (is_bool_t(ftype->getReturnType())) {
                type += "bool";
            } else if (is_void_t(ftype->getReturnType())) {
                type += "void";
            } else if (!ftype->getReturnType()) {
                type += "notset";
            } else {
                type += "unknown";
            }
            return type + "\"";
        }
        return "\"unknown\"";
    }
};

} // namespace yaya
