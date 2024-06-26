#ifndef SIMLAN_PARSER_HPP
#define SIMLAN_PARSER_HPP

#include <Lex/Lexer.hpp>
#include <Parse/Ast/Node.hpp>

namespace Simlan :: Parse {
    template<Ast::EAstNodeType type>
    using node_ptr = shared<Ast::Node<type>>;

    template<Ast::EAstNodeType type>
    node_ptr<type> make_node() {
        return make_shared<Ast::Node<type>>();
    }

    class Parser {
// #define make_node make_shared
        using enum Ast::EAstNodeType;
        List<Defarea*> AreaList;
        TypePtr curUseType;
        List<Map<string, node_ptr<Function_Def>>> funcdefs;
    public: 
        Parser(Lex::Lexer &Ilexer);
        node_ptr<Program> operator()();
    private:
        // 使用枚举模板函数
        template<Ast::EAstNodeType type>
        node_ptr<type> ParseNode() = delete;
        // 判断是否是期待的token，是则下一个，否则报错
        void ExpectToken(Lex::ETokenType type);
    private:
        Lex::Lexer &lexer;
    };
} // namespace Simlan :: Parse

#include "SimlanParser.inl"

#endif // ! SIMLAN_PARSER_HPP