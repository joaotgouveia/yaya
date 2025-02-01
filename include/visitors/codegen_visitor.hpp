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
#include "compiler/compiler.hpp"
#include "compiler/symbol_table.hpp"
#include "visitors/abstract_ast_visitor.hpp"
#include <fstream>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/IR/Module.h>
#include <llvm/MC/TargetRegistry.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/Target/TargetOptions.h>
#include <llvm/TargetParser/Host.h>
#include <memory>
#include <string>

namespace yaya {

class CodeGenVisitor : public AbstractASTVisitor {
    Compiler* _compiler;
    std::error_code _ec;
    llvm::legacy::PassManager _emit_file;
    llvm::raw_fd_ostream _output;
    std::unique_ptr<SymbolTable<llvm::Value*>> _symbol_table =
        std::make_unique<SymbolTable<llvm::Value*>>();
    std::unique_ptr<llvm::Module> _module =
        std::make_unique<llvm::Module>("YaYa Compiler", _compiler->context());
    std::unique_ptr<llvm::IRBuilder<>> _builder =
        std::make_unique<llvm::IRBuilder<>>(_compiler->context());

  public:
    CodeGenVisitor(Compiler* compiler, const std::string& output)
        : AbstractASTVisitor(), _compiler(compiler),
          _output(output, _ec, llvm::sys::fs::OF_None) {

        llvm::InitializeNativeTarget();
        llvm::InitializeNativeTargetAsmParser();
        llvm::InitializeNativeTargetAsmPrinter();

        std::string error;
        auto tt = llvm::sys::getDefaultTargetTriple();
        auto target = llvm::TargetRegistry::lookupTarget(tt, error);
        if (!target) {
            llvm::errs() << error;
            return;
        }

        auto cpu = "generic";
        auto features = "";
        llvm::TargetOptions opt;
        auto machine = target->createTargetMachine(tt, cpu, features, opt,
                                                   llvm::Reloc::PIC_);

        _module->setDataLayout(machine->createDataLayout());
        _module->setTargetTriple(tt);

        if (_ec) {
            llvm::errs() << "Could not open file: " << _ec.message();
            return;
        }

        auto ft = llvm::CodeGenFileType::ObjectFile;
        if (machine->addPassesToEmitFile(_emit_file, _output, nullptr, ft)) {
            llvm::errs() << "Target machine can't emit a file of this type";
        }
    }

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
