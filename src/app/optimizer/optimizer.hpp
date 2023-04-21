#pragma once

#include "grammar/CsharpParser.h"
#include "grammar/CsharpVisitor.h"
#include "parser/symbol_table/symbol_table.hpp"
#include "parser/ast/ast.hpp"

#include <fstream>
#include <string>
#include <vector>
#include <utility>

class OptimizerVisitor : public Visitor {
  using Table = std::vector<std::map<std::string, Properties>>;
  using Indexer = std::map<std::string, std::size_t>;
  private:
	Table table;
	std::map<std::string, bool> unused_vars;
  std::vector<std::pair<std::string, std::string>> warnings;
  Indexer fname_indexer;
  bool wall;
  bool opt;
  public:
  OptimizerVisitor(Table t, Indexer fi, bool w = false, bool o = false) : table(t), fname_indexer(fi), wall(w), opt(o) {}
  std::size_t get_fname_index(std::string s) { return fname_indexer[s]; }
	void append_warning(std::pair<std::string, std::string> w) { warnings.push_back(w); }
	void append_unused_var(std::string var) { unused_vars[var] = true; }
  void set_used_var(std::string var) { unused_vars[var] = false; }
  void dump_warnings(std::string func_name) { 
    if(warnings.size() > 0)
    {
      std::cout << "In " << func_name << "\n";
      for(auto& p : warnings)
      {
        std::cout << "\tWARNING: " << p.first << " " << p.second << "\n";
      }
    }
  }
  void print_unused_var() {
    std::cout << "UNUSED VARS:\n";
    for(auto const& item : unused_vars)
    {
      if(item.second)
        std::cout << item.first << " ";
    }
    std::cout << "\n";
  }
  void clear_unused_vars() { unused_vars.clear(); }
	std::map<std::string, bool> get_unused_vars() { return unused_vars; }
  bool is_del_func(std::string func_name);
  void visit(ASTProgram &node) override;
  void visit(ASTFunction &node) override;
  void visit(ASTVariable &node) {}
  void visit(ASTFuncCall &node) override;
  void visit(ASTScope &node) override;
  void visit(ASTArgs &node) {}
  void visit(ASTAssign &node) override;
  void visit(ASTReturn &node) override;
  void visit(ASTIf &node) override;
  void visit(ASTFor &node) override;
  void visit(ASTForCond &node) override;
  void visit(ASTForOp &node) {}
  void visit(ASTKw &node) {}
  void visit(ASTPrint &node) override;
  void visit(ASTRead &node) override;
  void visit(ASTElse &node) override;
};

class PtrVisitor : public Visitor {
  std::map<std::string, bool> unused_vars;
  public:
  PtrVisitor(std::map<std::string, bool> uv) : unused_vars(uv) {}
  void visit(ASTProgram &node) override;
  void visit(ASTFunction &node) override;
  void visit(ASTVariable &node) {}
  void visit(ASTFuncCall &node) override;
  void visit(ASTScope &node) override;
  void visit(ASTArgs &node) {}
  void visit(ASTAssign &node) override;
  void visit(ASTReturn &node) override;
  void visit(ASTIf &node) override;
  void visit(ASTFor &node) override;
  void visit(ASTForCond &node) override;
  void visit(ASTForOp &node) {}
  void visit(ASTKw &node) {}
  void visit(ASTPrint &node) {}
  void visit(ASTRead &node) {}
  void visit(ASTElse &node) override;
};

class DeleteVisitor : public Visitor {
  std::map<std::string, bool> unused_vars;
  public:
  DeleteVisitor(std::map<std::string, bool> uv) :unused_vars(uv) {}
  void visit(ASTProgram &node) override;
  void visit(ASTFunction &node) override;
  void visit(ASTVariable &node) {}
  void visit(ASTFuncCall &node) override;
  void visit(ASTScope &node) override;
  void visit(ASTArgs &node) {}
  void visit(ASTAssign &node) override;
  void visit(ASTReturn &node) override;
  void visit(ASTIf &node) override;
  void visit(ASTFor &node) override;
  void visit(ASTForCond &node) override;
  void visit(ASTForOp &node) {}
  void visit(ASTKw &node) {}
  void visit(ASTPrint &node) {}
  void visit(ASTRead &node) {};
  void visit(ASTElse &node) override;
};