#include "parser/parser.hpp"
#include "optimizer/optimizer.hpp"
#include "semantic/semantic.hpp"
#include "codegen/codegen.hpp"
#include <string>
#include <vector>
#include <gtest/gtest.h>

const std::string path = "../../tests/lexer-tests-cs/";

TEST(LexerSuite, PositionTest) {
    const std::string testfile = "test1.cs";
    cs_lang::tokens_array tkn_arr = cs_lang::dump_tokens(path+testfile, 0);
    EXPECT_EQ(1, tkn_arr[0].get_x());
    EXPECT_EQ(0, tkn_arr[0].get_y());
    EXPECT_EQ(1, tkn_arr[1].get_x());
    EXPECT_EQ(7, tkn_arr[1].get_y());
    EXPECT_EQ(1, tkn_arr[2].get_x());
    EXPECT_EQ(12, tkn_arr[2].get_y());
    EXPECT_EQ(1, tkn_arr[3].get_x());
    EXPECT_EQ(16, tkn_arr[3].get_y());
    EXPECT_EQ(1, tkn_arr[4].get_x());
    EXPECT_EQ(17, tkn_arr[4].get_y());
    EXPECT_EQ(1, tkn_arr[5].get_x());
    EXPECT_EQ(23, tkn_arr[5].get_y());
    EXPECT_EQ(1, tkn_arr[6].get_x());
    EXPECT_EQ(24, tkn_arr[6].get_y());
    EXPECT_EQ(1, tkn_arr[7].get_x());
    EXPECT_EQ(26, tkn_arr[7].get_y());
    EXPECT_EQ(3, tkn_arr[19].get_x());
    EXPECT_EQ(35, tkn_arr[19].get_y());
}

TEST(LexerSuite, TokenTest)
{
    const std::string testfile = "test1.cs";
    cs_lang::tokens_array tkn_arr = cs_lang::dump_tokens(path+testfile, 0);
    std::vector<std::string> token_strs = {"static", "void", "Main", "(", "string", "[", "]", "args", ")", "{", "int", "[", ",", "]", "array", "=", "new", "int", "[", "5", ",", "5", "]", ";", "InitializeArray", "(", "array", ")", ";", "Console.WriteLine", "(", ")", ";", "Console.WriteLine", "(", "\"", "Min", "Element", "in", "array", ":", "{", "0", "}", "\"", ",", "MinElement", "(", "array", ")", ")", ";", "Console.ReadLine", "(", ")", ";", "}"};
    std::vector<std::string> token_names = {"KEYWORD", "VOID", "ID", "RLP", "VAR", "SLP", "SRP", "ID", "RRP", "CLB", "VAR", "SLP", "COMMA", "SRP", "ID", "ASSIGN", "KEYWORD", "VAR", "SLP", "NUMBER", "COMMA", "NUMBER", "SRP", "SEMICOLON", "ID", "RLP", "ID", "RRP", "SEMICOLON", "ID", "RLP", "RRP", "SEMICOLON", "ID", "RLP", "DQUOTES", "ID", "ID", "KEYWORD", "ID", "COLON", "CLB", "NUMBER", "CRB", "DQUOTES", "COMMA", "ID", "RLP", "ID", "RRP", "RRP", "SEMICOLON", "ID", "RLP", "RRP", "SEMICOLON", "CRB"};
    for(std::size_t i = 0; i < token_names.size(); i++)
    {
        EXPECT_EQ(token_strs[i], tkn_arr[i].get_token_str());
        EXPECT_EQ(token_names[i], tkn_arr[i].get_token_name());
    }
}

TEST(ParserSuite, ParserTest)
{
    const std::string testfile = "../../tests/parser-tests-cs/test.cs";
    std::string xml_file = "../../tests/parser-tests-cs/test.xml";
    std::vector<std::string> res{"<program>", "<function name='main' return-type='int', return_statement=(value=\"Enough.\", literal=1, type=STRING)/>", "<scope name='main'>", "<assign lhs=f, rhs=10.1, op=''/>", "<assign lhs=s, rhs=\"Hello\", op=''/>", "<assign lhs=c, rhs='A', op=''/>", "<assign lhs=i, rhs=10, op=''/>", "<assign lhs=f, rhs=(10, \"Hello\"), op='+'/>", "<assign lhs=d, rhs=(10.2, 'G'), op='+'/>", "<assign lhs=e, rhs=('E', 42.2), op='+'/>", "<assign lhs=f, rhs=(a, b), op='+'/>", "<call name='func', args=(\"Hello, World!\", 3.0, 2, 1, 0.0, 0, 'R', 'U', 'N', '!')/>", "<if lhs=(value=i, type=ID), lhs=(value=10, type=NUMBER), op='>'>", "<scope name='ifi'>", "<assign lhs=e, rhs=('E', 42.2), op='+'/>", "<assign lhs=f, rhs=(a, b), op='+'/>", "<call name='func', args=(\"Hello, World!\", 3.0, 2, 1, 0.0, 0, 'R', 'U', 'N', '!')/>", "</scope>", "</scope>", "</program>"};
    auto parse_result = cs_lang::parse_test(testfile);
    cs_lang::dump_ast(parse_result.m_program, xml_file, 0);
    std::ifstream xf(xml_file);
    xf.open(xml_file);
    std::string line;
    if (xf.is_open())
    {
        std::size_t i = 0;
        while (std::getline(xf,line) )
        {
            EXPECT_EQ(line, res[i]);
            i++;
        }
        xf.close();
        std::remove(xml_file.c_str());
    }
}

TEST(ParserSuite, ParserCycleTest)
{
    const std::string testfile = "../../tests/parser-tests-cs/cycletest.cs";
    std::string xml_file = "../../tests/parser-tests-cs/test.xml";
    std::vector<std::string> res{"<for <assign lhs=i, rhs=10, op=''/>, condition=(i>0), operation=(i--)/>", "<scope name=''>", "<call name='Console.WriteLine', args=(i)/>", "</scope>", "</program>"};
    auto parse_result = cs_lang::parse_test(testfile);
    cs_lang::dump_ast(parse_result.m_program, xml_file, 0);
    std::ifstream xf(xml_file);
    xf.open(xml_file);
    std::string line;
    if (xf.is_open())
    {
        std::size_t i = 0;
        while (std::getline(xf,line) )
        {
            EXPECT_EQ(line, res[i]);
            i++;
        }
        xf.close();
        std::remove(xml_file.c_str());
    }
}

std::string exec(const char* cmd) {
    FILE* pipe = popen(cmd, "r");
    if (!pipe) return "ERROR";
    char buffer[128];
    std::string result = "";
    while(!feof(pipe)) {
        if(fgets(buffer, 128, pipe) != NULL)
            result += buffer;
    }
    pclose(pipe);
    return result;
}

TEST(CodeGenSuite, ASMTest)
{
    const std::string filepath = "../../tests/codegen-tests-cs/test1.cs";
    std::string fileout = "../../tests/codegen-tests-cs/test1.ll";
    auto parse_result = cs_lang::parse_test(filepath);
    VisitorTable visitor;
    parse_result.m_program->accept(visitor);
    SemanticVisitor semantic_visitor(visitor.get_table(), visitor.get_fprops(), visitor.get_indexer(), visitor.get_inner_table());
    parse_result.m_program->accept(semantic_visitor);
    std::ofstream stream(fileout);
    CodeGen code_generator(stream, filepath, fileout);
    parse_result.m_program->accept(code_generator);
    const char* cmd = "cd ../../tests && chmod +x compile.sh && ./compile.sh";
    std::string res(exec(cmd));
    EXPECT_EQ(res, "20\n4.200000\n8\n-12.600000\n4\n1\n84\n52\n0\n0\n1\n2\n3\n4\n14\n0\n2022\n2022\n");
    stream.close();
    // std::remove("../../tests/codegen-tests-cs/test1.ll");
    std::remove("../../tests/codegen-tests-cs/test1.s");
    std::remove("../../tests/codegen-tests-cs/test1");
}