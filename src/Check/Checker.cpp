#include "Checker.hpp"

#include <iostream>

using namespace std;

namespace Simlan :: Check{
    void Checker::cal(Node<Mid_Expr>::EOperator op) {
        using enum Node<Mid_Expr>::EOperator;
        auto r = stack.back();
        stack.pop_back();
        auto l = stack.back();
        stack.pop_back();
        switch (op) {
        case Add: {
            stack.push_back(l + r);
            break;
        }
        case Sub: {
            stack.push_back(l - r);
            break;
        }
        case Mul: {
            stack.push_back(l * r);
            break;
        }
        case Div: {
            stack.push_back(l / r);
            break;
        }
        default:
            break;
        }
    }

    void Checker::Visit(Node<Program> &prg) {
        for (auto &stmt : prg.stmts) {
            stmt->Accept(this);
        }
    }

    void Checker::Visit(Node<Block>& blk) { }
    void Checker::Visit(Node<Expression_Stmt>& exs) {
        exs.expr->Accept(this);
        cout << " => " << stack.back() << endl;
        stack.pop_back();
    }
    void Checker::Visit(Node<IfElse_Stmt>& fls) { }
    void Checker::Visit(Node<While_Stmt>& whl) { }
    void Checker::Visit(Node<For_Stmt>& frst) { }
    void Checker::Visit(Node<Mid_Expr>& mid) {
        using enum Node<Mid_Expr>::EOperator;
        mid.Lkd->Accept(this);
        mid.Rkd->Accept(this);
        switch (mid.oprt) {
        case Add: {
            cout << "+";
            break;
        }
        case Sub: {
            cout << "-";
            break;
        }
        case Mul: {
            cout << "*";
            break;
        }
        case Div: {
            cout << "/";
            break;
        }
        default:
            break;
        }
        cal(mid.oprt);
    }
    void Checker::Visit(Node<Assign_Expr>& ass) {
        ass.Rkd->Accept(this);
        ass.Lkd->Accept(this);
    }
    void Checker::Visit(Node<Primary_Expr>& pry) {
        pry.expr->Accept(this);
    }
    void Checker::Visit(Node<Number>& num) {
        std::cout << ' ' << num.value << ' ';
        stack.push_back(num.value);
    }
    void Checker::Visit(Node<String>& str) { }
    void Checker::Visit(Node<Boolean>& bln) { }
}
