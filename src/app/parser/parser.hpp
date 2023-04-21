#pragma once

#include "symbol_table/symbol_table.hpp"
#include "grammar/CsharpLexer.h"
#include <string>
#include "ast/ast.hpp"
#include <vector>
#include "antlr4-runtime.h"

namespace cs_lang { 



struct Error {
  size_t m_line;
  size_t m_column;
  std::string m_message;
};

using Errors = std::vector<Error>;


struct ParseResult {
  static ParseResult program(ASTProgram* program) {
    ParseResult result;
    result.m_program = program;
    return result;
  }

  static ParseResult errors(Errors errors) {
    ParseResult result;
    result.m_errors = std::move(errors);
    return result;
  }

  ~ParseResult() { delete m_program; }

  ASTProgram* m_program = nullptr;
  Errors m_errors;
};


// void dump_ast(ASTProgram& program);
void dump_errors(Errors& errors);

        struct token {
        int x;
        int y;
        std::string token_name;
        std::string token_str;
        token(std::size_t loc_x, std::size_t loc_y, std::string tkn_name, std::string tkn) : x(loc_x), y(loc_y), token_name(tkn_name), token_str(tkn) {}
        std::size_t get_x() {return x;}
        std::size_t get_y() {return y;}
        std::string get_token_name() {return token_name;}
        std::string get_token_str() {return token_str;}
    };

        class tokens_array {
        std::vector<token> tokens;
    public:
        token operator[](int index) {return tokens[index];}
        void push_back(std::size_t loc_x, std::size_t loc_y, std::string token_name, std::string token_str)
        {
            tokens.push_back(token(loc_x, loc_y, token_name, token_str));
        }
    };
    tokens_array dump_tokens(std::string filepath, int print_key=1);
    void dump_ast(ASTProgram* program, std::string xml_file, bool print_key);
    void dump_table(VisitorTable table);
    ParseResult parse_test(std::string filepath);
   








} // namespace cs_lexer
