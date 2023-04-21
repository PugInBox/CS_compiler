#pragma once

#include "grammar/CsharpParser.h"
#include "grammar/CsharpVisitor.h"
#include "parser/symbol_table/symbol_table.hpp"
#include "parser/ast/ast.hpp"

#include <fstream>
#include <string>
#include <vector>
#include <utility>

class CodeGen : public Visitor {
	std::ofstream &stream;
	std::string filepath;
	std::string outfile;

public:
	CodeGen(std::ofstream &os, std::string f, std::string of) : stream(os), filepath(f), outfile(of) {

	}
  void visit(ASTProgram &node) override;
  void visit(ASTFunction &node) override;
  void visit(ASTVariable &node) {}
  void visit(ASTFuncCall &node) override;
  void visit(ASTScope &node) override;
  void visit(ASTArgs &node) {}
  void visit(ASTAssign &node) override;
  void visit(ASTReturn &node) {}
  void visit(ASTIf &node) override;
  void visit(ASTFor &node) override;
  void visit(ASTForCond &node) {}
  void visit(ASTForOp &node) {}
  void visit(ASTKw &node) {}
  void visit(ASTElse &node) override;
  void visit(ASTPrint &node) override;
  void visit(ASTRead &node) override;
};