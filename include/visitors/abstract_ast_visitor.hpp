#pragma once

namespace yaya {

class add_node;
class and_node;
class assignment_node;
class basic_node;
class call_node;
class declaration_node;
class div_node;
class eq_node;
class function_node;
class geq_node;
class group_node;
class gt_node;
class int_literal_node;
class leq_node;
class lt_node;
class lvalue_node;
class mod_node;
class mul_node;
class neq_node;
class or_node;
class return_node;
class sub_node;

class AbstractASTVisitor {
  public:
    AbstractASTVisitor() = default;

    virtual void do_node(basic_node& node) = 0;

    virtual void do_node(add_node* node) = 0;
    virtual void do_node(and_node* node) = 0;
    virtual void do_node(assignment_node* node) = 0;
    virtual void do_node(call_node* node) = 0;
    virtual void do_node(declaration_node* node) = 0;
    virtual void do_node(div_node* node) = 0;
    virtual void do_node(eq_node* node) = 0;
    virtual void do_node(function_node* node) = 0;
    virtual void do_node(geq_node* node) = 0;
    virtual void do_node(group_node* node) = 0;
    virtual void do_node(gt_node* node) = 0;
    virtual void do_node(int_literal_node* node) = 0;
    virtual void do_node(leq_node* node) = 0;
    virtual void do_node(lt_node* node) = 0;
    virtual void do_node(lvalue_node* node) = 0;
    virtual void do_node(mod_node* node) = 0;
    virtual void do_node(mul_node* node) = 0;
    virtual void do_node(neq_node* node) = 0;
    virtual void do_node(or_node* node) = 0;
    virtual void do_node(return_node* node) = 0;
    virtual void do_node(sub_node* node) = 0;
};

} // namespace yaya
