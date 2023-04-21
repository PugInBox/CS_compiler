#include "grammar/CsharpVisitor.h"
#include "semantic.hpp"

#include <any>
#include <fstream>
#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>

// ASTProgram ASTFunction
// ASTVariable ASTFuncCall
// ASTScope ASTArgs
// ASTAssign ASTReturn
// ASTIf ASTFor
// ASTForCond ASTForOp
// ASTKw

static std::string current_function_scope = "Global";
static std::string current_scope = "";
static std::size_t inner_counter = 0;

namespace cs_lang {
  void print_semantic_report(SemanticVisitor sv) 
  {
      if(!sv.get_errors().empty())
      {
        for(auto& i : sv.get_errors())
        {
					std::cout << i.first << ": " << i.second << "\n";
        }
      } else {
          std::cout << "Semantic errors not found\n";
      }
  }
}

std::string SemanticVisitor::get_literal_type(std::string literal)
{
    if(table[get_fname_index(current_function_scope)][literal].type == "NUMBER")
    {
        return std::string("int");
    }
    if(table[get_fname_index(current_function_scope)][literal].type == "FLOAT_NUMBER")
    {
        return std::string("float");
    }
    if(table[get_fname_index(current_function_scope)][literal].type == "CHAR")
    {
        return std::string("char");
    }
    if(table[get_fname_index(current_function_scope)][literal].type == "STRING" || table[get_fname_index(current_function_scope)][literal].type == "TEXT")
    {
        return std::string("string");
    }
    return std::string("unknown");
}


void SemanticVisitor::visit(ASTProgram& node)
{
    for(auto& child : node.get_children())
    {
        child->accept(*this);
    }
}

int SemanticVisitor::get_level(std::string str)
{
    int i = 0;
    for(; i < 100; i++)
    {
        if(table[get_fname_index(current_function_scope)].contains(str+std::to_string(i)))
        {
            break;
        }
    }
    return i;
}

// int main(int i) {}
void SemanticVisitor::visit(ASTFunction& node)
{
    current_function_scope = node.func_name();
    if(node.get_return() == nullptr && node.return_type() != "void")
    {
        append_error(std::make_pair("Function definition ERROR", "No return statement in function returning non-void, in function \'" + node.func_name() + "\', in line " + std::to_string(node.get_line()) +"\n"));
    }
    if(node.get_return() != nullptr)
    {
        int lvl = get_level(node.get_return()->get_return_value()+"_return_");
        std::string ret_type = table[get_fname_index(current_function_scope)][node.get_return()->get_return_value()+"_return_"+std::to_string(lvl)].type;
        std::transform(ret_type.begin(), ret_type.end(), ret_type.begin(),
            [](unsigned char c){ return std::tolower(c); });
        std::string node_ret_type = node.return_type();
        if(node.get_return()->is_literal())
        {
            if(ret_type == "NUMBER" || ret_type == "number")
            {
                ret_type = "int";
            } else if(ret_type == "FLOAT_NUMBER" || ret_type == "float_number")
            {
                ret_type = "float";
            } else if(ret_type == "CHAR")
            {
                ret_type = "char";
            } else if(ret_type == "STRING")
            {
                ret_type = "string";
            }
        }
        if(ret_type != node_ret_type)
        {
            errors.emplace_back(std::make_pair("Function-Definition ERROR", "Cannot convert type \'" + ret_type + "\' to \'" + node_ret_type + "\' in line "+ std::to_string(node.get_line())));
        }
    }


    node.get_scope()->accept(*this);
}

void SemanticVisitor::visit(ASTVariable& node) {}
void SemanticVisitor::visit(ASTFuncCall& node) {
    if(!f_props.contains(node.func_name()))
    {
        try {
            throw std::runtime_error("ERROR: Undefined function \'" + node.func_name() + "\'");
        } catch(std::runtime_error& e)
        {
            std::cerr << e.what() << "\n";
            exit(EXIT_FAILURE);
        }
    }
    int i = 0;
    if(node.get_args_from_vector().size() != f_props[node.func_name()].size())
    {
        try {
            throw std::runtime_error("ERROR: Incorrect number of arguments when calling a function \'" + node.func_name() + "\' in line " + std::to_string(node.get_line()));
        } catch(std::runtime_error& e)
        {
            std::cerr << e.what() << "\n";
            exit(EXIT_FAILURE);
        }
        
    }
    for(auto& arg : node.get_args_from_vector())
    {
        
        if(arg.second != f_props[node.func_name()][i].second)
        {
            errors.emplace_back(std::make_pair("Function-Call ERROR", "Cannot convert type \'" + arg.second + "\' to \'" + f_props[node.func_name()][i].second + "\' in line "+ std::to_string(node.get_line())));
        }
        i++;
    }
}
void SemanticVisitor::visit(ASTScope& node) {

    inner_counter++;
    current_scope = "scope";
    std::size_t fix_in_id = inner_counter;
    std::size_t amount_in_scopes = 0;
    for(auto& child : node.get_statements())
    {
        child->accept(*this);
        if(inner_counter != fix_in_id)
        {
            inner_counter = fix_in_id;
            amount_in_scopes++;
        }
        if(current_scope == "")
        {
            current_scope = "scope";
        }
    }
    current_scope = "";
    inner_counter += amount_in_scopes;
}

void SemanticVisitor::visit(ASTElse &node) 
{
    node.get_scope()->accept(*this);
    
}

std::string get_type_by_lit_type(std::string lit_type)
{
    if(lit_type.find("FLOAT") != std::string::npos || lit_type == "float" || lit_type.find("FLOAT_NUMBER") != std::string::npos)
        return std::string("float");
    if(lit_type.find("NUMBER") != std::string::npos || lit_type == "int")
        return std::string("int");
    if(lit_type.find("CHAR") != std::string::npos || lit_type == "char")
        return std::string("char");
    if(lit_type.find("STRING") != std::string::npos || lit_type == "string" || lit_type.find("TEXT") != std::string::npos)
        return std::string("string");
    return std::string("unknwn");
}

void SemanticVisitor::visit(ASTArgs& node) {}
void SemanticVisitor::visit(ASTAssign& node) {
    if(current_scope == "")
    {
        bool is_def = node.get_lvalue()->get_var_type() == ""? false : true;
        std::string lhs_type = "";
        /*if(inner_table[std::to_string(inner_counter)].contains(node.get_lvalue()->get_var_name()))
        {
            lhs_type = inner_table[std::to_string(inner_counter)][node.get_lvalue()->get_var_name()];
        }
        else */if(table[get_fname_index(current_function_scope)].contains(node.get_lvalue()->get_var_name()))
        {
            lhs_type = table[get_fname_index(current_function_scope)][node.get_lvalue()->get_var_name()].type;
        } else if(table[0].contains(node.get_lvalue()->get_var_name()))
        {
            lhs_type = table[0][node.get_lvalue()->get_var_name()].type;
        } 
        if(node.get_rvalue1() != nullptr)
        {
            std::string type_r1 = "";
            std::string type_r2 = "";
            if(node.get_rvalue1()->is_literal())
            {
                type_r1 = get_type_by_lit_type(node.get_rvalue1()->get_var_type());
            }
            else
            {
                /*if(inner_table[std::to_string(inner_counter)].contains(node.get_rvalue1()->get_var_name()))
                {
                    type_r1 = inner_table[std::to_string(inner_counter)][node.get_rvalue1()->get_var_name()];
                } else*/ if(table[get_fname_index(current_function_scope)].contains(node.get_rvalue1()->get_var_name()))
                {
                    type_r1 = table[get_fname_index(current_function_scope)][node.get_rvalue1()->get_var_name()].type;
                } else if(table[0].contains(node.get_rvalue1()->get_var_name()))
                {
                    type_r1 = table[0][node.get_rvalue1()->get_var_name()].type;
                }
                if(is_def && node.get_rvalue1()->get_var_name() == node.get_lvalue()->get_var_name())
                {
                    errors.emplace_back(std::make_pair("Assign ERROR", "Use of unassigned local variable \'" + node.get_lvalue()->get_var_name() + "\' in line "+ std::to_string(node.get_line())));
                }
            }
            if(lhs_type == type_r1)
            {
                if(node.get_rvalue2() != nullptr)
                {
                    if(node.get_rvalue2()->is_literal())
                    {
                        type_r2 = get_literal_type(node.get_rvalue2()->get_var_name());
                    }
                    else
                    {
                        type_r2 = table[get_fname_index(current_function_scope)][node.get_rvalue2()->get_var_name()].type;
                        //CHECK: 'string str = "10" + str;'
                        if(is_def && node.get_rvalue2()->get_var_name() == node.get_lvalue()->get_var_name())
                        {
                            errors.emplace_back(std::make_pair("Assign ERROR", "Use of unassigned local variable \'" + node.get_lvalue()->get_var_name() + "in line "+ std::to_string(node.get_line()) + "\'"));
                        }
                    }
                    if(lhs_type != type_r2)
                    {
                        errors.emplace_back(std::make_pair("Assign ERROR1", "Cannot convert type \'" + lhs_type + "\' to \'" + type_r2 + "\' in line "+ std::to_string(node.get_lvalue()->get_line())));
                    }
                }
            } else {
                errors.emplace_back(std::make_pair("Assign ERROR2", "Cannot convert type \'" + lhs_type + "\' to \'" + type_r1 + "\' in line "+ std::to_string(node.get_lvalue()->get_line())));
            }
        }
    } else {
        std::string l_type = "";
        std::string r1_type = "";
        std::string r2_type = "";
        if(node.get_lvalue() != nullptr)
        {
            if(node.get_lvalue()->get_var_type() != "")
            {
                l_type = node.get_lvalue()->get_var_type();
            } else {
                if(!inner_table[std::to_string(inner_counter)].contains(node.get_lvalue()->get_var_name()))
                {
                    if(inner_table[std::to_string(inner_counter+1)].contains(node.get_lvalue()->get_var_name()))
                    {
                        l_type = inner_table[std::to_string(inner_counter+1)][node.get_lvalue()->get_var_name()];
                    }

                    if(table[get_fname_index(current_function_scope)].contains(node.get_lvalue()->get_var_name()))
                    {
                        l_type = table[get_fname_index(current_function_scope)][node.get_lvalue()->get_var_name()].type;
                    } else if(table[0].contains(node.get_lvalue()->get_var_name()))
                    {
                        
                        l_type = table[0][node.get_lvalue()->get_var_name()].type;
                        
                    } else {
                        std::cerr << "ERROR: Undefined variable " << node.get_lvalue()->get_var_name() << " in line " << node.get_lvalue()->get_line() << "\n";
                        throw;
                    }
                } else {
                    
                    l_type = inner_table[std::to_string(inner_counter)][node.get_lvalue()->get_var_name()];
                }
                
            }
        }
        
        if(node.get_rvalue1() != nullptr)
        {
            
            if(node.get_rvalue1()->is_literal())
            {
                r1_type = get_type_by_lit_type(node.get_rvalue1()->get_var_type());
            } else {
                r1_type = node.get_rvalue1()->get_var_type();
                if(r1_type == "")
                {
                    if(inner_table[std::to_string(inner_counter)].contains(node.get_rvalue1()->get_var_name()))
                    {
                        r1_type = inner_table[std::to_string(inner_counter)][node.get_rvalue1()->get_var_name()];
                    }
                    else if(inner_table[std::to_string(inner_counter+1)].contains(node.get_rvalue1()->get_var_name()))
                    {
                        r1_type = inner_table[std::to_string(inner_counter+1)][node.get_rvalue1()->get_var_name()];
                    }
                    else if(table[get_fname_index(current_function_scope)].contains(node.get_rvalue1()->get_var_name()))
                    {
                        r1_type = table[get_fname_index(current_function_scope)][node.get_rvalue1()->get_var_name()].type;
                    } else if(table[0].contains(node.get_rvalue1()->get_var_name()))
                    {
                        r1_type = table[0][node.get_rvalue1()->get_var_name()].type;
                    }
                }
            }
            if(l_type != r1_type) {
                append_error(std::make_pair("Assign ERROR3", "Cannot convert type \'" + l_type + "\' to \'" + r1_type + "\' in line " + std::to_string(node.get_rvalue1()->get_line())));
            }
        }
        if(node.get_rvalue2() != nullptr)
        {
            if(node.get_rvalue2()->is_literal())
            {
                
                r2_type = get_type_by_lit_type(node.get_rvalue2()->get_var_type());
            } else {
                if(node.get_rvalue2()->get_var_type() != "")
                {
                    r2_type = node.get_rvalue2()->get_var_type();
                }
                else if(inner_table[std::to_string(inner_counter)].contains(node.get_rvalue2()->get_var_name()))
                {
                    r2_type = inner_table[std::to_string(inner_counter)][node.get_rvalue2()->get_var_name()];
                }

                else if(inner_table[std::to_string(inner_counter+1)].contains(node.get_rvalue2()->get_var_name()))
                {
                    r2_type = inner_table[std::to_string(inner_counter+1)][node.get_rvalue2()->get_var_name()];
                }

                else if(table[get_fname_index(current_function_scope)].contains(node.get_rvalue2()->get_var_name()))
                {
                    r2_type = table[get_fname_index(current_function_scope)][node.get_rvalue2()->get_var_name()].type;
                } else if(table[0].contains(node.get_rvalue2()->get_var_name()))
                {
                    r2_type = table[0][node.get_rvalue2()->get_var_name()].type;
                }
            }
            if(l_type != r2_type) {
                append_error(std::make_pair("Assign ERROR4: ", "Cannot convert type \'" + l_type + "\' to \'" + r2_type + "\' in line " + std::to_string(node.get_rvalue2()->get_line())));
            }
        }

        
    }
    
    
}
void SemanticVisitor::visit(ASTReturn& node) {}
void SemanticVisitor::visit(ASTIf& node) {
    std::string type_second_operand = "";
    std::string type_first_operand = "";
    if(node.get_second() != "")
    {
        if(node.is_literal())
        {
            if(table[get_fname_index(current_function_scope)].contains(node.get_second()))
            {
                type_second_operand = get_literal_type(node.get_second());
            } else if(inner_table[std::to_string(inner_counter)].contains(node.get_second()))
            {
                type_second_operand = inner_table[std::to_string(inner_counter)][node.get_second()];
                if(type_second_operand == "FLOAT" || type_second_operand == "FLOAT_NUMBER")
                {
                    type_second_operand = "float";
                } else if(type_second_operand == "NUMBER")
                {
                    type_second_operand = "int";
                }else if(type_second_operand == "TEXT" || type_second_operand == "STRING")
                {
                    type_second_operand = "string";
                }else if(type_second_operand == "CHAR")
                {
                    type_second_operand = "char";
                }
            }
        
        } else {
            
            if(inner_table[std::to_string(inner_counter)].contains(node.get_second()))
            {
                type_second_operand = inner_table[std::to_string(inner_counter)][node.get_second()];
            } else if(table[get_fname_index(current_function_scope)].contains(node.get_second()))
            {
                type_second_operand = table[get_fname_index(current_function_scope)][node.get_second()].type;
                
            } else if(table[0].contains(node.get_second()))
            {
                type_second_operand = table[0][node.get_second()].type;
            }
        }
        if(inner_table[std::to_string(inner_counter)].contains(node.get_first()))
        {
            
            type_first_operand = inner_table[std::to_string(inner_counter)][node.get_first()];
        } else if(table[get_fname_index(current_function_scope)].contains(node.get_first()))
        {
            type_first_operand = table[get_fname_index(current_function_scope)][node.get_first()].type;
        } else if(table[0].contains(node.get_first()))
        {
            type_second_operand = table[0][node.get_first()].type;
        }

        if(type_first_operand != type_second_operand)
        {
            errors.emplace_back(std::make_pair("If ERROR", "Operator \'" + node.get_op() + "\' cannot be applied to operands of type \'"+ type_first_operand + "\' to \'" + type_second_operand + "\' in line "+ std::to_string(node.get_line())));
        }
    } else {
        errors.emplace_back(std::make_pair("If ERROR", "Cannot implicitly convert type \'" + node.get_second_type() + "\' to \'bool\' in line "+ std::to_string(node.get_line())));
    }
    current_scope = "if";
    node.get_scope()->accept(*this);
    current_scope = "";
    
    if(node.get_else() != nullptr)
    {
        current_scope = "else";
        node.get_else()->accept(*this);
        current_scope = "";
       
    }
}
void SemanticVisitor::visit(ASTFor& node) {
    node.get_assing()->accept(*this);
    node.get_cond()->accept(*this);
    node.get_op()->accept(*this);
    current_scope = "for";
    node.get_scope()->accept(*this);
    current_scope = "";
    
}
void SemanticVisitor::visit(ASTForCond& node) {
    std::string type_second_operand = "";
    if(node.get_second() != "")
    {
        if(node.is_literal())
        {
            if(table[get_fname_index(current_function_scope)].contains(node.get_second()))
            {
                type_second_operand = get_literal_type(node.get_second());
            } else if(inner_table[std::to_string(inner_counter+1)].contains(node.get_second()))
            {
                type_second_operand = inner_table[std::to_string(inner_counter+1)][node.get_second()];
                if(type_second_operand == "FLOAT" || type_second_operand == "FLOAT_NUMBER")
                {
                    type_second_operand = "float";
                } else if(type_second_operand == "NUMBER")
                {
                    type_second_operand = "int";
                }else if(type_second_operand == "TEXT" || type_second_operand == "STRING")
                {
                    type_second_operand = "string";
                }else if(type_second_operand == "CHAR")
                {
                    type_second_operand = "char";
                }
            }
            
        } else {
            if(table[get_fname_index(current_function_scope)].contains(node.get_second()))
            {
                type_second_operand = table[get_fname_index(current_function_scope)][node.get_second()].type;
            } else if(inner_table[std::to_string(inner_counter+1)].contains(node.get_second()))
            {
                type_second_operand = inner_table[std::to_string(inner_counter+1)][node.get_second()];
            }
        }
    } else {
        errors.emplace_back(std::make_pair("For-condition ERROR","Cannot implicitly convert type \'" + table[get_fname_index(current_function_scope)][node.get_second()].type + "\' to \'bool\' in line "+ std::to_string(node.get_line())));
    }
    if(table[get_fname_index(current_function_scope)][node.get_first()].type != "int" && table[get_fname_index(current_function_scope)][node.get_first()].type != "float")
    {
        if(inner_table[std::to_string(inner_counter+1)][node.get_first()] != "int" && inner_table[std::to_string(inner_counter+1)][node.get_first()] != "float")
        {
            errors.emplace_back(std::make_pair("For-condition ERROR", "Cannot iterate by type \'" + table[get_fname_index(current_function_scope)][node.get_first()].type + "\' in line "+ std::to_string(node.get_line())));
        }
            
    }
    
}
void SemanticVisitor::visit(ASTForOp& node) {
    if(node.get_assign() != nullptr)
    {
        node.get_assign()->accept(*this);
    } else {
        if(table[get_fname_index(current_function_scope)][node.get_id()].type != "int" && table[get_fname_index(current_function_scope)][node.get_id()].type != "float")
        {
            if(inner_table[std::to_string(inner_counter+1)][node.get_id()] != "int" && inner_table[std::to_string(inner_counter+1)][node.get_id()] != "float")
            {
                errors.emplace_back(std::make_pair("For-operation ERROR","Operator \'" + node.get_unary_op() + "\' cannot be applied to operand of type \'"+ table[get_fname_index(current_function_scope)][node.get_id()].type + "\' in line "+ std::to_string(node.get_line())));
            }
        }
    }
}
void SemanticVisitor::visit(ASTKw& node) {}

void SemanticVisitor::visit(ASTPrint &node) {}
