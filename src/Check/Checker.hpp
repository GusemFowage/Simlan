#ifndef SIMLAN_CHECK_HPP
#define SIMLAN_CHECK_HPP

#include <Simlan/Simlan.hpp>
#include <Parse/Ast/Visitor.hpp>
#include <Parse/Ast/Node.hpp>

namespace Simlan :: Check {
    template<Parse::Ast::EAstNodeType Itype>
    using Node = Parse::Ast::Node<Itype>;
    
    class Checker : public Parse::Ast::Visitor {
        using enum Parse::Ast::EAstNodeType;
    private:
        Map<string, uint64_t>::iterator curval;
        List<uint64_t> stack;
        List<Map<string, uint64_t>> vals;
        
    private:
        size_t tabs;
        void tab();
        void cal(Node<Mid_Expr>::EOperator op);
    public:
        // 特化并重载 Visit 纯虚函数
        void Visit(Node<Program>& prg) override;
        void Visit(Node<Block>& blk) override;
        void Visit(Node<Expression_Stmt>& exs) override;
        void Visit(Node<IfElse_Stmt>& fls) override;
        void Visit(Node<While_Stmt>& whl) override;
        void Visit(Node<For_Stmt>& frst) override;
        void Visit(Node<Mid_Expr>& mid) override;
        void Visit(Node<Assign_Expr>& ass) override;
        void Visit(Node<Primary_Expr>& pry) override;
        void Visit(Node<Number>& num) override;
        void Visit(Node<String>& str) override;
        void Visit(Node<Boolean>& bln) override;
        void Visit(Node<Variable>& var) override;
        void Visit(Node<Variable_Def>& var) override;
        void Visit(Node<Function>& fnc) override;
        void Visit(Node<Function_Def>& fnc_def) override;
    };
}

#endif // ! SIMLAN_CHECK_HPP