#include "grammar/CsharpVisitor.h"
#include "optimizer.hpp"

#include <any>
#include <fstream>
#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>

static std::string current_function = "Global";
std::vector<ASTNode*> delete_ptrs;
std::vector<std::string> deleted_funcs;
ASTNode* program = nullptr;


bool OptimizerVisitor::is_del_func(std::string func_name)
{
    for(auto& del_func : deleted_funcs)
    {
        if(del_func == func_name)
        {
            return true;
        }
    }
    return false;
}

void OptimizerVisitor::visit(ASTProgram& node)
{
    program = &node;
    for(auto& child : node.get_children())
    {
        child->accept(*this);
    }
    if(opt)
    {
        PtrVisitor pvis(unused_vars);
        program->accept(pvis);
        DeleteVisitor dvis(unused_vars);
        program->accept(dvis);
    }
}

void OptimizerVisitor::visit(ASTFunction& node)
{

    clear_unused_vars();

    current_function = node.func_name();
    for(auto const& item : table[get_fname_index(current_function)])
    {
        if(item.second.fragment_type[0] != 'L' && item.second.fragment_type != "FUNCTION_CALL" && item.second.fragment_type != "FUNCTION_DEF")
        {
            if(item.first.find("_argument_") == std::string::npos && item.first.find("_return_") == std::string::npos)
            {
                append_unused_var(item.first);
            }
        }
    }
    for(auto const& params : node.get_params())
    {
        set_used_var(params->get_var_name());
    }

    if(node.get_scope()->get_statements().size() < 1)
    {
        append_warning(std::make_pair("Empty function", node.func_name()));
        deleted_funcs.push_back(node.func_name());
    } else {
        node.get_scope()->accept(*this);
    }
    if(node.get_return() != nullptr)
    {
        node.get_return()->accept(*this);
    }
    for(auto& item : get_unused_vars())
    {
        if(item.second)
        {
            if(item.first.find("_argument_") == std::string::npos && item.first.find("_return_") == std::string::npos)
            {
                append_warning(std::make_pair("Unused variable", item.first));
            }
        }
    }
    //print_unused_var();
    if(wall)
    {
        dump_warnings(current_function);
        warnings.clear();
    }
    if(opt)
    {
        PtrVisitor pvis(unused_vars);
        node.accept(pvis);
        DeleteVisitor dvis(unused_vars);
        node.accept(dvis);
        node.accept(pvis);
        node.accept(dvis);
        // node.accept(*this);
        // PtrVisitor pvis1(unused_vars);
        // DeleteVisitor dvis1(unused_vars);
        // program->accept(pvis);
        // program->accept(dvis);
    }

}

void OptimizerVisitor::visit(ASTScope &node)
{
    for(auto& statement : node.get_statements())
    {
        statement->accept(*this);
    }

}

void OptimizerVisitor::visit(ASTPrint &node)
{
    set_used_var(node.get_name());
}

void OptimizerVisitor::visit(ASTRead &node)
{
    set_used_var(node.get_name());
}

void OptimizerVisitor::visit(ASTFuncCall &node)
{
    
    // if(is_del_func(node.func_name()))
    // {
    //     append_warning(std::make_pair("Call empty function", node.func_name()));
    // }
        

    for(auto& arg : node.get_args_from_vector())
    {
        if(table[get_fname_index(current_function)][arg.first].fragment_type[0] != 'L')
        {
            std::size_t found = arg.first.find("_argument_");
            std::string first = arg.first;
            if(found != std::string::npos)
            {
                first = first.substr(0, found);
                
            }
            if(!is_del_func(node.func_name()))
            {
                set_used_var(first);
            }
        }
    }

}

void OptimizerVisitor::visit(ASTIf &node)
{

    set_used_var(node.get_first());
    node.get_scope()->accept(*this);
    if(node.get_else() != nullptr)
    {
        node.get_else()->accept(*this);
    }

}

void OptimizerVisitor::visit(ASTFor &node)
{
    set_used_var(node.get_assing()->get_lvalue()->get_var_name());
    node.get_cond()->accept(*this);
    node.get_scope()->accept(*this);
}

void OptimizerVisitor::visit(ASTAssign &node)
{

    if(node.get_rvalue1()!=nullptr && !node.get_rvalue1()->is_literal())
    {
        set_used_var(node.get_rvalue1()->get_var_name());
    }
    if(node.get_rvalue2()!=nullptr && !node.get_rvalue2()->is_literal())
    {
        set_used_var(node.get_rvalue2()->get_var_name());
    }

}

void OptimizerVisitor::visit(ASTForCond &node)
{

    if(table[get_fname_index(current_function)][node.get_second()].fragment_type[0] != 'L')
    {
        set_used_var(node.get_second());
    }
    set_used_var(node.get_first());

}

void OptimizerVisitor::visit(ASTElse &node)
{

    node.get_scope()->accept(*this);

}

void OptimizerVisitor::visit(ASTReturn &node)
{
    if(!node.is_literal())
    {
        set_used_var(node.get_return_value());
    }
}

//*********************************************************
//*----------------PtrVisitor-----------------------------*
//*********************************************************

void PtrVisitor::visit(ASTProgram& node)
{

    for(auto& child : node.get_children())
    {
        child->accept(*this);
    }

}

void PtrVisitor::visit(ASTFunction& node)
{
    current_function = node.func_name();
    node.get_scope()->accept(*this);
    if(node.get_scope()->get_statements().size() < 1)
    {
        delete_ptrs.push_back(&node);
        deleted_funcs.push_back(node.func_name());
    }
}

void PtrVisitor::visit(ASTScope &node)
{
    for(auto& statement : node.get_statements())
    {
        statement->accept(*this);
    }
}

void PtrVisitor::visit(ASTFuncCall &node)
{
    for(auto& fname : deleted_funcs)
    {
        if(node.func_name() == fname)
        {
            delete_ptrs.push_back(&node);
            break;
        }
    }
}

void PtrVisitor::visit(ASTIf &node)
{
    node.get_scope()->accept(*this);
    if(node.get_else() != nullptr)
    {
        node.get_else()->accept(*this);
    }
}

void PtrVisitor::visit(ASTFor &node)
{
    node.get_cond()->accept(*this);
}

void PtrVisitor::visit(ASTAssign &node)
{
    if(unused_vars.contains(node.get_lvalue()->get_var_name()) && unused_vars[node.get_lvalue()->get_var_name()])
    {
        delete_ptrs.push_back(&node);
    }
}

void PtrVisitor::visit(ASTForCond &node)
{}

void PtrVisitor::visit(ASTElse &node)
{
    node.get_scope()->accept(*this);
}

void PtrVisitor::visit(ASTReturn &node)
{}

//*********************************************************
//*----------------DeleteVisitor--------------------------*
//*********************************************************

void DeleteVisitor::visit(ASTProgram& node)
{
    for(auto& child : node.get_children())
    {
        child->accept(*this);
    }
    for(auto* statement : node.get_children())
    {
        for(auto* delete_stat : delete_ptrs)
        {
            if(statement == delete_stat)
            {
                node.remove_child(statement);
                break;
            } else {
                statement->accept(*this);
            }
        }
    }

}

void DeleteVisitor::visit(ASTFunction& node)
{
    current_function = node.func_name();
    node.get_scope()->accept(*this);
}

void DeleteVisitor::visit(ASTScope &node)
{
    if(node.get_statements().size() > 0)
    {
        for(auto* statement : node.get_statements())
        {
            statement->accept(*this);
        }
    }
    for(auto* statement : node.get_statements())
    {
        for(auto* delete_stat : delete_ptrs)
        {
            if(statement == delete_stat)
            {
                node.remove_statement(statement);
                break;
            }
        }
    }

}

void DeleteVisitor::visit(ASTFuncCall &node)
{}

void DeleteVisitor::visit(ASTIf &node)
{
    if(node.get_scope()->get_statements().size() > 0)
    {
        node.get_scope()->accept(*this);
    }
    
    if(node.get_else() != nullptr)
    {
        node.get_else()->accept(*this);
    }
}

void DeleteVisitor::visit(ASTFor &node)
{
    node.get_cond()->accept(*this);
}

void DeleteVisitor::visit(ASTAssign &node)
{

}

void DeleteVisitor::visit(ASTForCond &node)
{}

void DeleteVisitor::visit(ASTElse &node)
{
    node.get_scope()->accept(*this);
}

void DeleteVisitor::visit(ASTReturn &node)
{}