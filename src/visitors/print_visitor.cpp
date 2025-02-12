#include "visitors/print_visitor.hpp"
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
#include <iostream>

namespace yaya {

void PrintVisitor::open_tag(const std::string& name) {
    std::cout << std::string(_indent, ' ') << "<" << name << ">\n";
    _indent += 4;
}

void PrintVisitor::close_tag(const std::string& name) {
    _indent -= 4;
    std::cout << std::string(_indent, ' ') << "</" << name << ">\n";
}

void* PrintVisitor::do_node(basic_node& node) {
    _indent = 0;
    node.accept(this);
    return nullptr;
}

void* PrintVisitor::do_node(add_node* node) {
    open_tag("add_node type=" + node->type_str());
    node->lhs()->accept(this);
    node->rhs()->accept(this);
    close_tag("add_node");
    return nullptr;
}

void* PrintVisitor::do_node(and_node* node) {
    open_tag("and_node type=" + node->type_str());
    node->lhs()->accept(this);
    node->rhs()->accept(this);
    close_tag("and_node");
    return nullptr;
}

void* PrintVisitor::do_node(assignment_node* node) {
    open_tag("assignment_node id=" + node->identifier());
    node->expr()->accept(this);
    close_tag("assignment_node");
    return nullptr;
}

void* PrintVisitor::do_node(call_node* node) {
    open_tag("call_node id=" + node->identifier() +
             " type=" + node->type_str());
    node->args()->accept(this);
    close_tag("call_node");
    return nullptr;
}

void* PrintVisitor::do_node(declaration_node* node) {
    open_tag("declaration_node id=" + node->identifier() +
             " type=" + node->type_str());
    if (node->initializer() != nullptr) {
        node->initializer()->accept(this);
    }
    close_tag("declaration_node");
    return nullptr;
}

void* PrintVisitor::do_node(div_node* node) {
    open_tag("div_node type=" + node->type_str());
    node->lhs()->accept(this);
    node->rhs()->accept(this);
    close_tag("div_node");
    return nullptr;
}

void* PrintVisitor::do_node(eq_node* node) {
    open_tag("eq_node type=" + node->type_str());
    node->lhs()->accept(this);
    node->rhs()->accept(this);
    close_tag("eq_node");
    return nullptr;
}

void* PrintVisitor::do_node(function_header_node* node) {
    open_tag("function_header_node id=" + node->identifier() +
             " type=" + node->type_str());
    node->args()->accept(this);
    close_tag("function_header_node");
    return nullptr;
}

void* PrintVisitor::do_node(function_node* node) {
    open_tag("function_node id=" + node->identifier() +
             " type=" + node->type_str());
    node->args()->accept(this);
    node->body()->accept(this);
    close_tag("function_node");
    return nullptr;
}

void* PrintVisitor::do_node(geq_node* node) {
    open_tag("geq_node type=" + node->type_str());
    node->lhs()->accept(this);
    node->rhs()->accept(this);
    close_tag("geq_node");
    return nullptr;
}

void* PrintVisitor::do_node(group_node* node) {
    open_tag("group_node");
    for (const auto& el : node->elements()) {
        el->accept(this);
    }
    close_tag("group_node");
    return nullptr;
}

void* PrintVisitor::do_node(gt_node* node) {
    open_tag("gt_node type=" + node->type_str());
    node->lhs()->accept(this);
    node->rhs()->accept(this);
    close_tag("gt_node");
    return nullptr;
}

void* PrintVisitor::do_node(int_literal_node* node) {
    std::cout << std::string(_indent, ' ')
              << "<int_literal_node val=" << node->val()
              << " type=" << node->type_str() << " />\n";
    return nullptr;
}

void* PrintVisitor::do_node(leq_node* node) {
    open_tag("leq_node type=" + node->type_str());
    node->lhs()->accept(this);
    node->rhs()->accept(this);
    close_tag("leq_node");
    return nullptr;
}

void* PrintVisitor::do_node(lt_node* node) {
    open_tag("lt_node type=" + node->type_str());
    node->lhs()->accept(this);
    node->rhs()->accept(this);
    close_tag("lt_node");
    return nullptr;
}

void* PrintVisitor::do_node(lvalue_node* node) {
    std::cout << std::string(_indent, ' ')
              << "<lvalue_node id=" << node->identifier()
              << " type=" << node->type_str() << " />\n";
    return nullptr;
}

void* PrintVisitor::do_node(mod_node* node) {
    open_tag("mod_node type=" + node->type_str());
    node->lhs()->accept(this);
    node->rhs()->accept(this);
    close_tag("mod_node");
    return nullptr;
}

void* PrintVisitor::do_node(mul_node* node) {
    open_tag("mul_node type=" + node->type_str());
    node->lhs()->accept(this);
    node->rhs()->accept(this);
    close_tag("mul_node");
    return nullptr;
}

void* PrintVisitor::do_node(neq_node* node) {
    open_tag("neq_node type=" + node->type_str());
    node->lhs()->accept(this);
    node->rhs()->accept(this);
    close_tag("neq_node");
    return nullptr;
}

void* PrintVisitor::do_node(or_node* node) {
    open_tag("or_node type=" + node->type_str());
    node->lhs()->accept(this);
    node->rhs()->accept(this);
    close_tag("or_node");
    return nullptr;
}

void* PrintVisitor::do_node(return_node* node) {
    open_tag("return_node type=" + node->type_str());
    node->expression()->accept(this);
    close_tag("return_node");
    return nullptr;
}

void* PrintVisitor::do_node(sub_node* node) {
    open_tag("sub_node type=" + node->type_str());
    node->lhs()->accept(this);
    node->rhs()->accept(this);
    close_tag("sub_node");
    return nullptr;
}

} // namespace yaya
