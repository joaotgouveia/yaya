#include "visitors/type_visitor.hpp"
#include "ast/add_node.hpp"
#include "ast/and_node.hpp"
#include "ast/assignment_node.hpp"
#include "ast/basic_node.hpp"
#include "ast/call_node.hpp"
#include "ast/declaration_node.hpp"
#include "ast/div_node.hpp"
#include "ast/eq_node.hpp"
#include "ast/function_header_node.hpp"
#include "ast/function_node.hpp"
#include "ast/geq_node.hpp"
#include "ast/group_node.hpp"
#include "ast/gt_node.hpp"
#include "ast/int_literal_node.hpp"
#include "ast/leq_node.hpp"
#include "ast/lt_node.hpp"
#include "ast/lvalue_node.hpp"
#include "ast/mod_node.hpp"
#include "ast/mul_node.hpp"
#include "ast/neq_node.hpp"
#include "ast/or_node.hpp"
#include "ast/return_node.hpp"
#include "ast/sub_node.hpp"
#include "ast/typed_node.hpp"
#include "types/types.hpp"
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Type.h>
#include <memory>

namespace yaya {

bool TypeVisitor::do_binop_types(typed_node& lhs, typed_node& rhs) {
    lhs.accept(this);
    rhs.accept(this);
    return join_types(lhs.type(), rhs.type()) != nullptr;
}

void* TypeVisitor::do_node(basic_node& node) {
    _symbol_table->push(); /* Global context */
    node.accept(this);
    _symbol_table->pop();
    return nullptr;
}

void* TypeVisitor::do_node(add_node* node) {
    if (!do_binop_types(*node->lhs(), *node->rhs())) {
        // TODO: Report error
    } else {
        node->type(node->lhs()->type());
    }
    return nullptr;
}

void* TypeVisitor::do_node(and_node* node) {
    if (!do_binop_types(*node->lhs(), *node->rhs())) {
        // TODO: Report error
    }
    node->type(_compiler->get_bool_t());
    return nullptr;
}

void* TypeVisitor::do_node(assignment_node* node) {
    llvm::Type* type = _symbol_table->find(node->identifier());
    if (!type) {
        // TODO: Report error
        return nullptr;
    }

    node->expr()->accept(this);
    llvm::Type* join = join_types(type, node->expr()->type());
    if (join != type) {
        // TODO: Report error
        return nullptr;
    }
    node->expr()->type(join);

    return nullptr;
}

void* TypeVisitor::do_node(call_node* node) {
    llvm::FunctionType* type = static_cast<llvm::FunctionType*>(
        _symbol_table->find(node->identifier()));
    if (!type) {
        // TODO: Report error
        return nullptr;
    }

    node->type(type->getReturnType());
    node->args()->accept(this);
    if (node->args()->length() != type->getNumParams()) {
        // TODO: Report error
        return nullptr;
    }

    size_t i = 0;
    for (auto& arg : node->args()->elements()) {
        declaration_node* decl = static_cast<declaration_node*>(arg.get());
        if (!decl) {
            // TODO: Report error
            return nullptr;
        }
        llvm::Type* join = join_types(type->getParamType(i), decl->type());
        if (join != type->getParamType(i++)) {
            // TODO: Report error
            return nullptr;
        }
    }

    return nullptr;
}

void* TypeVisitor::do_node(declaration_node* node) {
    if (_symbol_table->defined(node->identifier())) {
        // TODO: Report error
        return nullptr;
    }

    _symbol_table->insert(node->identifier(), node->type());
    if (node->initializer() != nullptr) {
        node->initializer()->accept(this);
        llvm::Type* join =
            join_types(node->type(), node->initializer()->type());
        if (join != node->type()) {
            // TODO: Report error
            return nullptr;
        }
        node->initializer()->type(join);
    }
    return nullptr;
}

void* TypeVisitor::do_node(div_node* node) {
    if (!do_binop_types(*node->lhs(), *node->rhs())) {
        // TODO: Report error
    } else {
        // TODO: Div should always be double
        node->type(node->lhs()->type());
    }
    return nullptr;
}

void* TypeVisitor::do_node(eq_node* node) {
    if (!do_binop_types(*node->lhs(), *node->rhs())) {
        // TODO: Report error
    }
    node->type(_compiler->get_bool_t());
    return nullptr;
}

void* TypeVisitor::do_node(function_header_node* node) {
    if (_symbol_table->defined(node->identifier())) {
        // TODO: Report error
        return nullptr;
    }

    _symbol_table->push();
    std::vector<llvm::Type*> args;
    node->args()->accept(this);
    for (const auto& arg : node->args()->elements()) {
        declaration_node* decl = dynamic_cast<declaration_node*>(arg.get());
        args.push_back(decl->type());
    }
    node->type(_compiler->get_fn_t(node->ret_type(), args));
    _symbol_table->pop();

    _symbol_table->insert(node->identifier(), node->type());
    return nullptr;
}

void* TypeVisitor::do_node(function_node* node) {
    if (_symbol_table->defined(node->identifier())) {
        // TODO: Report error
        return nullptr;
    }

    _symbol_table->push();
    std::vector<llvm::Type*> args;
    node->args()->accept(this);
    for (const auto& arg : node->args()->elements()) {
        declaration_node* decl = dynamic_cast<declaration_node*>(arg.get());
        args.push_back(decl->type());
    }
    node->type(_compiler->get_fn_t(node->ret_type(), args));
    node->body()->accept(this);
    _symbol_table->pop();

    _symbol_table->insert(node->identifier(), node->type());
    return nullptr;
}

void* TypeVisitor::do_node(geq_node* node) {
    if (!do_binop_types(*node->lhs(), *node->rhs())) {
        // TODO: Report error
    }
    node->type(_compiler->get_bool_t());
    return nullptr;
}

void* TypeVisitor::do_node(group_node* node) {
    for (const auto& el : node->elements()) {
        el->accept(this);
    }
    return nullptr;
}

void* TypeVisitor::do_node(gt_node* node) {
    if (!do_binop_types(*node->lhs(), *node->rhs())) {
        // TODO: Report error
    }
    node->type(_compiler->get_bool_t());
    return nullptr;
}

void* TypeVisitor::do_node(int_literal_node* node) {
    node->type(_compiler->get_int_t());
    return nullptr;
}

void* TypeVisitor::do_node(leq_node* node) {
    if (!do_binop_types(*node->lhs(), *node->rhs())) {
        // TODO: Report error
    }
    node->type(_compiler->get_bool_t());
    return nullptr;
}

void* TypeVisitor::do_node(lt_node* node) {
    if (!do_binop_types(*node->lhs(), *node->rhs())) {
        // TODO: Report error
    }
    node->type(_compiler->get_bool_t());
    return nullptr;
}

void* TypeVisitor::do_node(lvalue_node* node) {
    llvm::Type* type = _symbol_table->find(node->identifier());
    if (!type) {
        // TODO: Report error
    } else {
        node->type(type);
    }
    return nullptr;
}

void* TypeVisitor::do_node(mod_node* node) {
    node->lhs()->accept(this);
    node->rhs()->accept(this);
    if (!is_int_t(node->lhs()->type()) || !is_int_t(node->rhs()->type())) {
        // TODO: Report error
    } else {
        node->type(_compiler->get_int_t());
    }
    return nullptr;
}

void* TypeVisitor::do_node(mul_node* node) {
    if (!do_binop_types(*node->lhs(), *node->rhs())) {
        // TODO: Report error
    } else {
        node->type(node->lhs()->type());
    }
    return nullptr;
}

void* TypeVisitor::do_node(neq_node* node) {
    if (!do_binop_types(*node->lhs(), *node->rhs())) {
        // TODO: Report error
    }
    node->type(_compiler->get_bool_t());
    return nullptr;
}

void* TypeVisitor::do_node(or_node* node) {
    if (!do_binop_types(*node->lhs(), *node->rhs())) {
        // TODO: Report error
    }
    node->type(_compiler->get_bool_t());
    return nullptr;
}

void* TypeVisitor::do_node(return_node* node) {
    if (node->expression()) {
        node->expression()->accept(this);
        node->type(node->expression()->type());
    } else {
        node->type(_compiler->get_void_t());
    }
    return nullptr;
}

void* TypeVisitor::do_node(sub_node* node) {
    if (!do_binop_types(*node->lhs(), *node->rhs())) {
        // TODO: Report error
    } else {
        node->type(node->lhs()->type());
    }
    return nullptr;
}

} // namespace yaya
