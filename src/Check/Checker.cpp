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

    void Checker::tab() {
        auto i = tabs;
        while (i--) {
            cout << "\t";
        }
    }

    void Checker::Visit(Node<Program> &prg) {
        tabs = 0;
        vals.emplace_back();
        for (auto &stmt : prg.stmts) {
            stmt->Accept(this);
            stack.clear();
        }
        vals.pop_back();
    }

    void Checker::Visit(Node<Block>& blk) {
        tab();
        tabs++;
        cout << "{\n";
        vals.emplace_back();
        for (auto &stmt : blk.stmts) {
            stmt->Accept(this);
        }
        vals.pop_back();
        tabs--;
        tab();
        cout << "}\n";
    }
    void Checker::Visit(Node<Expression_Stmt>& exs) {
        tab();
        cout << "( ";
        exs.expr->Accept(this);
        cout << " ) => " << stack.back() << ";" << endl;
        if (!stack.empty())stack.pop_back();
    }
    void Checker::Visit(Node<IfElse_Stmt>& fls) {
        // static_assert(, "Not implemented yet");
        tab();
        cout << "if (";
        fls.condition->Accept(this);
        cout << "|>" << std::boolalpha << (stack.back() != 0);
        cout << ") ";
        
        if (stack.back() != 0) {
            fls.stmt->Accept(this);
        } else if (fls.else_stmt) {
            cout << '\n';
            tab();
            cout << "else ";
            fls.else_stmt->Accept(this);
        }
    }
    void Checker::Visit(Node<While_Stmt>& whl) {
        cout << "while\n";
        whl.condition->Accept(this);
        while (stack.back() != 0) {
            whl.body->Accept(this);
            whl.condition->Accept(this);
        }
    }
    void Checker::Visit(Node<For_Stmt>& frst) {
        cout << "for\n";
        frst.init->Accept(this);
        // 缺陷：赋值语句所导致
        frst.condition->Accept(this);
        while (stack.back() != 0) {
            frst.body->Accept(this);
            frst.increment->Accept(this);
            frst.condition->Accept(this);
        }
    }
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
        tab();
        auto&[ident, type] {*vardef.sym_iter};
        // cout << "var " << ident;
        if (vardef.expr) {
            cout << " = ( ";
            vardef.expr->Accept(this);
            cout << " ) => " << stack.back() << ';' << '\n';
            vals.back()[ident] = stack.back();
            stack.pop_back();
        } else vals.back()[ident] = 0;
        // cout << '\n';
    }

    void Checker::Visit(Node<Function>& func) {
        // 从右向左依次入栈
        static_assert(DEBUG_MODE, "副作用不确定");
        for (auto i{func.args.begin()};i!=func.args.end();i++) {
            (*i)->Accept(this);
        } {
            List<uint64_t> save;
            for (auto& i : func.args) {
                save.push_back(stack.back());
                stack.pop_back();
            }
            for (auto& i : save) {
                stack.push_back(i);
            }
        }
        func.func_def->Accept(this);
    }
    void Checker::Visit(Node<Function_Def>& funcdef) {
        vals.emplace_back();
        // 从左向右依次出栈
        for (auto& i : funcdef.args) {
            i->Accept(this);
            vals.back()[i->sym_iter->first] = stack.back();
            stack.pop_back();
        }
        for (auto& s : funcdef.body) {
            s->Accept(this);
        }
        vals.pop_back();
    }
}
