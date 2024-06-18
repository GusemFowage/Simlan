#include "Checker.hpp"

#include <iostream>

using namespace std;

namespace Simlan :: Check{

    using enum Parse::Ast::EAstNodeType;

    void Checker::cal(Node<Mid_Expr>::EOperator op) {
        using enum Node<Mid_Expr>::EOperator;
        auto r = stack.back();
        stack.pop_back();
        auto l = stack.back();
        stack.pop_back();
        switch (op) {
        case Add: {
            stack.push_back(l + r); break;
        }
        case Sub: {
            stack.push_back(l - r); break;
        }
        case Mul: {
            stack.push_back(l * r); break;
        }
        case Div: {
            stack.push_back(l / r); break;
        }
        case Less: {
            stack.push_back(l < r); break;
        }
        case Greater: {
            stack.push_back(l > r); break;
        }
        case LessEqual: {
            stack.push_back(l <= r); break;
        }
        case GreaterEqual: {
            stack.push_back(l >= r); break;
        }
        case Equal: {
            stack.push_back(l == r); break;
        }
        default:
            break;
        }
    }

    void Checker::Visit(Node<Program> &prg) {
        vals.emplace_back();
        for (auto &stmt : prg.stmts) {
            stmt->Accept(this);
        }
        vals.pop_back();
    }

    void Checker::Visit(Node<Block>& blk) {
        cout << "\t{\n";
        vals.emplace_back();
        for (auto &stmt : blk.stmts) {
            stmt->Accept(this);
        }
        vals.pop_back();
        cout << "\t}\n";
    }
    void Checker::Visit(Node<Expression_Stmt>& exs) {
        cout << " (";
        exs.expr->Accept(this);
        cout << " ) => " << stack.back() << ";" << endl;
        stack.pop_back();
    }
    void Checker::Visit(Node<IfElse_Stmt>& fls) {
        cout << "if (";
        fls.condition->Accept(this);
        cout << "|>" << std::boolalpha << (stack.back() != 0);
        cout << ") ";
        
        if (stack.back() != 0) {
            fls.stmt->Accept(this);
        } else if (fls.else_stmt) {
            cout << "\nelse ";
            fls.else_stmt->Accept(this);
        }
    }
    void Checker::Visit(Node<While_Stmt>& whl) { }
    void Checker::Visit(Node<For_Stmt>& frst) { }
    void Checker::Visit(Node<Mid_Expr>& mid) {
        using enum Node<Mid_Expr>::EOperator;
        mid.Lkd->Accept(this);
        switch (mid.oprt) {
        case Add: {
            cout << "+"; break;
        }
        case Sub: {
            cout << "-"; break;
        }
        case Mul: {
            cout << "*"; break;
        }
        case Div: {
            cout << "/"; break;
        }
        case Less: {
            cout << "<"; break;
        }
        case Greater: {
            cout << ">"; break;
        }
        case LessEqual: {
            cout << "<="; break;
        }
        case GreaterEqual: {
            cout << ">="; break;
        }
        case Equal: {
            cout << "=="; break;
        }
        default:
            break;
        }
        mid.Rkd->Accept(this);
        cal(mid.oprt);
    }
    void Checker::Visit(Node<Assign_Expr>& ass) {
        ass.Rkd->Accept(this);
        cout << " => " << stack.back() << " := ";
        ass.Lkd->Accept(this);
        stack.pop_back();
        // cout << " := " << curval->first;
        curval->second = stack.back();
        // stack.pop_back();
        // stack.push_back(curval->second);
        // curval = vals.end()->end();
    }
    void Checker::Visit(Node<Primary_Expr>& pry) {
        // pry.expr->Accept(this);
    }
    void Checker::Visit(Node<Number>& num) {
        std::cout << ' ' << num.value << ' ';
        stack.push_back(num.value);
    }
    void Checker::Visit(Node<String>& str) { }
    void Checker::Visit(Node<Boolean>& bln) { }

    void Checker::Visit(Node<Variable>& var) {
        auto&[ident, type] {*var.sym_iter};
        for (auto it{vals.rbegin()}; it != vals.rend(); ++it) {
            auto vit = it->find(ident);
            if (vit == it->end()) {
                continue;
            }
            curval = vit;
            cout << curval->first << "{"<<curval->second<<"}";
            stack.push_back(curval->second);
            return;
        }

    }
    void Checker::Visit(Node<Variable_Def>& vardef) {
        auto&[ident, type] {*vardef.sym_iter};
        cout << "var " << ident;
        if (vardef.expr) {
            cout << " = ( ";
            vardef.expr->Accept(this);
            cout << " ) => " << stack.back() << ';' << endl;
            vals.back()[ident] = stack.back();
            stack.pop_back();
        } else vals.back()[ident] = 0;
    }
}
