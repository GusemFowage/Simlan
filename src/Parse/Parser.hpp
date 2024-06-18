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
    using Type = Ast::EAstNodeType;
    template<>
    node_ptr<Type::Program> Parser::ParseNode<Type::Program>();
    template<>
    node_ptr<Type::Statement> Parser::ParseNode<Type::Statement>();
    template<>
    node_ptr<Type::Expression_Stmt> Parser::ParseNode<Type::Expression_Stmt>();
    template<>
    node_ptr<Type::Block> Parser::ParseNode<Type::Block>();
    template<>
    node_ptr<Type::IfElse_Stmt> Parser::ParseNode<Type::IfElse_Stmt>();
    template<>
    node_ptr<Type::Expression> Parser::ParseNode<Type::Expression>();
    template<>
    node_ptr<Type::Mid_Expr> Parser::ParseNode<Type::Mid_Expr>();
    template<>
    node_ptr<Type::Primary_Expr> Parser::ParseNode<Type::Primary_Expr>();
    template<>
    node_ptr<Type::Number> Parser::ParseNode<Type::Number>();
    template<>
    node_ptr<Type::Variable> Parser::ParseNode<Type::Variable>();
    template<>
    node_ptr<Type::Variable_Def> Parser::ParseNode<Type::Variable_Def>();
} // namespace Simlan :: Parse

#endif // ! SIMLAN_PARSER_HPP