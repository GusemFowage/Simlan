#ifndef SIMLAN_PARSER_AST_VISITOR_HPP
#define SIMLAN_PARSER_AST_VISITOR_HPP

#include <Simlan/Simlan.hpp>

namespace Simlan :: Parse :: Ast {
    // 抽象树节点类型枚举
    enum class EAstNodeType {
        Base, 
        Program, Statement, Expression, 
        Block,
        IfElse_Stmt, While_Stmt, For_Stmt,
        Expression_Stmt,
        Assign_Expr, Mid_Expr, Primary_Expr, 
        Number, String, Boolean,
    };
    // 观察者
    class Visitor;
    // 抽象树节点基类
    template<EAstNodeType Type = EAstNodeType::Base>
    struct Node {
        using base_type = void*;
        virtual void Accept(Visitor* visitor) = 0;
        virtual ~Node() = default;
    };

    class Visitor {
    public:
        using enum EAstNodeType;
        // 删除未实现的 Visit 函数
        template<EAstNodeType Type> 
        void Visit(Node<Type>& node) = delete;
        // 特化并重载 Visit 纯虚函数
        virtual void Visit(Node<Program>& program) = 0;
        virtual void Visit(Node<Block>& block) = 0;
        virtual void Visit(Node<Expression_Stmt>& expr_stmt) = 0;
        virtual void Visit(Node<IfElse_Stmt>& ifelse_stmt) = 0;
        virtual void Visit(Node<While_Stmt>& while_stmt) = 0;
        virtual void Visit(Node<For_Stmt>& for_stmt) = 0;
        virtual void Visit(Node<Mid_Expr>& mid_expr) = 0;
        virtual void Visit(Node<Assign_Expr>& ass) = 0;
        virtual void Visit(Node<Primary_Expr>& primary_expr) = 0;
        virtual void Visit(Node<Number>& num) = 0;
        virtual void Visit(Node<String>& str) = 0;
        virtual void Visit(Node<Boolean>& boolean) = 0;
    };
} // namespace Simlan :: Parse :: Ast
#endif // ! SIMLAN_PARSER_AST_VISITOR_HPP