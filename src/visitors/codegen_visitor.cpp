#include "visitors/codegen_visitor.hpp"
#include "ast/add_node.hpp"
#include "ast/and_node.hpp"
#include "ast/assignment_node.hpp"
#include "ast/basic_node.hpp"
#include "ast/call_node.hpp"
#include "ast/declaration_node.hpp"
#include "ast/div_node.hpp"
#include "ast/eq_node.hpp"
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
#include "types/types.hpp"
#include <cstddef>
#include <llvm/ADT/APInt.h>
#include <llvm/ADT/STLExtras.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Verifier.h>
#include <vector>

namespace yaya {

void* CodeGenVisitor::do_node(basic_node& node) {
    _symbol_table->push(); /* Global context */
    node.accept(this);
    _symbol_table->pop();

    _emit_file.run(*_module);
    _output.flush();

    return nullptr;
}

void* CodeGenVisitor::do_node(add_node* node) {
    llvm::Value* lhs = static_cast<llvm::Value*>(node->lhs()->accept(this));
    llvm::Value* rhs = static_cast<llvm::Value*>(node->rhs()->accept(this));
    if (!lhs || !rhs) {
        // TODO: Report errors
        return nullptr;
    }
    return (void*)_builder->CreateAdd(lhs, rhs, "addtmp");
}

void* CodeGenVisitor::do_node(and_node* node) {
    llvm::Value* lhs = static_cast<llvm::Value*>(node->lhs()->accept(this));
    llvm::Value* rhs = static_cast<llvm::Value*>(node->rhs()->accept(this));
    if (!lhs || !rhs) {
        // TODO: Report errors
        return nullptr;
    }
    return (void*)_builder->CreateLogicalAnd(lhs, rhs, "andtmp");
}

void* CodeGenVisitor::do_node(assignment_node* node) {
    llvm::AllocaInst* alloca =
        static_cast<llvm::AllocaInst*>(_symbol_table->find(node->identifier()));
    llvm::Value* expr = static_cast<llvm::Value*>(node->expr()->accept(this));
    if (!expr || !alloca) {
        // TODO: Report errors
    }
    _builder->CreateStore(expr, alloca);
    return nullptr;
}

void* CodeGenVisitor::do_node(call_node* node) {
    llvm::Function* callee = _module->getFunction(node->identifier());
    if (!callee) {
        // TODO: Report errors
        return nullptr;
    }
    if (callee->arg_size() != node->args()->length()) {
        // TODO: Report errors
        return nullptr;
    }

    std::vector<llvm::Value*> args;
    for (const auto& arg : node->args()->elements()) {
        args.push_back(static_cast<llvm::Value*>(arg->accept(this)));
        if (!args.back()) {
            return nullptr;
        }
    }

    return (void*)_builder->CreateCall(callee, args);
}

void* CodeGenVisitor::do_node(declaration_node* node) {
    llvm::AllocaInst* alloca;
    if (_builder->GetInsertBlock()) {
        alloca =
            _builder->CreateAlloca(node->type(), nullptr, node->identifier());
        if (node->initializer()) {
            llvm::Value* initializer =
                static_cast<llvm::Value*>(node->initializer()->accept(this));
            _builder->CreateStore(initializer, alloca);
        }
    }
    if (!_symbol_table->insert(node->identifier(), alloca)) {
        // TODO: report error
    }
    return nullptr;
}

void* CodeGenVisitor::do_node(div_node* node) {
    llvm::Value* lhs = static_cast<llvm::Value*>(node->lhs()->accept(this));
    llvm::Value* rhs = static_cast<llvm::Value*>(node->rhs()->accept(this));
    if (!lhs || !rhs) {
        // TODO: Report errors
        return nullptr;
    }
    return (void*)_builder->CreateExactSDiv(lhs, rhs, "divtmp");
}

void* CodeGenVisitor::do_node(eq_node* node) {
    llvm::Value* lhs = static_cast<llvm::Value*>(node->lhs()->accept(this));
    llvm::Value* rhs = static_cast<llvm::Value*>(node->rhs()->accept(this));
    if (!lhs || !rhs) {
        // TODO: Report errors
        return nullptr;
    }
    return (void*)_builder->CreateICmpEQ(lhs, rhs, "eqtmp");
}

void* CodeGenVisitor::do_node(function_header_node* node) {
    llvm::FunctionType* ftype = static_cast<llvm::FunctionType*>(node->type());
    if (!ftype) {
        // TODO: Report errors
    }
    llvm::Function* function = llvm::Function::Create(
        ftype, llvm::Function::ExternalLinkage, node->identifier(), *_module);

    _symbol_table->push();
    node->args()->accept(this);
    auto name = _symbol_table->top().begin();
    for (auto& arg : function->args()) {
        arg.setName(name->first);
        ++name;
    }
    _symbol_table->pop();

    if (llvm::verifyFunction(*function, &llvm::errs())) {
        // TODO: Report errors
        function->eraseFromParent();
    }

    return nullptr;
}
void* CodeGenVisitor::do_node(function_node* node) {
    llvm::FunctionType* ftype = static_cast<llvm::FunctionType*>(node->type());
    if (!ftype) {
        // TODO: Report errors
    }
    llvm::Function* function = llvm::Function::Create(
        ftype,
        node->identifier() == "main" ? llvm::Function::ExternalLinkage
                                     : llvm::Function::InternalLinkage,
        node->identifier(), *_module);

    _symbol_table->push();
    llvm::BasicBlock* bb =
        llvm::BasicBlock::Create(_compiler->context(), "entry", function);
    _builder->SetInsertPoint(bb);

    node->args()->accept(this);
    auto name = _symbol_table->top().begin();
    for (auto& arg : function->args()) {
        arg.setName(name->first);
        llvm::AllocaInst* alloca =
            static_cast<llvm::AllocaInst*>(_symbol_table->find(name->first));
        _builder->CreateStore(&arg, alloca);
        ++name;
    }

    node->body()->accept(this);
    _symbol_table->pop();

    for (const llvm::BasicBlock& BB : *function) {
        if (!BB.empty() && BB.back().isTerminator())
            continue;

        // Final return for functions that do not have an explicit return
        if (is_int_t(node->ret_type())) {
            _builder->CreateRet(llvm::ConstantInt::get(_compiler->context(),
                                                       llvm::APInt(64, 0)));
        } else if (is_bool_t(node->ret_type())) {
            _builder->CreateRet(llvm::ConstantInt::get(_compiler->context(),
                                                       llvm::APInt(1, 1)));
        } else if (is_void_t(node->ret_type())) {
            _builder->CreateRetVoid();
        } else {
            // TODO: Report errors
            return nullptr;
        }
    }

    if (llvm::verifyFunction(*function, &llvm::errs())) {
        // TODO: Report errors
        function->eraseFromParent();
    }

    return nullptr;
}

void* CodeGenVisitor::do_node(geq_node* node) {
    llvm::Value* lhs = static_cast<llvm::Value*>(node->lhs()->accept(this));
    llvm::Value* rhs = static_cast<llvm::Value*>(node->rhs()->accept(this));
    if (!lhs || !rhs) {
        // TODO: Report errors
        return nullptr;
    }
    return (void*)_builder->CreateICmpSGE(lhs, rhs, "geqtmp");
}

void* CodeGenVisitor::do_node(group_node* node) {
    for (const auto& el : node->elements()) {
        el->accept(this);
    }
    return nullptr;
}

void* CodeGenVisitor::do_node(gt_node* node) {
    llvm::Value* lhs = static_cast<llvm::Value*>(node->lhs()->accept(this));
    llvm::Value* rhs = static_cast<llvm::Value*>(node->rhs()->accept(this));
    if (!lhs || !rhs) {
        // TODO: Report errors
        return nullptr;
    }
    return (void*)_builder->CreateICmpSGT(lhs, rhs, "gttmp");
}

void* CodeGenVisitor::do_node(int_literal_node* node) {
    return (void*)llvm::ConstantInt::get(_compiler->context(),
                                         llvm::APInt(64, node->val()));
}

void* CodeGenVisitor::do_node(leq_node* node) {
    llvm::Value* lhs = static_cast<llvm::Value*>(node->lhs()->accept(this));
    llvm::Value* rhs = static_cast<llvm::Value*>(node->rhs()->accept(this));
    if (!lhs || !rhs) {
        // TODO: Report errors
        return nullptr;
    }
    return (void*)_builder->CreateICmpSLE(lhs, rhs, "leqtmp");
}

void* CodeGenVisitor::do_node(lt_node* node) {
    llvm::Value* lhs = static_cast<llvm::Value*>(node->lhs()->accept(this));
    llvm::Value* rhs = static_cast<llvm::Value*>(node->rhs()->accept(this));
    if (!lhs || !rhs) {
        // TODO: Report errors
        return nullptr;
    }
    return (void*)_builder->CreateICmpSLT(lhs, rhs, "lttmp");
}

void* CodeGenVisitor::do_node(lvalue_node* node) {
    llvm::AllocaInst* alloca =
        static_cast<llvm::AllocaInst*>(_symbol_table->find(node->identifier()));
    if (!alloca) {
        // TODO: Report errors
    }
    return (void*)_builder->CreateLoad(node->type(), alloca, "loadtmp");
}

void* CodeGenVisitor::do_node(mod_node* node) {
    llvm::Value* lhs = static_cast<llvm::Value*>(node->lhs()->accept(this));
    llvm::Value* rhs = static_cast<llvm::Value*>(node->rhs()->accept(this));
    if (!lhs || !rhs) {
        // TODO: Report errors
        return nullptr;
    }
    llvm::Value* quotient =
        _builder->CreateExactUDiv(lhs, rhs, "modquotienttmp");
    llvm::Value* tmpmul = _builder->CreateMul(rhs, quotient, "modmultmp");
    return (void*)_builder->CreateSub(lhs, tmpmul, "modtmp");
}

void* CodeGenVisitor::do_node(mul_node* node) {
    llvm::Value* lhs = static_cast<llvm::Value*>(node->lhs()->accept(this));
    llvm::Value* rhs = static_cast<llvm::Value*>(node->rhs()->accept(this));
    if (!lhs || !rhs) {
        // TODO: Report errors
        return nullptr;
    }
    return (void*)_builder->CreateMul(lhs, rhs, "multmp");
}

void* CodeGenVisitor::do_node(neq_node* node) {
    llvm::Value* lhs = static_cast<llvm::Value*>(node->lhs()->accept(this));
    llvm::Value* rhs = static_cast<llvm::Value*>(node->rhs()->accept(this));
    if (!lhs || !rhs) {
        // TODO: Report errors
        return nullptr;
    }
    return (void*)_builder->CreateICmpNE(lhs, rhs, "neqtmp");
}

void* CodeGenVisitor::do_node(or_node* node) {
    llvm::Value* lhs = static_cast<llvm::Value*>(node->lhs()->accept(this));
    llvm::Value* rhs = static_cast<llvm::Value*>(node->rhs()->accept(this));
    if (!lhs || !rhs) {
        // TODO: Report errors
        return nullptr;
    }
    return (void*)_builder->CreateLogicalOr(lhs, rhs, "ortmp");
}

void* CodeGenVisitor::do_node(return_node* node) {
    if (!node->expression()) {
        _builder->CreateRetVoid();
        return nullptr;
    }
    llvm::Value* val =
        static_cast<llvm::Value*>(node->expression()->accept(this));
    _builder->CreateRet(val);
    return nullptr;
}

void* CodeGenVisitor::do_node(sub_node* node) {
    llvm::Value* lhs = static_cast<llvm::Value*>(node->lhs()->accept(this));
    llvm::Value* rhs = static_cast<llvm::Value*>(node->rhs()->accept(this));
    if (!lhs || !rhs) {
        // TODO: Report errors
        return nullptr;
    }
    return (void*)_builder->CreateSub(lhs, rhs, "subtmp");
}

} // namespace yaya
