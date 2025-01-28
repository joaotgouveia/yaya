#include "visitors/print_visitor.hpp"
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

void PrintVisitor::do_node(basic_node& node) {
    _indent = 0;
    node.accept(this);
}

void PrintVisitor::do_node(add_node* node) {
    open_tag("add_node");
    node->lhs()->accept(this);
    node->rhs()->accept(this);
    close_tag("add_node");
}

void PrintVisitor::do_node(and_node* node) {
    open_tag("and_node");
    node->lhs()->accept(this);
    node->rhs()->accept(this);
    close_tag("and_node");
}

void PrintVisitor::do_node(assignment_node* node) {
    open_tag("assignment_node");
    node->lvalue()->accept(this);
    node->expr()->accept(this);
    close_tag("assignment_node");
}

void PrintVisitor::do_node(call_node* node) {
    open_tag("call_node id=" + node->identifier());
    node->args()->accept(this);
    close_tag("call_node");
}

void PrintVisitor::do_node(declaration_node* node) {
    open_tag("declaration_node id=" + node->identifier());
    if (node->initializer() != nullptr) {
        node->initializer()->accept(this);
    }
    close_tag("declaration_node");
}

void PrintVisitor::do_node(div_node* node) {
    open_tag("div_node");
    node->lhs()->accept(this);
    node->rhs()->accept(this);
    close_tag("div_node");
}

void PrintVisitor::do_node(eq_node* node) {
    open_tag("eq_node");
    node->lhs()->accept(this);
    node->rhs()->accept(this);
    close_tag("eq_node");
}

void PrintVisitor::do_node(function_node* node) {
    open_tag("function_node id=" + node->identifier());
    if (node->args() != nullptr) {
        node->args()->accept(this);
    }
    if (node->body() != nullptr) {
        node->body()->accept(this);
    }
    close_tag("function_node");
}

void PrintVisitor::do_node(geq_node* node) {
    open_tag("geq_node");
    node->lhs()->accept(this);
    node->rhs()->accept(this);
    close_tag("geq_node");
}

void PrintVisitor::do_node(group_node* node) {
    open_tag("group_node");
    for (const auto& el : node->elements()) {
        el->accept(this);
    }
    close_tag("group_node");
}

void PrintVisitor::do_node(gt_node* node) {
    open_tag("gt_node");
    node->lhs()->accept(this);
    node->rhs()->accept(this);
    close_tag("gt_node");
}

void PrintVisitor::do_node(int_literal_node* node) {
    std::cout << std::string(_indent, ' ')
              << "<int_literal_node val=" << node->val() << " />\n";
}

void PrintVisitor::do_node(leq_node* node) {
    open_tag("leq_node");
    node->lhs()->accept(this);
    node->rhs()->accept(this);
    close_tag("leq_node");
}

void PrintVisitor::do_node(lt_node* node) {
    open_tag("lt_node");
    node->lhs()->accept(this);
    node->rhs()->accept(this);
    close_tag("lt_node");
}

void PrintVisitor::do_node(lvalue_node* node) {
    std::cout << std::string(_indent, ' ')
              << "<lvalue_node id=" << node->identifier() << " />\n";
}

void PrintVisitor::do_node(mod_node* node) {
    open_tag("mod_node");
    node->lhs()->accept(this);
    node->rhs()->accept(this);
    close_tag("mod_node");
}

void PrintVisitor::do_node(mul_node* node) {
    open_tag("mul_node");
    node->lhs()->accept(this);
    node->rhs()->accept(this);
    close_tag("mul_node");
}

void PrintVisitor::do_node(neq_node* node) {
    open_tag("neq_node");
    node->lhs()->accept(this);
    node->rhs()->accept(this);
    close_tag("neq_node");
}

void PrintVisitor::do_node(or_node* node) {
    open_tag("or_node");
    node->lhs()->accept(this);
    node->rhs()->accept(this);
    close_tag("or_node");
}

void PrintVisitor::do_node(return_node* node) {
    open_tag("return_node");
    node->expression()->accept(this);
    close_tag("return_node");
}

void PrintVisitor::do_node(sub_node* node) {
    open_tag("sub_node");
    node->lhs()->accept(this);
    node->rhs()->accept(this);
    close_tag("sub_node");
}

} // namespace yaya
