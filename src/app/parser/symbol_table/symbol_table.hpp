#pragma once

#include "grammar/CsharpParser.h"
#include "grammar/CsharpVisitor.h"

#include "parser/ast/ast.hpp"

#include <fstream>
#include <string>
#include <vector>
#include <iomanip>


struct Properties {
    std::string fragment_type; // VARIABLE, FUNCTION
    std::size_t level=0;
    std::string type="";
};

class VisitorTable : public Visitor {
    using Table = std::vector<std::map<std::string, Properties>>;
    using InnerTable = std::map<std::string, std::map<std::string, std::string>>;
    using FunctionProp = std::map<std::string, std::vector<std::pair<std::string, std::string>>>;
    using Indexer = std::map<std::string, std::size_t>;
private:
  Table table;
  InnerTable inner_table;
  FunctionProp f_props;
	static std::size_t table_level;
  static std::size_t vector_num;
  Indexer fname_indexer;
public:
  VisitorTable() = default;

	Table& get_table() { return table; }
  FunctionProp& get_fprops() { return f_props; }
  void set_fprop(std::string f_name, std::pair<std::string, std::string> p) {
    if(p.first == "" || p.second == "")
    {
      f_props[f_name] = {};
    }
    else {
      f_props[f_name].push_back(p); 
    } 
  }
  Indexer get_indexer() { return fname_indexer; }
  std::size_t get_fname_index(std::string s) { return fname_indexer[s]; }
  InnerTable get_inner_table() { return inner_table; }
  void set_fname_index(std::string s) { fname_indexer[s] = vector_num++; }
	//void set_symbol(std::string s, Properties p)  { table[s] = p; }
	static void incr_level() { table_level++; }
	static void decr_level() { table_level--; }
  std::string get_literal_type(std::string literal, std::string func_name)
  {
    if(table[get_fname_index(func_name)][literal].type == "NUMBER")
    {
      return std::string("int");
    }
    if(table[get_fname_index(func_name)][literal].type == "FLOAT_NUMBER")
    {
      return std::string("float");
    }
    if(table[get_fname_index(func_name)][literal].type == "CHAR")
    {
      return std::string("char");
    }
    if(table[get_fname_index(func_name)][literal].type == "STRING" || table[get_fname_index(func_name)][literal].type == "TEXT")
    {
      return std::string("string");
    }
    return std::string("unknown");
  }
  void print_inner_table(std::string ind)
  {
    for(auto const& item : inner_table[ind])
    {
      std::cout << "\t";
      std::cout << std::setw(30) << item.first << std::setw(20) << item.second << "\n";
    }
    std::cout << "------------------------------------\n";
  }
  void print_table() { 
    for(std::size_t i = 0; i < table.size(); i++)
    {
      if(i == 0)
      {
        std::cout << "Scope: Global\n";
        
      }
      else {
        for(auto it_func = f_props.begin(); it_func != f_props.end(); it_func++)
          {
            if(get_fname_index(it_func->first) == i)
            {
              std::cout << "Scope: " << it_func->first << "\n";
              break;
            }
          }
      }
      
      for(auto it_map = table[i].begin(); it_map != table[i].end(); it_map++)
      {
        std::cout << std::setw(30) << it_map->first << std::setw(20) << it_map->second.type << std::setw(3) << it_map->second.level << std::setw(20) << it_map->second.fragment_type << "\n";
      }
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
  void visit(ASTElse &node) override;
  void visit(ASTPrint &node) override;
  void visit(ASTRead& node) override;
  void visit(ASTKw &node) {};
};
