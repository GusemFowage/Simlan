#include "Parser.hpp"

#include <Simlan/Standard.hpp>
#include <cassert>

namespace Simlan::Parse {
    using enum Ast::EAstNodeType;

    Parser::Parser(Lex::Lexer &Ilexer) : lexer(Ilexer) { }

    node_ptr<Program> Parser::operator()() {
        return ParseNode<Program>();
    }

    void Parser::ExpectToken(Lex::ETokenType type) {
        assert(lexer.PeekToken().type == type);
        lexer.NextToken();
    }
    
    template<> node_ptr<Program> Parser::ParseNode<Program>(){
        auto prg = make_node<Program>();
        AreaList.push_back(&(*prg));
        while (lexer.PeekToken().type != Lex::ETokenType::S_End) {
            prg->stmts.push_back(ParseNode<Statement>());
        }
        ExpectToken(Lex::ETokenType::S_End);
        AreaList.pop_back();
        return prg;
    }

    template<> node_ptr<Statement> Parser::ParseNode<Statement>() {
        // auto stmt = make_node<Statement>();
        if (Standard::KeywordsTable.count(lexer.PeekToken().ident)) 
        switch (Standard::KeywordsTable.at(lexer.PeekToken().ident)) {
            // TODO: 实现语句解析
        case ESimlanKeyword::If:
            break;
        default:
            break;
        } else return ParseNode<Expression_Stmt>();
        return nullptr;
    }

    template<> node_ptr<Expression_Stmt> Parser::ParseNode<Expression_Stmt>() {
        auto expr_stmt = make_node<Expression_Stmt>();
        expr_stmt->expr = ParseNode<Expression>();
        ExpectToken(Lex::ETokenType::S_Semicolon);
        return expr_stmt;
    }
    
    template<> node_ptr<Expression> Parser::ParseNode<Expression>() {
        return ParseNode<Mid_Expr>();
    }

    template<> node_ptr<Mid_Expr> Parser::ParseNode<Mid_Expr>() {
        using EOperator = Ast::Node<Mid_Expr>::EOperator;
        using ETokenType = Lex::ETokenType;
        auto mid_expr = make_node<Mid_Expr>();
        // auto left = ParseNode<Primary_Expr>();
        auto tkn = lexer.PeekToken();

        // 乘法级
        auto mul = [&, this]() {
            node_ptr<Expression> left = ParseNode<Primary_Expr>();
            tkn = lexer.PeekToken();
            while (tkn.type == ETokenType::S_Star ||
                tkn.type == ETokenType::S_Slash) {
                auto midexpr = make_node<Mid_Expr>();
                midexpr->Lkd = move(left);
                midexpr->oprt =
                    tkn.type == ETokenType::S_Star ?
                    EOperator::Mul : EOperator::Div;
                lexer.NextToken();
                midexpr->Rkd = ParseNode<Primary_Expr>();
                left = move(midexpr);
                tkn = lexer.PeekToken();
            }
            return left;
        };
        // 加法级
        auto add = [&, this]() {
            node_ptr<Expression> left = mul();
            tkn = lexer.PeekToken();
            while (tkn.type == ETokenType::S_Plus ||
                tkn.type == ETokenType::S_Minus) {
                auto midexpr = make_node<Mid_Expr>();
                midexpr->Lkd = move(left);
                midexpr->oprt =
                    tkn.type == ETokenType::S_Plus ?
                    EOperator::Add : EOperator::Sub;
                lexer.NextToken();
                midexpr->Rkd = mul();
                left = move(midexpr);
                tkn = lexer.PeekToken();
            }
            return left;
        };
        // 比较
        auto cmp = [&, this]() {
            node_ptr<Expression> left = add();
            tkn = lexer.PeekToken();
            while (tkn.type == ETokenType::S_Less ||
                tkn.type == ETokenType::S_Greater ||
                tkn.type == ETokenType::S_Equal) {
                auto midexpr = make_node<Mid_Expr>();
                midexpr->Lkd = move(left);
                if (lexer.PeekToken(1).type == ETokenType::S_Question) {
                    switch (tkn.type) {
                    case ETokenType::S_Equal:
                        midexpr->oprt = EOperator::Equal;
                        break;
                    case ETokenType::S_Greater:
                        midexpr->oprt = EOperator::Greater;
                        break;
                    case ETokenType::S_Less:
                        midexpr->oprt = EOperator::Less;
                        break;
                    default:
                        break;
                    }
                }
                else if (lexer.PeekToken(1).type == ETokenType::S_Equal) {
                    switch (tkn.type) {
                    case ETokenType::S_Equal:
                        midexpr->oprt = EOperator::Equal;
                        break;
                    case ETokenType::S_Greater:
                        midexpr->oprt = EOperator::GreaterEqual;
                        break;
                    case ETokenType::S_Less:
                        midexpr->oprt = EOperator::LessEqual;
                        break;
                    default:
                        break;
                    }  
                }
                else {
                    // 不是判断语句
                    return move(midexpr->Lkd);
                }
                lexer.NextToken();
                lexer.NextToken();
                midexpr->Rkd = add();
                left = move(midexpr);
                tkn = lexer.PeekToken();
            }
            return left;
        };
        //// 赋值
        auto ass = [&, this]() {
            node_ptr<Expression> left = cmp();
            tkn = lexer.PeekToken();
            while (tkn.type == ETokenType::S_Equal) {
                auto midexpr = make_node<Assign_Expr>();
                midexpr->Lkd = move(left);
                midexpr->oprt = EOperator::Assign;
                lexer.NextToken();
                midexpr->Rkd = cmp();
                left = move(midexpr);
                tkn = lexer.PeekToken();
            }
            return left;
        };
        auto left = ass();
        return static_pointer_cast<Ast::Node<Mid_Expr>>(left);
    }

    template<> node_ptr<Primary_Expr> Parser::ParseNode<Primary_Expr>() {
        auto prmy = make_node<Primary_Expr>();
        switch (lexer.PeekToken().type) {
        case Lex::ETokenType::U_Number:
            prmy->expr = ParseNode<Number>();
            return prmy;
        default:
            return nullptr;
        }
    }

    template<> node_ptr<Number> Parser::ParseNode<Number>() {
        auto num = make_node<Number>();
        std::stringstream ss;
        ss << lexer.PeekToken().ident;
        ss >> num->value;
        ExpectToken(Lex::ETokenType::U_Number);
        return num;
    }

} // namespace Simlan::Parse

