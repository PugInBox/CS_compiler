#pragma once

#include "grammar/CsharpParser.h"
#include "grammar/CsharpVisitor.h"

#include <fstream>
#include <string>
#include <vector>

class ASTProgram;
class ASTFunction;
class ASTVariable;
class ASTFuncCall;
class ASTScope;
class ASTArgs;
class ASTAssign;
class ASTReturn;
class ASTIf;
class ASTFor;
class ASTForOp;
class ASTForCond;
class ASTKw;
class ASTElse;
class ASTPrint;
class ASTRead;

class Visitor : public CsharpVisitor {
public:
  antlrcpp::Any
  visitExpressions(CsharpParser::ExpressionsContext *context) override;

  antlrcpp::Any visitProgram(CsharpParser::ProgramContext *context) override;
  antlrcpp::Any visitVar_def(CsharpParser::Var_defContext *context) override;
  antlrcpp::Any visitFunc_def(CsharpParser::Func_defContext *context) override;

  antlrcpp::Any visitAssign_statement(
      CsharpParser::Assign_statementContext *context) override;
  antlrcpp::Any
  visitStatement(CsharpParser::StatementContext *context) override;
  antlrcpp::Any visitScope(CsharpParser::ScopeContext *context) override;
  antlrcpp::Any
  visitFunc_call(CsharpParser::Func_callContext *context) override;
  antlrcpp::Any visitArgs(CsharpParser::ArgsContext *context) override;
  antlrcpp::Any visitLiteral(CsharpParser::LiteralContext *context) override;
  antlrcpp::Any visitReturn_statement(
      CsharpParser::Return_statementContext *context) override;
  antlrcpp::Any visitArg(CsharpParser::ArgContext *context) override;
  antlrcpp::Any
  visitIf_statement(CsharpParser::If_statementContext *context) override;
  antlrcpp::Any
  visitFor_statement(CsharpParser::For_statementContext *context) override;

  antlrcpp::Any
  visitFor_condition(CsharpParser::For_conditionContext *context) override;

  antlrcpp::Any
  visitFor_operation(CsharpParser::For_operationContext *context) override;
  antlrcpp::Any
  visitKw_statement(CsharpParser::Kw_statementContext *context) override;
  antlrcpp::Any visitPars(CsharpParser::ParsContext *context) override;
  antlrcpp::Any visitPrint_statement(CsharpParser::Print_statementContext *context) override;
  antlrcpp::Any visitElse_statement(CsharpParser::Else_statementContext *context) override;
  antlrcpp::Any visitRead_statement(CsharpParser::Read_statementContext *context) override;
  // antlrcpp::Any visitArgs(CsharpParser::ArgsContext *context) override;

  virtual void visit(ASTProgram &node) = 0;
  virtual void visit(ASTFunction &node) = 0;
  virtual void visit(ASTVariable &node) = 0;
  virtual void visit(ASTFuncCall &node) = 0;
  virtual void visit(ASTScope &node) = 0;
  virtual void visit(ASTArgs &node) = 0;
  virtual void visit(ASTAssign &node) = 0;
  virtual void visit(ASTReturn &node) = 0;
  virtual void visit(ASTIf &node) = 0;
  virtual void visit(ASTFor &node) = 0;
  virtual void visit(ASTForCond &node) = 0;
  virtual void visit(ASTForOp &node) = 0;
  virtual void visit(ASTKw &node) = 0;
  virtual void visit(ASTElse &node) = 0;
  virtual void visit(ASTPrint &node) = 0;
  virtual void visit(ASTRead &node) = 0;
};

class ASTNode {
protected:
  static inline std::size_t m_depth = 0;
  static inline bool dpsn = false;
  std::size_t line = 0;
  std::size_t char_pos = 0;
  bool x = false;
public:
  virtual ~ASTNode() = default;
  static void increase_depth() { m_depth += 2; }
  static void decrease_depth() {
    if (m_depth >= 2) {
      m_depth -= 2;
    }
  }
  void set_x(bool b) { b = x; }
  bool get_x() { return x; }
  void set_line(std::size_t l) { line = l; }
  std::size_t get_line() { return line; }
  void set_char_pos(std::size_t cp) { char_pos = cp; }
  std::size_t get_char_pos() { return char_pos; } 
  static void set_depth(std::size_t d) { m_depth = d; }
  static void set_dpsn(bool d) { dpsn = d; }
  static bool get_dpsn() { return dpsn; }
  static std::size_t get_depth() { return m_depth; }
  virtual void accept(Visitor &visitor) = 0;
};

class ASTArgs : public ASTNode {
private:
  std::string arg;
  bool is_lit = false;
public:
  ASTArgs() = default;
  bool is_literal() { return is_lit; }
  void set_literal(bool l) { is_lit = l; }
  std::string get_arg() { return arg; }
  void set_arg(std::string a) { arg = a; }
  void accept(Visitor &visitor) override;
  ~ASTArgs() = default;
};

class ASTScope : public ASTNode {
private:
  std::vector<ASTNode *> m_statements;
  std::string scope_name;
  std::map<std::string, std::string> variables;

public:
  ASTScope() = default;
  void append_var(std::string variable, std::string type) { variables[variable] = type; }
  std::map<std::string, std::string> get_var_map() { return variables; }
  void append_statement(ASTNode *node);
  ASTNode *get_statement(std::size_t i);
  // NOTE:
  std::string get_scope_name() { return scope_name; }
  void set_scope_name(std::string sn) { scope_name = sn; }
  std::vector<ASTNode *> get_statements();
  void remove_statement(ASTNode* r) { 
    for(auto it = m_statements.begin(); it!=m_statements.end(); it++)
    {
      if(*it == r)
      {
        m_statements.erase(it);
        return;
      }
    }
  }

  void accept(Visitor &visitor) override;
  ~ASTScope() {
    for (std::size_t i = 0; i < m_statements.size(); i++) {
      delete m_statements[i];
    }
  }
};

class ASTVariable : public ASTNode {
private:
    // m_var_name - NAME if var isn't literal, and VALUE if var is literal
    std::string fragment_data = "";
    std::string m_var_name = "";
    bool is_lit = false;
    std::string m_var_type = "";
    std::string expr_type = "";
    std::string ctx_type = "";
    std::string func_name = "";

public:
    ASTVariable() = default;
    std::string get_frag() { return fragment_data; }
    void set_frag(std::string f) { fragment_data = f;}
    std::string get_ctx_type() { return ctx_type; }
    void set_ctx_type(std::string s) { ctx_type = s; }
    std::string get_expr_type() { return expr_type; }
    void set_expr_type(std::string s) { expr_type = s; }
    std::string get_var_name() { return m_var_name; }
    void set_var_name(std::string s) { m_var_name = s; }
    std::string get_var_type() { return m_var_type; }
    void set_var_type(std::string s) { m_var_type = s; }
    std::string get_func_name() { return func_name; }
    void set_func_name(std::string s) { func_name = s; }
    // std::string& var_name();
    // std::string& var_type();
    void set_literal(bool l) { is_lit = l;}
    bool is_literal() {return is_lit;}
    void accept(Visitor& visitor) override;
    ~ASTVariable() = default;
};

class ASTFuncCall : public ASTNode {
  using Args = std::vector<std::pair<std::string, std::string>>;
private:
  std::string m_func_name;
  std::vector<ASTArgs *> args;
  Args vector_args;
  // ASTFunction* func_def;
  // std::vector<ASTVariable>

public:
  ASTFuncCall() = default;

  std::string &func_name();
  std::vector<ASTArgs *> get_args() { return args; }
  ASTArgs *get_arg(std::size_t i) { return args[i]; }
  void set_args(ASTArgs *a, size_t i) { args[i] = a; }
  void append_arg(ASTArgs *a) { args.push_back(a); }
  // void set_func_def(ASTFunction* fd) { func_def = fd; }
  // ASTFunction get_func_def() { return *func_def; }
  void append_args_to_vector(std::string name, std::string type) { vector_args.push_back(std::make_pair(name, type)); }
  Args get_args_from_vector() { return vector_args; }
  void accept(Visitor &visitor) override;
  ~ASTFuncCall() {
    for (std::size_t i = 0; i < args.size(); i++) {
      delete args[i];
    }
  }
};

class ASTAssign : public ASTNode {
private:
  ASTVariable *lvalue = nullptr;
  ASTVariable *rvalue1 = nullptr;
  ASTVariable *rvalue2 = nullptr;
  ASTFuncCall *funccall = nullptr;
  std::string oper;


public:
  ASTAssign() = default;
  void set_lvalue(ASTVariable *a) { lvalue = a; }
  void set_rvalue1(ASTVariable *a) { rvalue1 = a; }
  void set_rvalue2(ASTVariable *a) { rvalue2 = a; }
  void set_funccall(ASTFuncCall *f) { funccall = f; }
  void set_oper(std::string o) { oper = o; }

  ASTVariable *get_lvalue() { return lvalue; }
  ASTVariable *get_rvalue1() { return rvalue1; }
  ASTVariable *get_rvalue2() { return rvalue2; }
  ASTFuncCall *get_funccall() { return funccall; }
  std::string get_oper() { return oper; }

  void accept(Visitor &visitor) override;

  ~ASTAssign() {
    delete lvalue;
    delete rvalue1;
    delete rvalue2;
    delete funccall;
  }
};

class ASTProgram : public ASTNode {
private:
  std::vector<ASTNode *> m_children;

public:
  ASTProgram() = default;

  void append_child(ASTNode *node);
  const ASTNode *get_child(std::size_t i) const;

  std::vector<ASTNode *> get_children();
  void accept(Visitor &visitor) override;
  void remove_child(ASTNode* r) { 
    for(auto it = m_children.begin(); it!=m_children.end(); it++)
    {
      if(*it == r)
      {
        m_children.erase(it);
        return;
      }
    }
  }
  ~ASTProgram() {
    for (std::size_t i = 0; i < m_children.size(); i++) {
      delete m_children[i];
    }
  }
};

class ASTRead : public ASTNode {
  std::string type = "";
  std::string name = "";

public:
  ASTRead()=default;
  std::string get_type() { return type; }
  std::string get_name() { return name; }
  void set_type(std::string t) { type = t; }
  void set_name(std::string n) { name = n; }
  void accept(Visitor &visitor) override;
  ~ASTRead()=default;
};


// i < 10
class ASTForCond : public ASTNode {
  std::string first = "";
  std::string second = "";
  std::string op = "";
  bool is_lit = false;

public:
  ASTForCond() = default;
  void set_first(std::string f) { first = f; }
  void set_second(std::string s) { second = s; }
  void set_op(std::string o) { op = o; }
  void set_literal(bool l) { is_lit = l; }

  std::string get_first() { return first; }
  std::string get_second() { return second; }
  std::string get_op() { return op; }
  bool is_literal() { return is_lit; }
  void accept(Visitor &visitor) override;
  ~ASTForCond() = default;
};
// i++
class ASTForOp : public ASTNode {
  ASTAssign *for_op_assign = nullptr;
  std::string id = "";
  std::string unary_op = "";

public:
  ASTForOp() = default;
  void set_assign(ASTAssign *a) { for_op_assign = a; }
  void set_id(std::string i) { id = i; }
  void set_unary_op(std::string uo) { unary_op = uo; }

  ASTAssign *get_assign() { return for_op_assign; }
  std::string get_id() { return id; }
  std::string get_unary_op() { return unary_op; }
  void accept(Visitor &visitor) override;

  ~ASTForOp() { delete for_op_assign; }
};

// int i = 0; i < 10; i++
class ASTFor : public ASTNode {
  ASTAssign *assign;
  ASTForCond *condition;
  ASTForOp *op;
  ASTScope *scope;

public:
  ASTFor() = default;
  void set_assing(ASTAssign *a) { assign = a; }
  void set_cond(ASTForCond *c) { condition = c; }
  void set_op(ASTForOp *o) { op = o; }
  void set_scope(ASTScope *s) { scope = s; }

  ASTAssign *get_assing() { return assign; }
  ASTForCond *get_cond() { return condition; }
  ASTForOp *get_op() { return op; }
  ASTScope *get_scope() { return scope; }
  void accept(Visitor &visitor) override;
  ~ASTFor() {
    delete assign;
    delete condition;
    delete op;
    delete scope;
  }
};

// class ASTPars : public ASTNode {
//     std::string type = "";
//     std::string value = "";
//     bool is_lit = false;
// public:
//     ASTPars() = default;
//     bool is_literal() { return is_lit; }
//     std::string get_value() { return value; }
//     void set_value(std::string rv) { value = rv; }
//     std::string get_type() { return type; }
//     void set_type(std::string t) { type = t; }
//     void set_literal(bool l) { is_lit = l; }
//     ~ASTPars() = default;
    
//     void accept(Visitor& visitor) override;
// };

class ASTReturn : public ASTNode {
private:
  std::string return_value = "";
  std::string type = "";
  bool is_lit = false;

public:
  ASTReturn() = default;
  bool is_literal() { return is_lit; }
  std::string get_return_value() { return return_value; }
  void set_return_value(std::string rv) { return_value = rv; }
  std::string get_return_type() { return type; }
  void set_return_type(std::string t) { type = t; }
  void set_literal(bool l) { is_lit = l; }
  ~ASTReturn() = default;

  void accept(Visitor &visitor) override;
};

class ASTFunction : public ASTNode {
  using Pars = std::vector<std::pair<std::string, std::string>>;
private:
    /* Node info */
    std::string m_func_name;
    std::string m_return_type;
    Pars pars;
    std::vector<ASTVariable*> params;
    ASTReturn* m_return = nullptr;
    ASTScope* m_scope = nullptr;
public:
    ASTFunction() = default;
    std::string& return_type();
    std::string& func_name();
    void set_scope(ASTScope* sc)
    {
        m_scope = sc;
    }
    ASTScope* get_scope()
    {
        return m_scope;
    }
    void set_return(ASTReturn* r)
    {
        m_return = r;
    }
    ASTReturn* get_return()
    {
        return m_return;
    }

    void accept(Visitor& visitor) override;
    ~ASTFunction()
    {
        delete m_scope;
        delete m_return;
    }

    std::vector<ASTVariable*> get_params() { return params; }

    void append_param(ASTVariable* p) { params.push_back(p); }
    void append_param_to_vector(std::string name, std::string type) { pars.push_back(std::make_pair(name, type)); }
    Pars get_param_from_vector() { return pars; }
};

class ASTElse : public ASTNode {
  ASTScope *m_scope = nullptr;

  public:
    ASTElse() = default;
    void set_scope(ASTScope *sc) { m_scope = sc; }
    ASTScope *get_scope() { return m_scope; }

    void accept(Visitor &visitor) override;
    ~ASTElse() {
      delete m_scope;
    }
};

class ASTIf : public ASTNode {
  std::string first = "";
  std::string first_type = "";
  std::string second = "";
  std::string second_type = "";
  bool is_lit = false;
  std::string op = "";
  ASTScope *m_scope = nullptr;
  ASTElse* else_statement = nullptr;

public:
  ASTIf() = default;
  void set_else(ASTElse* es) { else_statement = es; }
  void set_first(std::string s) { first = s; }
  void set_first_type(std::string s) { first_type = s; }
  void set_second(std::string s) { second = s; }
  void set_second_type(std::string s) { second_type = s; }
  void set_op(std::string s) { op = s; }

  void set_literal(bool l) { is_lit = l; }
  bool is_literal() { return is_lit; }

  ASTElse* get_else() { return else_statement; }
  std::string get_first() { return first; }
  std::string get_first_type() { return first_type; }
  std::string get_second() { return second; }
  std::string get_second_type() { return second_type; }
  std::string get_op() { return op; }

  void set_scope(ASTScope *sc) { m_scope = sc; }
  ASTScope *get_scope() { return m_scope; }

  void accept(Visitor &visitor) override;

  ~ASTIf() { delete m_scope; }
};

class ASTKw : public ASTNode {
  std::string kw = "";

public:
  ASTKw() = default;
  std::string get_kw() { return kw; }
  void set_kw(std::string k) { kw = k; }
  void accept(Visitor &visitor) { visitor.visit(*this); };
  ~ASTKw() = default;
};

class ASTPrint : public ASTNode {
  std::string type = "";
  std::string name = "";

public:
  ASTPrint()=default;
  std::string get_type() { return type; }
  std::string get_name() { return name; }
  void set_type(std::string t) { type = t; }
  void set_name(std::string n) { name = n; }
  void accept(Visitor &visitor) override;
  ~ASTPrint()=default;
};

/* Visitor Implementation */
class VisitorInitialiser : public Visitor {
private:
  antlrcpp::Any m_context;

public:
  VisitorInitialiser(antlrcpp::Any context);

  void visit(ASTProgram &node) override;
  void visit(ASTFunction &node) override;
  void visit(ASTVariable &node) override;
  void visit(ASTFuncCall &node) override;
  void visit(ASTScope &node) override;
  void visit(ASTArgs &node) override;
  void visit(ASTAssign &node) override;
  void visit(ASTReturn &node) override;
  void visit(ASTIf &node) override;
  void visit(ASTFor &node) override;
  void visit(ASTForCond &node) override;
  void visit(ASTForOp &node) override;
  void visit(ASTKw &node) override;
  void visit(ASTElse &node) override;
  void visit(ASTPrint &node) override;
  void visit(ASTRead &node) override;
};

class VisitorTraverse : public Visitor {
private:
  std::ofstream &stream;

public:
  VisitorTraverse(std::ofstream &os);

  void set_indent(std::size_t spaces) {
    for (std::size_t i = 0; i < spaces; i++) {
      stream << " ";
    }
  }
  void visit(ASTProgram &node) override;
  void visit(ASTFunction &node) override;
  void visit(ASTVariable &node) override;
  void visit(ASTFuncCall &node) override;
  void visit(ASTScope &node) override;
  void visit(ASTArgs &node) override;
  void visit(ASTAssign &node) override;
  void visit(ASTReturn &node) override;
  void visit(ASTIf &node) override;
  void visit(ASTFor &node) override;
  void visit(ASTForCond &node) override;
  void visit(ASTForOp &node) override;
  void visit(ASTKw &node) override;
  void visit(ASTElse &node) override;
  void visit(ASTPrint &node) override;
  void visit(ASTRead &node) override;
};
