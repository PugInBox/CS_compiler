#include "grammar/CsharpVisitor.h"
#include "codegen.hpp"

#include <any>
#include <fstream>
#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>

//STRING
//call i32 (i8*, ...) @printf(i8* noundef getelementptr inbounds ([3 x i8], [3 x i8]* @.str, i64 0, i64 0), i8* noundef %4) string
// @__const.main.b = private unnamed_addr constant [5 x i8] c"gg\00\00\00", align 1
// @.str = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1


//INT
//call i32 (i8*, ...) @printf(i8* noundef getelementptr inbounds ([3 x i8], [3 x i8]* @.str, i64 0, i64 0), i32 noundef %5) int
// @.str = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

//CHAR
//call i32 (i8*, ...) @printf(i8* noundef getelementptr inbounds ([3 x i8], [3 x i8]* @.str, i64 0, i64 0), i32 noundef %5) char
//@.str = private unnamed_addr constant [4 x i8] c"%c\0A\00", align 1


//FLOAT
//call i32 (i8*, ...) @printf(i8* noundef getelementptr inbounds ([3 x i8], [3 x i8]* @.str, i64 0, i64 0), double noundef %5)
// @.str = private unnamed_addr constant [4 x i8] c"%f\0A\00", align 1

std::size_t print_counter = 0;
std::size_t scanf_counter = 0;
std::string current_function = "Global";
std::size_t index = 0;
// ID = <%INDEX, TYPE>
std::map<std::string, std::pair<std::string, std::string>> name_index;
std::map<std::string, std::pair<std::string, std::string>> name_index_globals;
std::map<std::string, std::pair<std::string, std::string>> name_index_params;
std::map<std::string, std::pair<std::string, std::string>> name_index_scope;
std::map<std::string, std::string> funcs_props;
int amount_new = 0;
std::vector<std::string> name_index_scope_new;
std::map<std::string, std::string> global_strs;
bool is_inner_scope = false;

std::string new_index()
{
    std::string tmp = "";
    if(current_function == "Global")
    {
        tmp = "@g"+std::to_string(index);
    } else {
        tmp = "%v"+std::to_string(index);
    }
    index++;
    return tmp;
}
std::string new_global_index()
{
    std::string tmp = "@g"+std::to_string(index);
    index++;
    return tmp;
}

std::string get_llvm_type_from_literal(std::string p)
{
    auto ptype = "";
    if(std::isdigit(p[0]) && p.find('.')==std::string::npos)
    {
        ptype = "i32";
    } else if(p[0] == '\'')
    {
        ptype = "i8";
    } else if(std::isdigit(p[0]) && p.find('.')!=std::string::npos)
    {
        ptype = "double";
    }
    return ptype;
}


std::string get_llvm_type(std::string p)
{
    auto ptype = "";
    if(p == "int" || p == "NUMBER")
    {
        ptype = "i32";
    } else if(p == "char" || p == "CHAR")
    {
        ptype = "i8";
    } else if(p == "float" || p == "FLOAT_NUMBER" || p == "FLOAT")
    {
        ptype = "double";
    } else if(p == "void")
    {
        ptype = "void";
    }
    return ptype;
}

std::string get_llvm_op(std::string op, std::string type)
{
    auto ret_op = "";
    if(op == "+")
    {
        if(type == "int" || type == "i32")
        {
            ret_op = "add";
        }
        if(type == "float" || type == "double")
        {
            ret_op = "fadd";
        }
        
    } else if(op == "-")
    {
        if(type == "int" || type == "i32")
        {
            ret_op = "sub";
        }
        if(type == "float" || type == "double")
        {
            ret_op = "fsub";
        }
    } else if(op == "/")
    {
        if(type == "int" || type == "i32")
        {
            ret_op = "sdiv";
        }
        if(type == "float" || type == "double")
        {
            ret_op = "fdiv";
        }
        
    } else if(op == "%")
    {
        if(type == "float" || type == "double")
        {
            std::cerr << "ERROR: invalid operands to binary expression ('double' and 'double')\n";
        }
        ret_op = "srem";
    } else if(op == "*")
    {
        if(type == "int" || type == "i32")
        {
            ret_op = "mul";
        }
        if(type == "float" || type == "double")
        {
            ret_op = "fmul";
        }
    }

    return ret_op;
}

std::string get_log_op_int(std::string op)
{
    std::string lo = "";
    if(op == ">")
    {
        lo = "sgt";
    } else if(op == ">=") {
        lo = "sge";
    } else if(op == "<=") {
        lo = "sle";
    } else if(op == "<") {
        lo = "slt";
    } else if(op == "==") {
        lo = "eq";
    } else if(op == "!=") {
        lo = "ne";
    }
    return lo;
}

std::string get_log_op_double(std::string op)
{
    std::string lo = "";
    if(op == ">")
    {
        lo = "ogt";
    } else if(op == ">=") {
        lo = "oge";
    } else if(op == "<=") {
        lo = "ole";
    } else if(op == "<") {
        lo = "olt";
    } else if(op == "==") {
        lo = "oeq";
    } else if(op == "!=") {
        lo = "une";
    }

    return lo;
}

void CodeGen::visit(ASTProgram &node)
{
    stream << "source_filename = \"" << filepath << "\"\n";
    stream << "target datalayout = \"e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128\"\n";
    stream << "target triple = \"x86_64-pc-linux-gnu\"\n";

    stream << "@.str = private unnamed_addr constant [4 x i8] c\"%d\\0A\\00\", align 1\n";
    stream << "@.str.1 = private unnamed_addr constant [4 x i8] c\"%c\\0A\\00\", align 1\n";
    stream << "@.str.2 = private unnamed_addr constant [4 x i8] c\"%f\\0A\\00\", align 1\n";
    stream << "@.str.3 = private unnamed_addr constant [4 x i8] c\"%s\\0A\\00\", align 1\n";

    stream << "@.str.4 = private unnamed_addr constant [3 x i8] c\"%d\\00\", align 1\n";
    stream << "@.str.5 = private unnamed_addr constant [3 x i8] c\"%c\\00\", align 1\n";
    stream << "@.str.6 = private unnamed_addr constant [3 x i8] c\"%f\\00\", align 1\n";
    stream << "@.str.7 = private unnamed_addr constant [3 x i8] c\"%s\\00\", align 1\n";
    for(auto& child : node.get_children())
    {
        child->accept(*this);
    }
    if(!global_strs.empty())
    {
        for(auto& item : global_strs)
        {
            stream << item.first << item.second << "\n";
        }
        stream << "declare void @llvm.memcpy.p0i8.p0i8.i64(i8* noalias nocapture writeonly, i8* noalias nocapture readonly, i64, i1 immarg) #2\n";
    }
    if(print_counter > 0)
    {
        stream << "declare i32 @printf(i8* noundef, ...) #1\n";
    }
    if(scanf_counter > 0)
    {
        stream << "declare i32 @scanf(i8* noundef, ...) #1\n";
    }
    stream << "attributes #0 = { noinline nounwind optnone uwtable \"frame-pointer\"=\"all\" \"min-legal-vector-width\"=\"0\" \"no-trapping-math\"=\"true\" \"stack-protector-buffer-size\"=\"8\" \"target-cpu\"=\"x86-64\" \"target-features\"=\"+cx8,+fxsr,+mmx,+sse,+sse2,+x87\" \"tune-cpu\"=\"generic\" }\n";
    stream << "attributes #1 = { \"frame-pointer\"=\"all\" \"no-trapping-math\"=\"true\" \"stack-protector-buffer-size\"=\"8\" \"target-cpu\"=\"x86-64\" \"target-features\"=\"+cx8,+fxsr,+mmx,+sse,+sse2,+x87\" \"tune-cpu\"=\"generic\" }\n";
    stream << "attributes #2 = { argmemonly nofree nounwind willreturn }\n";
    stream.close();
}

void CodeGen::visit(ASTFunction &node)
{
    name_index.clear();
    name_index_params.clear();
    name_index = name_index_globals;
    current_function = node.func_name();
    funcs_props[node.func_name()] = node.return_type();
    stream << "define dso_local " << get_llvm_type(node.return_type()) << " @" << node.func_name();
//PARAMS
    stream << "(";
    std::size_t i = 0;
    for(auto& p : node.get_param_from_vector())
    {
        
        auto ptype = get_llvm_type(p.second);
        
        name_index[p.first] = std::make_pair(new_index(), ptype);
        stream << ptype << " noundef " << name_index[p.first].first;
        name_index_params[p.first] = std::make_pair(new_index(), ptype);
        if(i < node.get_param_from_vector().size()-1)
        {
            stream << ", ";
        }
        i++;
    }

    stream << ") ";
    stream << "#0 {\n";

    for(auto& item : name_index_params)
    {
        auto ind = new_index();
        auto old_ind = name_index[item.first].first;
        name_index[item.first].first = ind; 
        stream << ind << " = alloca " << name_index[item.first].second << "\n";
        stream << "store " << name_index[item.first].second <<" "<< old_ind << ", " << name_index[item.first].second << "* " << ind << "\n";
    }

//SCOPE
    node.get_scope()->accept(*this);
// RETURN
    std::string tmp_return = new_index();
    
    
    std::string ret_type;

    if(node.get_return() != nullptr){
        if(!name_index.contains(node.get_return()->get_return_value()))
            {
                ret_type = node.get_return()->get_return_type();

                ret_type = get_llvm_type(ret_type);

                stream << "ret ";
                stream << ret_type << " " << node.get_return()->get_return_value() << '\n';
            } else {
                ret_type = name_index[node.get_return()->get_return_value()].second;

                auto ind = name_index[node.get_return()->get_return_value()].first;

                stream << tmp_return << " = load " << ret_type << ", " << ret_type << "* " << ind << "\n";
                
                stream << "ret ";
                
                stream << ret_type << " " << tmp_return << '\n';
            }
    } else {
        stream << "ret void\n";
    }

    stream << "}\n";
}

void CodeGen::visit(ASTScope &node)
{
    int am_new_tmp = 0; 
    if(is_inner_scope)
    {
        am_new_tmp = amount_new;
    }
    for(auto& s : node.get_statements())
    {
        s->accept(*this);
    }
    for(; amount_new-am_new_tmp>0; amount_new--)
    {
        name_index_scope.erase(name_index_scope_new.back());
        name_index_scope_new.pop_back();
    }
}

void CodeGen::visit(ASTAssign &node)
{
    if(node.get_lvalue()->get_var_type() != "")
    {
        std::string ind = new_index();
        std::string type = "";
        if(node.get_lvalue()->get_var_type() == "string")
        {
            if(node.get_rvalue1()==nullptr)
            {
                std::cerr << "The string must be initialized, in line "+node.get_lvalue()->get_line() << "\n";
                throw;
            }
            if(node.get_rvalue1()->is_literal())
            {
                type = "[" + std::to_string(node.get_rvalue1()->get_var_name().size()-1) + " x i8]";
            } else {
                type = name_index[node.get_rvalue1()->get_var_name()].second;
                if(type == "float")
                    type = "double";
            }
            if(is_inner_scope)
            {
                name_index_scope[node.get_lvalue()->get_var_name()] = std::make_pair(ind, type);
                name_index_scope_new.push_back(node.get_lvalue()->get_var_name());
                amount_new++;
            } else {
                name_index[node.get_lvalue()->get_var_name()] = std::make_pair(ind, type);
            }
            
            stream << ind << " = alloca " << type << "\n";
            std::string pure_str = node.get_rvalue1()->get_var_name().substr(1,node.get_rvalue1()->get_var_name().size()-2);
            global_strs["@__const."+current_function+"."+node.get_lvalue()->get_var_name() + " = "] = "private unnamed_addr constant "+type+" c\"" + pure_str + "\\00\"\n";
            std::string bitcast_ind = new_index();
            stream << bitcast_ind << " = bitcast " << type << "* " << ind << " to i8*\n";
            stream << "call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 " << bitcast_ind << ", i8* align 1 getelementptr inbounds (" << type << ", " << type << "* " << "@__const." << current_function << "." << node.get_lvalue()->get_var_name() << ", i32 0, i32 0), i64 " << std::to_string(node.get_rvalue1()->get_var_name().size()-1) << ", i1 false)\n";
        } else {
            type = get_llvm_type(node.get_lvalue()->get_var_type());
            if(current_function == "Global")
            {
                stream << ind << " = global " << type;
                if(node.get_rvalue1() == nullptr)
                {   
                    if(type == "double" || type == "float")
                    {
                        stream << " 0.000000\n";
                    } else if(type == "int" || type == "i32" || type == "char" || type == "i8") {
                        stream << " 0\n";
                    } else if(type == "string" || type[0] == '[')
                    {
                        stream << " zeroinitializer\n";
                    }
                } else {
                    stream << " " << node.get_rvalue1()->get_var_name() << "\n";
                }
                
            } else {
                stream << ind << " = alloca " << type << "\n";
            }
            if(current_function == "Global")
            {
                name_index_globals[node.get_lvalue()->get_var_name()] = std::make_pair(ind, type);
            }
            if(is_inner_scope)
            {
                name_index_scope[node.get_lvalue()->get_var_name()] = std::make_pair(ind, type);
                name_index_scope_new.push_back(node.get_lvalue()->get_var_name());
                amount_new++;
            } else {
                name_index[node.get_lvalue()->get_var_name()] = std::make_pair(ind, type);
            }
        }
    } else {
        if(name_index[node.get_lvalue()->get_var_name()].second[0] == '[')
        {
            std::cerr << "Cannot assign new value to string, in line "+node.get_lvalue()->get_line() << "\n";
            throw;
        }
    }
    if(current_function != "Global")
    {
        if(node.get_rvalue1() != nullptr && node.get_rvalue2() != nullptr)
        {
            std::string type = get_llvm_type(node.get_rvalue1()->get_var_type());
            std::string op   = node.get_oper();
            float resf = 0.0f;
            int resi = 0;
            if(node.get_rvalue1()->is_literal() && node.get_rvalue2()->is_literal())
            {
                if(type == "double")
                {
                    float r1 = std::stof(node.get_rvalue1()->get_var_name());
                    float r2 = std::stof(node.get_rvalue2()->get_var_name());
                    if(op == "+") { resf = r1 + r2; } 
                    else if(op == "-") { resf = r1 - r2; }
                    else if(op == "/")
                    {
                        if(r2 == 0.0f)
                        {
                            std::cerr << "ERROR: Divide by zero\n";
                            throw;
                        }
                        resf = r1 / r2;
                    } else if(op == "%") { std::cerr << "ERROR: invalid operands to binary expression\n"; throw; }
                } else if(type == "i32")
                {
                    int r1 = std::stoi(node.get_rvalue1()->get_var_name());
                    int r2 = std::stoi(node.get_rvalue2()->get_var_name());
                    if(op == "+") { resi = r1 + r2; } 
                    else if(op == "-") { resi = r1 - r2; }
                    else if(op == "/")
                    {
                        if(r2 == 0.0f)
                        {
                            std::cerr << "ERROR: Divide by zero\n";
                            throw;
                        }
                        resi = r1 / r2;
                    } else if(op == "%") { resi = r1 % r2; }
                }

                stream << "store " << type << " ";
                if(type == "i32")
                {
                    stream << resi << ", ";
                } else if(type == "double")
                {
                    stream << resf << ", ";
                }
                if(is_inner_scope)
                {
                    stream << type << "* " << name_index_scope[node.get_lvalue()->get_var_name()].first << "\n";
                } else {
                    stream << type << "* " << name_index[node.get_lvalue()->get_var_name()].first << "\n";
                }
                
            }
            if(node.get_rvalue1()->is_literal() && !node.get_rvalue2()->is_literal())
            {
                std::string tmp_ind_b   = new_index();
                std::string tmp_literal = new_index();
                
                std::string r1_literal  = node.get_rvalue1()->get_var_name();
                std::string r2_type     = "";
                
                
                std::string r2_ind      = "";
                std::string l_ind       = "";
                if(name_index_scope.contains(node.get_lvalue()->get_var_name()))
                {
                    l_ind = name_index_scope[node.get_lvalue()->get_var_name()].first;
                } else {
                    l_ind   = name_index[node.get_lvalue()->get_var_name()].first;
                }
                if(name_index_scope.contains(node.get_rvalue2()->get_var_name()))
                {
                    r2_ind = name_index_scope[node.get_rvalue2()->get_var_name()].first;
                    r2_type = name_index_scope[node.get_rvalue2()->get_var_name()].second;
                } else {
                    r2_ind = name_index[node.get_rvalue2()->get_var_name()].first;
                    r2_type = name_index[node.get_rvalue2()->get_var_name()].second;
                }
                std::string op          = get_llvm_op(node.get_oper(), r2_type);
                stream << tmp_ind_b << " = load " << r2_type << ", " << r2_type << "* " << r2_ind << "\n";
                stream << tmp_literal << " = " << op << " " << r2_type << " " << r1_literal << ", " << tmp_ind_b << "\n";
                stream << "store " << r2_type << " " << tmp_literal << ", " << r2_type << "* " << l_ind << "\n";
            }
            if(!node.get_rvalue1()->is_literal() && node.get_rvalue2()->is_literal())
            {
                std::string tmp_ind_b   = new_index();
                std::string tmp_literal = new_index();
                
                std::string r2_literal  = node.get_rvalue2()->get_var_name();
                std::string r1_type     = "";
                
                std::string r1_ind      = "";
                std::string l_ind       = "";
                if(name_index_scope.contains(node.get_lvalue()->get_var_name()))
                {
                    l_ind = name_index_scope[node.get_lvalue()->get_var_name()].first;
                } else {
                    l_ind   = name_index[node.get_lvalue()->get_var_name()].first;
                }
                if(name_index_scope.contains(node.get_rvalue1()->get_var_name()))
                {
                    r1_ind = name_index_scope[node.get_rvalue1()->get_var_name()].first;
                    r1_type = name_index_scope[node.get_rvalue1()->get_var_name()].second;
                } else {
                    r1_ind = name_index[node.get_rvalue1()->get_var_name()].first;
                    r1_type = name_index[node.get_rvalue1()->get_var_name()].second;
                }
                std::string op          = get_llvm_op(node.get_oper(), r1_type);
                stream << tmp_ind_b << " = load " << r1_type << ", " << r1_type << "* " << r1_ind << "\n";
                stream << tmp_literal << " = " << op << " " << r1_type << " " << tmp_ind_b << ", " << r2_literal << "\n";
                stream << "store " << r1_type << " " << tmp_literal << ", " << r1_type << "* " << l_ind << "\n";
            }
            if(!node.get_rvalue1()->is_literal() && !node.get_rvalue2()->is_literal())
            {
                std::string tmp_ind_r1 = new_index();
                std::string tmp_ind_r2 = new_index();
                std::string res_r1_r2  = new_index();
                
                std::string r1_type    = "";
                std::string r2_type    = "";
                
                std::string r1_ind     = "";
                std::string r2_ind     = "";
                std::string l_ind      = "";
                if(name_index_scope.contains(node.get_lvalue()->get_var_name()))
                {
                    l_ind = name_index_scope[node.get_lvalue()->get_var_name()].first;
                } else {
                    l_ind   = name_index[node.get_lvalue()->get_var_name()].first;
                }
                if(name_index_scope.contains(node.get_rvalue1()->get_var_name()))
                {
                    r1_ind = name_index_scope[node.get_rvalue1()->get_var_name()].first;
                    r1_type = name_index_scope[node.get_rvalue1()->get_var_name()].second;
                } else {
                    r1_ind = name_index[node.get_rvalue1()->get_var_name()].first;
                    r1_type = name_index[node.get_rvalue1()->get_var_name()].second;
                }
                if(name_index_scope.contains(node.get_rvalue2()->get_var_name()))
                {
                    r2_ind = name_index_scope[node.get_rvalue2()->get_var_name()].first;
                    r2_type = name_index_scope[node.get_rvalue2()->get_var_name()].second;
                } else {
                    r2_ind = name_index[node.get_rvalue2()->get_var_name()].first;
                    r2_type = name_index[node.get_rvalue2()->get_var_name()].second;
                }
                std::string op         = get_llvm_op(node.get_oper(), r2_type);
                stream << tmp_ind_r1 << " = load " << r1_type << ", " << r1_type << "* " << r1_ind << "\n";
                stream << tmp_ind_r2 << " = load " << r2_type << ", " << r2_type << "* " << r2_ind << "\n";
                stream << res_r1_r2  << " = " << op << " " << r1_type << " " << tmp_ind_r1 << ", " << tmp_ind_r2 << "\n";
                stream << "store " << r1_type << " " << res_r1_r2 << ", " << r1_type << "* " << l_ind << "\n";
            }
        } else if(node.get_rvalue1() != nullptr)
        {
            if(node.get_rvalue1()->is_literal() && node.get_lvalue()->get_var_type()!="string")
            {
                std::string literal = node.get_rvalue1()->get_var_name();
                if(literal[0] == '\'')
                {
                    literal = std::to_string(static_cast<int>(literal[1]));
                }
                std::string l_ind = "";
                std::string l_type = "";
                if(name_index_scope.contains(node.get_lvalue()->get_var_name()))
                {
                    l_ind = name_index_scope[node.get_lvalue()->get_var_name()].first;
                    l_type = name_index_scope[node.get_lvalue()->get_var_name()].second;
                } else {
                    l_ind   = name_index[node.get_lvalue()->get_var_name()].first;
                    l_type  = name_index[node.get_lvalue()->get_var_name()].second;
                }
                
                stream << "store " << l_type << " " << literal << ", " << l_type << "* " << l_ind << "\n"; 
            }
            if(!node.get_rvalue1()->is_literal() && node.get_lvalue()->get_var_type()!="string")
            {
                std::string r1_ind  = "";
                std::string l_ind   = "";
                std::string l_type  = "";
                if(name_index_scope.contains(node.get_rvalue1()->get_var_name()))
                {
                    r1_ind = name_index_scope[node.get_rvalue1()->get_var_name()].first;
                } else {
                    r1_ind   = name_index[node.get_rvalue1()->get_var_name()].first;
                }
                if(name_index_scope.contains(node.get_lvalue()->get_var_name()))
                {
                    l_ind = name_index_scope[node.get_lvalue()->get_var_name()].first;
                    l_type = name_index_scope[node.get_lvalue()->get_var_name()].second;
                } else {
                    l_ind   = name_index[node.get_lvalue()->get_var_name()].first;
                    l_type  = name_index[node.get_lvalue()->get_var_name()].second;
                }
                std::string tmp_ind = new_index();
                stream << tmp_ind << " = load " << l_type << ", " << l_type << "* " << r1_ind << "\n";
                stream << "store " << l_type << " " << tmp_ind << ", " << l_type << "* " << l_ind << "\n"; 
            }
        }else if(node.get_funccall() != nullptr)
        {
            std::string l_ind = "";
            std::string l_type = "";
            if(name_index_scope.contains(node.get_lvalue()->get_var_name()))
            {
                l_ind = name_index_scope[node.get_lvalue()->get_var_name()].first;
                l_type = name_index_scope[node.get_lvalue()->get_var_name()].second;
            } else {
                l_ind   = name_index[node.get_lvalue()->get_var_name()].first;
                l_type  = name_index[node.get_lvalue()->get_var_name()].second;
            }
            auto args = node.get_funccall()->get_args();
            auto name = node.get_funccall()->func_name();
            auto ret_type = get_llvm_type(funcs_props[name]);
            std::string ind = "";
            std::string tmp_ind = "";
            std::string type = "";
            std::string res = "";
            std::vector<std::pair<std::string, std::string>> tmp_args;
            for(std::size_t i = 0; i < args.size(); i++)
            {
                if(name_index_scope.contains(args[i]->get_arg()))
                {
                    ind = name_index_scope[args[i]->get_arg()].first;
                } else {
                    ind = name_index[args[i]->get_arg()].first;
                }
                if(args[i]->is_literal())
                {
                    type = get_llvm_type_from_literal(args[i]->get_arg());
                } else {
                    if(name_index_scope.contains(args[i]->get_arg()))
                    {
                        type = name_index_scope[args[i]->get_arg()].second;
                    } else {
                        type = name_index[args[i]->get_arg()].second;
                    }
                }
                tmp_ind = new_index();
                stream << tmp_ind << " = load " << type << ", " << type << "* " << ind << "\n"; 
                tmp_args.push_back(std::make_pair(tmp_ind, type));
            }
            res = new_index();
            stream << res << " = ";
            stream << "call " << ret_type << " @" << name << "(";
            for(std::size_t i = 0; i < args.size(); i++)
            {
                stream << tmp_args[i].second << " noundef " << tmp_args[i].first;
                if(i+1 < args.size())
                {
                    stream << ", ";
                }
            }
            stream << ")\n";
            stream << "store " << l_type << " " << res << ", " << l_type << "* " << l_ind << "\n";
        }
    } 
    
}

void CodeGen::visit(ASTPrint &node)
{
    print_counter++;
    std::string tmp_ind = new_index();
    std::string tmp_ind2 = "";
    std::string tmp_ind2_type = "i32";
    std::string str = "@.str";
    std::string name_type = "";
    std::string name_ind = "";
    if(name_index_scope.contains(node.get_name()))
    {
        name_type = name_index_scope[node.get_name()].second;
        name_ind  = name_index_scope[node.get_name()].first;
    } else {
        name_type   = name_index[node.get_name()].second;
        name_ind    = name_index[node.get_name()].first;
    }
    if(name_type[0] != '[')
    {
        stream << tmp_ind << " = load " << name_type << ", " << name_type << "* " << name_ind << "\n";
    }
    
    if(name_type == "char" || name_type == "i8")
    {
        tmp_ind2 = new_index();
        stream << tmp_ind2 << " = sext " << name_type << " " << tmp_ind << " to i32\n";
        str = "@.str.1";
    }
    else if(name_type == "double")
    {
        tmp_ind2_type = "double"; 
        str = "@.str.2";
    } else if(name_type[0] == '[')
    {
        tmp_ind2 = new_index();
        std::string string_type = name_type;
        stream << tmp_ind2 << " = getelementptr inbounds " << string_type << ", " << string_type << "* " << name_ind << ", i64 0, i64 0\n";
        str = "@.str.3";
        tmp_ind2_type = "i8*";
    }
    if(tmp_ind2 == "")
    {
        tmp_ind2 = tmp_ind;
    }
    std::string call_index = new_index();
    stream << call_index << " = call i32 (i8*, ...) @printf(i8* noundef getelementptr inbounds ([4 x i8], [4 x i8]* " << str << ", i64 0, i64 0), " << tmp_ind2_type << " noundef " << tmp_ind2 << ")\n"; 
}

void CodeGen::visit(ASTRead &node)
{
    scanf_counter++;
    // std::string tmp_ind = new_index();
    std::string tmp_ind2 = "";
    std::string tmp_ind2_type = "i32";
    std::string str = "@.str.4";
    std::string name_type = "";
    std::string name_ind = "";
    if(name_index_scope.contains(node.get_name()))
    {
        name_type = name_index_scope[node.get_name()].second;
        name_ind  = name_index_scope[node.get_name()].first;
    } else {
        name_type   = name_index[node.get_name()].second;
        name_ind    = name_index[node.get_name()].first;
    }
    if(name_type == "double")
    {
        tmp_ind2_type = "double"; 
        str = "@.str.6";
    } else if(name_type[0] == '[')
    {
        tmp_ind2 = new_index();
        std::string string_type = name_type;
        stream << tmp_ind2 << " = getelementptr inbounds " << string_type << ", " << string_type << "* " << name_ind << ", i64 0, i64 0\n";
        str = "@.str.7";
        tmp_ind2_type = "i8*";
    }

    std::string call_index = new_index();
    stream << call_index << " = call i32 (i8*, ...) @scanf(i8* noundef getelementptr inbounds ([3 x i8], [3 x i8]* " << str << ", i64 0, i64 0), " << tmp_ind2_type << "* noundef " << name_ind << ")\n"; 
}

void CodeGen::visit(ASTElse &node)
{
    is_inner_scope = true;
    node.get_scope()->accept(*this);
    is_inner_scope = false;
}

void CodeGen::visit(ASTIf &node)
{
    std::string load_index1 = new_index();
    std::string log_op = "";
    std::string cmp_type = "icmp";
    std::string lvalue_type = "";
    std::string lvalue_index = "";
    if(name_index_scope.contains(node.get_first()))
    {
        lvalue_type = name_index_scope[node.get_first()].second;
        lvalue_index  = name_index_scope[node.get_first()].first;
    } else {
        lvalue_index   = name_index[node.get_first()].first;
        lvalue_type    = name_index[node.get_first()].second;
    }
    if(lvalue_type == "int" || lvalue_type == "i32")
    {
        log_op = get_log_op_int(node.get_op());
    } else if(lvalue_type == "float" || lvalue_type == "double")
    {
        cmp_type = "fcmp";
        log_op = get_log_op_double(node.get_op());
    }
    std::string load_index2 = "";
    
    
    std::string rvalue_type = "";
    std::string rvalue_index = "";
    
    if(node.is_literal())
    {
        rvalue_index = node.get_second();
        rvalue_type = get_llvm_type(node.get_second_type());
    } else {
        if(name_index_scope.contains(node.get_second()))
        {
            rvalue_type = name_index_scope[node.get_second()].second;
            rvalue_index   = name_index_scope[node.get_second()].first;
        } else {
            rvalue_index = name_index[node.get_second()].first;
            rvalue_type = name_index[node.get_second()].second;
        }
        
    }
    stream << load_index1 << " = load " << lvalue_type << ", " << lvalue_type << "* " << lvalue_index << "\n";

    if(!node.is_literal())
    {
        load_index2 = new_index();
        stream << load_index2 << " = load " << rvalue_type << ", " << rvalue_type << "* " << rvalue_index << "\n";
        rvalue_index = load_index2;
    }
    std::string cmp_index = new_index();
    std::string if_index = new_index();
    std::string else_index = "";
    
    stream << cmp_index << " = " << cmp_type << " " << log_op << " " <<  lvalue_type << " " << load_index1 << ", " << rvalue_index << "\n";
    stream << "br i1 " << cmp_index << ", label " << if_index;
    
    if(node.get_else() != nullptr)
    {
        else_index = new_index();
        stream << ", label " << else_index;
    } 
    std::string after_if_index = new_index();
    if(node.get_else() == nullptr)
    {
        stream << ", label " << after_if_index;
    }
    
    stream << "\n";
    if_index = if_index.substr(1);
    stream << if_index << ":\n";
    is_inner_scope = true;
    node.get_scope()->accept(*this);
    is_inner_scope = false;
    stream << "br label " << after_if_index << "\n";
    if(else_index != "")
    {
        else_index = else_index.substr(1);
        stream << else_index << ":\n";
        node.get_else()->accept(*this);
        stream << "br label " << after_if_index << "\n";
    }
    after_if_index = after_if_index.substr(1);
    stream << after_if_index << ":\n";
}

void CodeGen::visit(ASTFor &node)
{
    node.get_assing()->accept(*this);
    std::string cond = new_index();
    std::string end_for = new_index();
    std::string pure_cond = cond.substr(1);
    stream << "br label " << cond << "\n";
    stream << pure_cond << ":\n";

    
    std::string load = new_index();
    std::string load2 = "";
    std::string cmp_type = "icmp";
    std::string log_op = get_log_op_int(node.get_cond()->get_op());

    //std::string first_type = name_index[node.get_cond()->get_first()].second;
    std::string first_type = "";
    std::string first_index = "";
    if(name_index_scope.contains(node.get_cond()->get_first()))
    {
        first_type = name_index_scope[node.get_cond()->get_first()].second;
        first_index = name_index_scope[node.get_cond()->get_first()].first;
    } else {
        first_type = name_index[node.get_cond()->get_first()].second;
        first_index = name_index[node.get_cond()->get_first()].first;
    }
    std::string second = node.get_cond()->get_second();
    load2 = second;
    if(!node.get_cond()->is_literal())
    {
        if(name_index_scope.contains(node.get_cond()->get_second()))
        {
            second = name_index_scope[node.get_cond()->get_second()].first;
        } else {
            second = name_index[node.get_cond()->get_second()].first;
        }
        load2 = new_index();
        stream << load2 << " = load " << first_type << " " << ", " << first_type << "* " << second << "\n";
    }
    if(first_type == "double")
    {
        log_op = get_log_op_double(node.get_cond()->get_op());
        cmp_type = "fcmp";
    }
    stream << load << " = load " << first_type << ", " << first_type << "* " << first_index << "\n";
    std::string cmp = new_index();
    stream << cmp << " = " << cmp_type << " " << log_op << " " << first_type << " " << load << ", " << load2 << "\n";
    std::string inner = new_index();
    std::string after_op = new_index();
    stream << "br i1 " << cmp << ", label " << inner << ", label " << end_for << "\n";
    stream << inner.substr(1) << ":\n";
    is_inner_scope = true;
    node.get_scope()->accept(*this);
    is_inner_scope = false;
    stream << "br label " << after_op << "\n";

    stream << after_op.substr(1) << ":\n";
    std::string after_load = new_index();
    if(node.get_op()->get_assign())
    {
        node.get_op()->get_assign()->accept(*this);
    } else {
        std::string after_id = node.get_op()->get_id();
        std::string after_oper = node.get_op()->get_unary_op();
        std::string op1sym(1, after_oper[0]);
        std::string after_id_type = "";
        std::string after_id_index = "";
        if(name_index_scope.contains(after_id))
        {
            after_id_type = name_index_scope[after_id].second;
            after_id_index = name_index_scope[after_id].first;
        } else {
            after_id_type = name_index[after_id].second;
            after_id_index = name_index[after_id].first;
        }
        std::string llvm_oper = get_llvm_op(op1sym, after_id_type);
        

        stream << after_load << " = load " << after_id_type << ", " << after_id_type << "* " << after_id_index << "\n";
        std::string res = new_index();
        stream << res << " = " << llvm_oper << " " << after_id_type << " " << after_load;
        if(after_id_type == "double")
        {
            stream << ", 1.000000\n";
        } else {
            stream << ", 1\n";
        }
        //first_index = cond->first->index
        
        stream << "store " << after_id_type << " " << res << ", " << after_id_type << "* " << after_id_index << "\n";
        
    }
    stream << "br label " << cond << "\n";
    stream << end_for.substr(1) << ":\n";
}

void CodeGen::visit(ASTFuncCall &node)
{
    auto args = node.get_args();
    auto name = node.func_name();
    auto ret_type = get_llvm_type(funcs_props[name]);
    std::string ind = "";
    std::string tmp_ind = "";
    std::string type = "";
    std::vector<std::pair<std::string, std::string>> tmp_args;
    for(std::size_t i = 0; i < args.size(); i++)
    {
        if(name_index_scope.contains(args[i]->get_arg()))
        {
            ind = name_index_scope[args[i]->get_arg()].first;
        } else {
            ind = name_index[args[i]->get_arg()].first;
        }
        if(args[i]->is_literal())
        {
            type = get_llvm_type_from_literal(args[i]->get_arg());
        } else {
            if(name_index_scope.contains(args[i]->get_arg()))
            {
                type = name_index_scope[args[i]->get_arg()].second;
            } else {
                type = name_index[args[i]->get_arg()].second;
            }
        }
        tmp_ind = new_index();
        stream << tmp_ind << " = load " << type << ", " << type << "* " << ind << "\n"; 
        tmp_args.push_back(std::make_pair(tmp_ind, type));
    }

    stream << "call " << ret_type << " @" << name << "(";
    for(std::size_t i = 0; i < args.size(); i++)
    {
        stream << tmp_args[i].second << " noundef " << tmp_args[i].first;
        if(i+1 < args.size())
        {
            stream << ", ";
        }
    }
    stream << ")\n";
}