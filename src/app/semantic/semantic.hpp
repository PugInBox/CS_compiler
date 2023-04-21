#pragma once

#include "grammar/CsharpParser.h"
#include "grammar/CsharpVisitor.h"
#include "parser/symbol_table/symbol_table.hpp"
#include "parser/ast/ast.hpp"

#include <fstream>
#include <string>
#include <vector>
#include <utility>

class SemanticVisitor : public Visitor {
  using Table = std::vector<std::map<std::string, Properties>>;
  using FunctionProp = std::map<std::string, std::vector<std::pair<std::string, std::string>>>;
  using Indexer = std::map<std::string, std::size_t>;
  using Errors = std::vector<std::pair<std::string, std::string>>;
  using InnerTable = std::map<std::string, std::map<std::string, std::string>>;
  private:
    Errors errors;
    Table table;
    FunctionProp f_props;
    Indexer fname_indexer;
    InnerTable inner_table;
  public:
  std::size_t get_fname_index(std::string s) { return fname_indexer[s]; }
  SemanticVisitor(Table t, FunctionProp p, Indexer fi, InnerTable it) : table(t), f_props(p), fname_indexer(fi), inner_table(it) {}
  void append_error(std::pair<std::string, std::string> s) { errors.emplace_back(s); }
  Errors get_errors() { return errors; }
  int get_level(std::string str);
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
  void visit(ASTRead &node) {};
  std::string get_literal_type(std::string literal);
};

namespace cs_lang {
  void print_semantic_report(SemanticVisitor sv);
}