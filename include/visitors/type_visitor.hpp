#pragma once

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
#include "compiler/compiler.hpp"
#include "compiler/symbol_table.hpp"
#include "visitors/abstract_ast_visitor.hpp"
#include <llvm/IR/Type.h>
#include <memory>
#include <string>

namespace yaya {

class TypeVisitor : public AbstractASTVisitor {

    Compiler* _compiler;
    std::unique_ptr<SymbolTable<llvm::Type*>> _symbol_table =
        std::make_unique<SymbolTable<llvm::Type*>>();
    bool do_binop_types(typed_node& lhs, typed_node& rhs);

  public:
    TypeVisitor(Compiler* compiler)
        : AbstractASTVisitor(), _compiler(compiler) {}

    void* do_node(basic_node& node) override;

    void* do_node(add_node* node) override;
    void* do_node(and_node* node) override;
    void* do_node(assignment_node* node) override;
    void* do_node(call_node* node) override;
    void* do_node(declaration_node* node) override;
    void* do_node(div_node* node) override;
    void* do_node(eq_node* node) override;
    void* do_node(function_header_node* node) override;
    void* do_node(function_node* node) override;
    void* do_node(geq_node* node) override;
    void* do_node(group_node* node) override;
    void* do_node(gt_node* node) override;
    void* do_node(int_literal_node* node) override;
    void* do_node(leq_node* node) override;
    void* do_node(lt_node* node) override;
    void* do_node(lvalue_node* node) override;
    void* do_node(mod_node* node) override;
    void* do_node(mul_node* node) override;
    void* do_node(neq_node* node) override;
    void* do_node(or_node* node) override;
    void* do_node(return_node* node) override;
    void* do_node(sub_node* node) override;
};

} // namespace yaya
