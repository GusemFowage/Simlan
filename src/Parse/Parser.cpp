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
        funcdefs.emplace_back();
        auto prg = make_node<Program>();
        static_assert(DEBUG_MODE, "暂时使用 uint64_t 作为默认变量类型");
        curUseType = prg->TypeTable["uint64_t"];
        AreaList.push_back(&(*prg));
        while (lexer.PeekToken().type != Lex::ETokenType::S_End) {
            if (lexer.PeekToken().type != Lex::ETokenType::L_Keyword ||
                Standard::KeywordsTable.at(lexer.PeekToken().ident) != ESimlanKeyword::Fnc) {
                prg->stmts.push_back(ParseNode<Statement>());
            } else ParseNode<Statement>();
        }
        ExpectToken(Lex::ETokenType::S_End);
        AreaList.pop_back();
        funcdefs.pop_back();
        return prg;
    }

    template<> node_ptr<Statement> Parser::ParseNode<Statement>() {
        // auto stmt = make_node<Statement>();
        if (Standard::KeywordsTable.count(lexer.PeekToken().ident)) 
        switch (Standard::KeywordsTable.at(lexer.PeekToken().ident)) {
            // TODO: 实现语句解析
        case ESimlanKeyword::Var:{
            ExpectToken(Lex::ETokenType::L_Keyword);
            auto var_def = ParseNode<Variable_Def>();
            if (lexer.PeekToken().type == Lex::ETokenType::S_Equal) {
                ExpectToken(Lex::ETokenType::S_Equal);
                var_def->expr = ParseNode<Expression>();
            }
            ExpectToken(Lex::ETokenType::S_Semicolon);
            return var_def;
        }
        case ESimlanKeyword::If: {
            return ParseNode<IfElse_Stmt>();
        }
        case ESimlanKeyword::For: {
            return ParseNode<For_Stmt>();
        }
        case ESimlanKeyword::While: {
            return ParseNode<While_Stmt>();
        }
        case ESimlanKeyword::Fnc: {
            ExpectToken(Lex::ETokenType::L_Keyword);
            return ParseNode<Function_Def>();
        }
        case ESimlanKeyword::Keep: {}
        default:
            break;
        } else if (lexer.PeekToken().type == Lex::ETokenType::S_LBrace) {
            return ParseNode<Block>();
        } 
        else return ParseNode<Expression_Stmt>();
        return nullptr;
    }

    template<> node_ptr<Type::Block> Parser::ParseNode<Type::Block>() {
        auto block = make_node<Block>();
        ExpectToken(Lex::ETokenType::S_LBrace);
        AreaList.push_back(&(*block));
        while (lexer.PeekToken().type != Lex::ETokenType::S_RBrace) {
            block->stmts.push_back(ParseNode<Statement>());
        }
        AreaList.pop_back();
        ExpectToken(Lex::ETokenType::S_RBrace);
        return block;
    }

    template<> node_ptr<IfElse_Stmt> Parser::ParseNode<IfElse_Stmt>(){
        auto ifelse = make_node<IfElse_Stmt>();
        ExpectToken(Lex::ETokenType::L_Keyword);
        ExpectToken(Lex::ETokenType::S_LParenthesis);
        ifelse->condition = ParseNode<Expression>();
        ExpectToken(Lex::ETokenType::S_RParenthesis);
        ifelse->stmt = ParseNode<Statement>();
        if (lexer.PeekToken().type == Lex::ETokenType::L_Keyword &&
            Standard::KeywordsTable.at(lexer.PeekToken().ident) ==  ESimlanKeyword::Else) {
            ExpectToken(Lex::ETokenType::L_Keyword);
            ifelse->else_stmt = ParseNode<Statement>();
        }
        return ifelse;
    }
    
    template<> node_ptr<For_Stmt> Parser::ParseNode<For_Stmt>() {
        ExpectToken(Lex::ETokenType::L_Keyword);
        auto for_stmt = make_node<For_Stmt>();
        ExpectToken(Lex::ETokenType::S_LParenthesis);
        for_stmt->init = ParseNode<Expression>();
        ExpectToken(Lex::ETokenType::S_Semicolon);
        for_stmt->condition = ParseNode<Expression>();
        ExpectToken(Lex::ETokenType::S_Semicolon);
        for_stmt->increment = ParseNode<Expression>();
        ExpectToken(Lex::ETokenType::S_RParenthesis);
        for_stmt->body = ParseNode<Statement>();
        return for_stmt;
    }
    
    template<> node_ptr<While_Stmt> Parser::ParseNode<While_Stmt>() {
        ExpectToken(Lex::ETokenType::L_Keyword);
        auto while_stmt = make_node<While_Stmt>();
        ExpectToken(Lex::ETokenType::S_LParenthesis);
        while_stmt->condition = ParseNode<Expression>();
        ExpectToken(Lex::ETokenType::S_RParenthesis);
        while_stmt->body = ParseNode<Statement>();
        return while_stmt;
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
        switch (lexer.PeekToken().type) {
        case Lex::ETokenType::U_Number:
            return ParseNode<Number>();
        case Lex::ETokenType::U_Identifer: {
            if (lexer.PeekToken(1).type == Lex::ETokenType::S_LParenthesis) {
                return ParseNode<Function>();
            } else return ParseNode<Variable>();
        }
            
        case Lex::ETokenType::S_LParenthesis: {
            auto prmy = make_node<Primary_Expr>();
            ExpectToken(Lex::ETokenType::S_LParenthesis);
            prmy->expr = ParseNode<Expression>();
            ExpectToken(Lex::ETokenType::S_RParenthesis);
            return prmy;
        }
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

    template<> node_ptr<Variable> Parser::ParseNode<Variable>() {
        bool found = false;
        auto var = make_node<Variable>();
        for (auto it{AreaList.rbegin()}; it != AreaList.rend(); ++it) {
            if ((*it)->Symbols.count(lexer.PeekToken().ident)) {
                var->sym_iter = (*it)->Symbols.find(lexer.PeekToken().ident);
                ExpectToken(Lex::ETokenType::U_Identifer);
                found = true;
            }
        }
        if (!found) {
            // TODO: 未定义变量
            throw std::runtime_error("Undefined variable: " + lexer.PeekToken().ident);
        }
        return var;
    }

    template<> node_ptr<Variable_Def> Parser::ParseNode<Variable_Def>() {
        auto var_def = make_node<Variable_Def>();
        if (AreaList.back()->Symbols.count(lexer.PeekToken().ident)) {
            // TODO: 变量重复定义
            throw std::runtime_error("Variable redefinition: " + lexer.PeekToken().ident);
        }
        AreaList.back()->Symbols[lexer.PeekToken().ident] = curUseType;
        var_def->sym_iter = AreaList.back()->Symbols.find(lexer.PeekToken().ident);
        ExpectToken(Lex::ETokenType::U_Identifer);
        return var_def;
    }

    template<>
    node_ptr<Type::Function> Parser::ParseNode<Type::Function>() {
        auto func = make_node<Type::Function>();
        func->func_def = funcdefs.back().find(lexer.PeekToken().ident)->second;
        ExpectToken(Lex::ETokenType::U_Identifer);
        ExpectToken(Lex::ETokenType::S_LParenthesis);
        while (lexer.PeekToken().type != Lex::ETokenType::S_RParenthesis) {
            func->args.push_back(ParseNode<Expression>());
            if (lexer.PeekToken().type == Lex::ETokenType::S_Comma) {
                lexer.NextToken();
            }
        }
        ExpectToken(Lex::ETokenType::S_RParenthesis);
        return func;
    }
    template<>
    node_ptr<Type::Function_Def> Parser::ParseNode<Type::Function_Def>() {
        auto func_def = make_node<Type::Function_Def>();
        if (AreaList.back()->Symbols.count(lexer.PeekToken().ident)) {
            // TODO: 函数重复定义
            throw std::runtime_error("Function redefinition: " + lexer.PeekToken().ident);
        } 
        AreaList.back()->Symbols[lexer.PeekToken().ident] = curUseType;
        func_def->sym_iter = AreaList.back()->Symbols.find(lexer.PeekToken().ident);
        funcdefs.back()[lexer.PeekToken().ident] = func_def;
        AreaList.push_back(&(*func_def));
        ExpectToken(Lex::ETokenType::U_Identifer);
        ExpectToken(Lex::ETokenType::S_LParenthesis);
        while (lexer.PeekToken().type != Lex::ETokenType::S_RParenthesis) {
            func_def->args.push_back(ParseNode<Variable_Def>());
            if (lexer.PeekToken().type == Lex::ETokenType::S_Comma) {
                lexer.NextToken();
            }
        }
        ExpectToken(Lex::ETokenType::S_RParenthesis);
        ExpectToken(Lex::ETokenType::S_LBrace);
        while (lexer.PeekToken().type != Lex::ETokenType::S_RBrace) {
            func_def->body.push_back(ParseNode<Statement>());
        }
        ExpectToken(Lex::ETokenType::S_RBrace);
        AreaList.pop_back();
        return func_def;
    }

} // namespace Simlan::Parse

