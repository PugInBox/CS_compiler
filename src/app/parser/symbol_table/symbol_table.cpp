#include "grammar/CsharpVisitor.h"
#include "symbol_table.hpp"

#include <any>
#include <fstream>
#include <iostream>

//   void visit(ASTProgram &node) override;
//   void visit(ASTFunction &node) override;
//   void visit(ASTVariable &node) override;
//   void visit(ASTFuncCall &node) override;
//   void visit(ASTScope &node) override;
//   void visit(ASTArgs &node) override;
//   void visit(ASTAssign &node) override;
//   void visit(ASTReturn &node) override;
//   void visit(ASTIf &node) override;
//   void visit(ASTFor &node) override;
//   void visit(ASTForCond &node) override;
//   void visit(ASTForOp &node) override;
//   void visit(ASTKw &node) override;

std::size_t VisitorTable::table_level = 0;
std::size_t VisitorTable::vector_num = 0;
std::size_t inner_counter = 0;
static std::string current_function = "Global";
static std::string current_scope = "";
std::size_t expr_counter = 0;
int func_scope = 0;
bool pit = false;

void VisitorTable::visit(ASTProgram& node)
{
    set_fname_index(current_function);
    std::map<std::string, Properties> global;
    table.push_back(global);
    for(auto& child : node.get_children())
    {
        child->accept(*this);
    }
}

void VisitorTable::visit(ASTFunction& node)
{
    func_scope = 1;
    Properties p;
    p.level = table_level;
    current_function = node.func_name();
    std::map<std::string, Properties> func;
    table.push_back(func);
    set_fname_index(current_function);
    p.type = node.return_type();
    
    incr_level();
    for(auto& param : node.get_params())
    {

        param->accept(*this);

        node.append_param_to_vector(param->get_var_name(), param->get_var_type());
    }

    set_fprop(node.func_name(), std::make_pair("", ""));
    for(auto& param : node.get_param_from_vector())
    {

        set_fprop(node.func_name(), param);

        //std::cout << "Params: " << param.first << " :: " << param.second << "\n";
    }
    p.fragment_type = "FUNCTION_DEF";

    table[get_fname_index(current_function)][node.func_name()] = p;
    
    node.get_scope()->accept(*this);
    if(node.get_return() != nullptr)
    {
        // Properties p_return;
        p.level = table_level;
        if(node.get_return()->is_literal())
        {
            p.fragment_type = "LRETURN_VARIABLE"; //LRETURN -> L(iteral)RETURN
        } 
        else 
        {
            p.fragment_type = "RETURN_VARIABLE";
        }
        auto return_type = node.get_return()->get_return_type();
        if(return_type == "ID")
        {
            if(!table[get_fname_index(current_function)].contains(node.get_return()->get_return_value()))
            {
                // try {
                //     throw std::runtime_error("ERROR: Undifined variable \'" + node.get_return()->get_return_value() + "\' in return-statement");
                // } catch(std::runtime_error& e)
                // {
                //     std::cerr << e.what() << "\n";
                //     exit(EXIT_FAILURE);
                // }
            } else {
                p.type = table[get_fname_index(current_function)][node.get_return()->get_return_value()].type;
            }
        }
        else {
            p.type = node.get_return()->get_return_type();
        }
        table[get_fname_index(current_function)][node.get_return()->get_return_value()+"_"+"return"+"_"+std::to_string(table_level)] = p;
    }

    decr_level();
}

void VisitorTable::visit(ASTVariable& node) 
{
    Properties p;
    if(node.get_ctx_type() == "ASSIGN")
    {
        if(node.is_literal())
        {
            p.fragment_type = "LVARIABLE";
        }
        else
        {
            if(node.get_expr_type() == "for")
            {
                p.fragment_type = "FORVARIABLE"+std::to_string(expr_counter);
                expr_counter++;  
            } else if(node.get_expr_type() == "if") {
                p.fragment_type = "IFVARIABLE"+std::to_string(expr_counter);
            } else if(node.get_expr_type() == "else") {
                p.fragment_type = "ELSEVARIABLE"+std::to_string(expr_counter);
            } else {
                p.fragment_type = "VARIABLE";
            }
            
        }
        //REDEFINITION ERROR
        // std::cout << node.get_var_name() << "::" << node.get_frag() << "::" << node.get_var_type() << current_function << "::" << node.get_expr_type() << "\n";
        if(table[get_fname_index(current_function)].contains(node.get_var_name()) && node.get_frag() == "LEFT_ASSIGN" && node.get_var_type() != "")
        {
            if(p.fragment_type.find("FOR") != std::string::npos && table[get_fname_index(current_function)][node.get_var_name()].fragment_type == p.fragment_type)
            {
                auto existing_symbol_type = table[get_fname_index(current_function)][node.get_var_name()].fragment_type;
                if(existing_symbol_type == p.fragment_type || p.fragment_type == "VARIABLE" || existing_symbol_type == "VARIABLE")
                {
                    try {
                        throw std::runtime_error("ERROR: Redefinition variable \'" + node.get_var_name() + "\'");
                    } catch(std::runtime_error& e)
                    {
                        std::cerr << e.what() << "\n";
                        exit(EXIT_FAILURE);
                    }
                }
            }
            
        }
        if(current_scope != "")
        {
            
            if(!node.is_literal() && node.get_var_type() != "" && inner_table[std::to_string(inner_counter)].contains(node.get_var_name()))
            {
                try {
                    throw std::runtime_error("ERROR: Redefinition variable \'" + node.get_var_name() + "\'");
                } catch(std::runtime_error& e)
                {
                    std::cerr << e.what() << "\n";
                    exit(EXIT_FAILURE);
                }
            }
        }
        // Because we gonna keep type variable when we assign new value to variable
        if(table[get_fname_index(current_function)].contains(node.get_var_name()) && node.get_frag() == "LEFT_ASSIGN" && node.get_var_type() == "")
        {
            return;
        }

        //UNDEFINED VARIABLE ERROR

        if(!table[get_fname_index(current_function)].contains(node.get_var_name()) && node.get_var_type() == "" && !inner_table[std::to_string(inner_counter)].contains(node.get_var_name()))
        {
            if((p.fragment_type.find("FOR") == std::string::npos || !inner_table[std::to_string(inner_counter+1)].contains(node.get_var_name())) && node.get_var_type() == "")
            {
                //CHECK IF VARIABLE EXSITS IN GLOBAL SCOPE
                if(table[0].contains(node.get_var_name()))
                {
                    node.set_var_type(table[0][node.get_var_name()].type);
                } else {
                    try {
                        throw std::runtime_error("ERROR: Undefined variable \'" + node.get_var_name() + "\'");
                    } catch(std::runtime_error& e)
                    {
                        std::cerr << e.what() << "\n";
                        exit(EXIT_FAILURE);
                    }
                }
            }

        }
    }
    else if(node.get_ctx_type() == "PARS")
    {
        if(node.is_literal())
        {
            //error
            throw "Parameter cannot be literal!\n";
            
            //p.fragment_type = "LPVARIABLE";
        }
        else
        {
            p.fragment_type = "PVARIABLE_" + node.get_func_name();
        }
         // P(arameter)Variable
    }

    p.level = table_level;
    p.type = node.get_var_type();
    if(node.get_var_type() == "")
    {
        if(inner_table[std::to_string(inner_counter)].contains(node.get_var_name()))
        {
            p.type = inner_table[std::to_string(inner_counter)].contains(node.get_var_name());
        }
        else if(table[get_fname_index(current_function)].contains(node.get_var_name()))
        {
            p.type = table[get_fname_index(current_function)][node.get_var_name()].type;
        } else if(table[0].contains(node.get_var_name()))
        {
            p.type = table[0][node.get_var_name()].type;
        } else {
            if(func_scope < 2 && p.fragment_type.find("FOR") == std::string::npos)
                try {
                    throw std::runtime_error("ERROR: Undefinefd variable \'" + node.get_var_name() + "\'");
                } catch(std::runtime_error& e)
                {
                    std::cerr << e.what() << "\n";
                    exit(EXIT_FAILURE);
                }
            }
    }

    std::string key = node.get_var_name();

    //if((node.get_frag() == "RIGHT_ASSIGN1" || node.get_frag() == "RIGHT_ASSIGN2") && p.fragment_type[0] != 'L' && current_scope == "")
    //{

      //  p.type = table[get_fname_index(current_function)][node.get_var_name()].type;
        //key = node.get_var_name();
        //--key = node.get_var_name() + "_" + std::to_string(table_level);

        //table[get_fname_index(current_function)][key] = p;

   // } //else if(p.fragment_type == "PVARIABLE") {
    //     key = key + "_" + node.get_func_name();
    // }
    if(current_scope == "" && p.fragment_type.find("FOR") == std::string::npos && func_scope < 3)
    {
        table[get_fname_index(current_function)][key] = p;
    } else {
        std::string type_in_var = node.get_var_type();
        if(type_in_var == "")
        {
            if(inner_table[std::to_string(inner_counter)].contains(node.get_var_name()))
            {
                type_in_var = inner_table[std::to_string(inner_counter)][node.get_var_name()];
            }else if(inner_table[std::to_string(inner_counter+1)].contains(node.get_var_name()) && (p.fragment_type.find("FOR") != std::string::npos || func_scope == 2))
            {
                type_in_var = inner_table[std::to_string(inner_counter+1)].contains(node.get_var_name());
            } 
            else if(table[get_fname_index(current_function)].contains(node.get_var_name()))
            {
                
                type_in_var = table[get_fname_index(current_function)][node.get_var_name()].type;
            } else if(table[0].contains(node.get_var_name())) {
                
                type_in_var = table[0][node.get_var_name()].type;
            } else {
                try {
                    throw std::runtime_error("ERROR: Undefinefd variable \'" + node.get_var_name() + "\'");
                } catch(std::runtime_error& e)
                {
                    std::cerr << e.what() << "\n";
                    exit(EXIT_FAILURE);
                }
            }
        }
        if(node.get_expr_type() == "for")
        {
            inner_counter++;
        }
        inner_table[std::to_string(inner_counter)][node.get_var_name()] = type_in_var;
        if(node.get_expr_type() == "for")
        {
            inner_counter--;
        }
    }
    

}


void VisitorTable::visit(ASTFuncCall& node) 
{
    Properties p;
    
    p.level = table_level;
    p.type = "~";
    for(auto& arg: node.get_args())
    {
        arg->accept(*this);
        if(arg->is_literal())
        {
            node.append_args_to_vector(arg->get_arg(), get_literal_type(arg->get_arg(), current_function));
        } else {
            node.append_args_to_vector(arg->get_arg(), table[get_fname_index(current_function)][arg->get_arg()].type);
        }
    }

    // for(auto& param : node.get_args_from_vector())
    // {
    //     std::cout << "Args: " << param.first << " :: " << param.second << "\n";
    // }
    p.fragment_type = "FUNCTION_CALL";
    table[get_fname_index(current_function)][node.func_name()] = p;
}

void VisitorTable::visit(ASTScope& node)
{
    func_scope++;
    if(current_scope != "" && inner_counter > 1)
    {
        for(auto& item : inner_table[std::to_string(inner_counter)])
        {
            inner_table[std::to_string(inner_counter+1)][item.first] = item.second;
        }
    }
    inner_counter++;
    if(func_scope > 2)
    {
        current_scope = "scope";
    }
    
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
        if(current_scope == "" && func_scope > 2)
        {
            current_scope = "scope";
        }
    }
    func_scope--;
    current_scope = "";
    inner_counter += amount_in_scopes;
}

void VisitorTable::visit(ASTElse &node) {
    incr_level();
    std::size_t cs = inner_counter+1;
    node.get_scope()->accept(*this);
    if(pit)
        print_inner_table(std::to_string(cs));
    
    decr_level();
}

void VisitorTable::visit(ASTArgs& node)
{
    std::string key = "";
    Properties p;
    if(node.is_literal())
    {
        p.fragment_type = "LARGUMENT";

        if(node.get_arg()[0] == std::string("\"")[0])
        {
            p.type = "TEXT";
        } else if(std::isdigit(node.get_arg()[0])) {
            if(node.get_arg().find(std::string(".")) != std::string::npos)
            {
                p.type = "FLOAT_NUMBER";
            } else {
                p.type = "NUMBER";
            }
            
        } else if(node.get_arg()[0] == std::string("\'")[0])
        {
            p.type = "CHAR";
        }
        key = node.get_arg();
    }
    else {
        p.fragment_type = "ARGUMENT";
        key = node.get_arg()+"_argument_"+std::to_string(table_level);
        if(!table[get_fname_index(current_function)].contains(node.get_arg()))
        {
            try {
            throw std::runtime_error("ERROR: Undefined argument \'" + node.get_arg() + "\' in function-call");
            } catch(std::runtime_error& e)
            {
                std::cerr << e.what() << "\n";
                exit(EXIT_FAILURE);
            }
        }
        else {
            p.type = table[get_fname_index(current_function)][node.get_arg()].type;
        }
    }
    p.level = table_level;
    table[get_fname_index(current_function)][key] = p;

}

void VisitorTable::visit(ASTAssign& node)
{
    if(node.get_lvalue() != nullptr)
    {
        node.get_lvalue()->accept(*this);
    }

    if(node.get_rvalue1() != nullptr)
    {
        node.get_rvalue1()->accept(*this);
    }

    if(node.get_rvalue2() != nullptr)
    {
        node.get_rvalue2()->accept(*this);
    }
}

void VisitorTable::visit(ASTReturn& node)
{
    Properties p;
    if(node.is_literal())
    {
        p.fragment_type = "LRETURN";
    }
    else
    {
        p.fragment_type = "RETURN";
    }

    p.level = table_level;
    p.type = node.get_return_type();
    table[get_fname_index(current_function)][node.get_return_value()] = p;
}

void VisitorTable::visit(ASTIf& node)
{
    // Properties p1;
    // p1.fragment_type = "IFVARIABLE";
    // p1.level = table_level;
    // p1.type = node.get_first_type();
    // if(current_scope == "")
    // {
    //     table[get_fname_index(current_function)][node.get_first()] = p1;
    // } else {
        
        if(table[get_fname_index(current_function)].contains(node.get_first()) && !inner_table[std::to_string(inner_counter)].contains(node.get_first()))
        {
            inner_table[std::to_string(inner_counter)][node.get_first()] = table[get_fname_index(current_function)][node.get_first()].type;
        } else if(table[0].contains(node.get_first()) && !inner_table[std::to_string(inner_counter)].contains(node.get_first()))
        {
            inner_table[std::to_string(inner_counter)][node.get_first()] = table[0][node.get_first()].type;
        
        } else if(!inner_table[std::to_string(inner_counter)].contains(node.get_first())) {
            try {
                throw std::runtime_error("ERROR: Undefined variable \'" + node.get_first() + "\' in if-statement\n");
            } catch(std::runtime_error& e)
            {
                std::cerr << e.what() << "\n";
                exit(EXIT_FAILURE);
            }
        }
    
    if(node.is_literal())
    {
        if(current_scope == "")
        {
            Properties p2;
            p2.fragment_type = "LIFVARIABLE";
            p2.level = table_level;
            p2.type = node.get_second_type();
            table[get_fname_index(current_function)][node.get_second()] = p2;
        } else {
            inner_table[std::to_string(inner_counter)][node.get_second()] = node.get_second_type();
        }
        
    }
    incr_level();
    std::size_t cs = inner_counter+1;
    node.get_scope()->accept(*this);
    if(pit)
        print_inner_table(std::to_string(cs));
    
    decr_level();
    if(node.get_else() != nullptr)
    {
        node.get_else()->accept(*this);
    }
}

void VisitorTable::visit(ASTFor& node)
{
    if(node.get_assing() != nullptr)
    {
        node.get_assing()->get_lvalue()->set_expr_type("for");
        node.get_assing()->accept(*this);
        node.get_assing()->get_lvalue()->set_expr_type("");
    }

    if(node.get_cond() != nullptr)
    {
        node.get_cond()->accept(*this);
    }

    if(node.get_op() != nullptr)
    {
        // if(node.get_op()->get_assign() != nullptr)
        // {
        //     node.get_op()->get_assign()->get_lvalue()->set_expr_type("for");
        // }
        
        node.get_op()->accept(*this);
    }
    incr_level();
    std::size_t cs = inner_counter+1;
    node.get_scope()->accept(*this);
    if(pit)
        print_inner_table(std::to_string(cs));
    
    decr_level();

    //????????????????
    // if(table[get_fname_index(current_function)][node.get_assing()->get_lvalue()->get_var_name()].level > table_level)
    // {   
    //      try {
    //             throw std::runtime_error("ERROR: Redefinition variable \'" + node.get_assing()->get_lvalue()->get_var_name() + "\'");
    //         } catch(std::runtime_error& e)
    //         {
    //             std::cerr << e.what() << "\n";
    //             exit(EXIT_FAILURE);
    //         }
    // }
    
}

void VisitorTable::visit(ASTForCond& node)
{
    Properties p2;
    // std::cout << "ans" << (node.is_literal());
    if(node.is_literal())
    {
        p2.fragment_type = "LFORCONDVARIABLE";
        if(node.get_second()[0] == std::string("\"")[0])
        {
            p2.type = "TEXT";
        } else if(node.get_second().find('.') != std::string::npos) {
            p2.type = "FLOAT_NUMBER";
        } else if(std::isdigit(node.get_second()[0])) {
            p2.type = "NUMBER";
        } 
    }
    else
    {
        p2.fragment_type = "FORCONDVARIABLE";
        if(table[get_fname_index(current_function)].contains(node.get_second()))
        {
            p2.type = table[get_fname_index(current_function)][node.get_second()].type;
        } else if(inner_table[std::to_string(inner_counter)].contains(node.get_second()))
        {
            p2.type = inner_table[std::to_string(inner_counter)][node.get_second()];
        } else {
            // for(int i = 0; i < k(undefined); i++)
            try {
                throw std::runtime_error("ERROR1: Undefined variable \'" + node.get_second() + "\' in cond-expression: \'" + node.get_first() + " " + node.get_op() + " " + node.get_second() + "\'");
            } catch(std::runtime_error& e)
            {
                std::cerr << e.what() << "\n";
                exit(EXIT_FAILURE);
            }
        }
    }
    p2.level = table_level;
    // for(int i = 0; j(undefined) < 10; i++)
    if(!table[get_fname_index(current_function)].contains(node.get_first()))
    {
        if(!inner_table[std::to_string(inner_counter+1)].contains(node.get_first()))
        {
            try {
                throw std::runtime_error("ERROR2: Undefined variable \'" + node.get_first() + "\' in cond-expression: \'" + node.get_first() + " " + node.get_op() + " " + node.get_second() + "\'");
            } catch(std::runtime_error& e)
            {
                std::cerr << e.what() << "\n";
                exit(EXIT_FAILURE);
            }
        }
    }
    inner_table[std::to_string(inner_counter+1)][node.get_second()] = p2.type;
    if(current_scope == "")
    {
        //table[get_fname_index(current_function)][node.get_second()] = p2;
    } else {
        
    }
    
    // table[get_fname_index(current_function)][node.get_second()+"_cond" + "_" + std::to_string(table_level)] = p2;

    // Properties p1;
    // p1.fragment_type = "FORCONDVARIABLE";
    // p1.level = table_level;
    // p1.type = table[get_fname_index(current_function)][node.get_first()].type;
    // table[get_fname_index(current_function)][node.get_first()] = p1;

    // table[get_fname_index(current_function)][node.get_first()+"_cond" + "_" + std::to_string(table_level)] = p1;
}

void VisitorTable::visit(ASTForOp& node)
{
    if(node.get_assign() != nullptr)
    {
        node.get_assign()->accept(*this);
    }
    else
    {
        Properties p1;
        p1.fragment_type = "FOROPVARIABLE";
        p1.level = table_level;
        if(!table[get_fname_index(current_function)].contains(node.get_id()) && !inner_table[std::to_string(inner_counter+1)].contains(node.get_id()))
        {
            try {
            throw std::runtime_error("ERROR: Undefined variable \'" + node.get_id() + "\' in loop-expression: \'" + node.get_id() + node.get_unary_op() + "\'");
            } catch(std::runtime_error& e)
            {
                std::cerr << e.what() << "\n";
                exit(EXIT_FAILURE);
            }
        }
        if(current_scope == "" && table[get_fname_index(current_function)].contains(node.get_id()))
        {
            p1.type = table[get_fname_index(current_function)][node.get_id()].type;
        }
        
        // if(current_scope == "")
        // {   
        //     table[get_fname_index(current_function)][node.get_id()] = p1;
        // }
        
        //table[get_fname_index(current_function)][node.get_id()+"_op" + "_" + std::to_string(table_level)] = p1;
    }
}

void VisitorTable::visit(ASTPrint& node)
{
    if(!table[get_fname_index(current_function)].contains(node.get_name()) && !table[0].contains(node.get_name())&& !inner_table[std::to_string(inner_counter)].contains(node.get_name()))
    {
        try {
            throw std::runtime_error("ERROR: Undefined variable \'" + node.get_name() + "\' in line: " + std::to_string(node.get_line()) + "\n");
        } catch(std::runtime_error& e)
        {
            std::cerr << e.what() << "\n";
            exit(EXIT_FAILURE);
        }
    }
}

void VisitorTable::visit(ASTRead& node)
{
    if(!table[get_fname_index(current_function)].contains(node.get_name()) && !table[0].contains(node.get_name())&& !inner_table[std::to_string(inner_counter)].contains(node.get_name()))
    {
        try {
            throw std::runtime_error("ERROR: Undefined variable \'" + node.get_name() + "\' in line: " + std::to_string(node.get_line()) + "\n");
        } catch(std::runtime_error& e)
        {
            std::cerr << e.what() << "\n";
            exit(EXIT_FAILURE);
        }
    }
}