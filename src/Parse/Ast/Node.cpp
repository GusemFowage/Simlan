#include "Node.hpp"
namespace Simlan :: Parse :: Ast {
    void Node<EAstNodeType::Program>::Accept(Visitor *visitor) {
        visitor->Visit(*this);
    }
    void Node<EAstNodeType::Block>::Accept(Visitor *visitor) {
        visitor->Visit(*this);
    }
    void Node<EAstNodeType::Expression_Stmt>::Accept(Visitor *visitor) {
        visitor->Visit(*this);
    }
    void Node<EAstNodeType::IfElse_Stmt>::Accept(Visitor *visitor) {
        visitor->Visit(*this);
    }
    void Node<EAstNodeType::While_Stmt>::Accept(Visitor *visitor) {
        visitor->Visit(*this);
    }
    void Node<EAstNodeType::For_Stmt>::Accept(Visitor *visitor) {
        visitor->Visit(*this);
    }
    void Node<EAstNodeType::Mid_Expr>::Accept(Visitor *visitor) {
        visitor->Visit(*this);
    }
    void Node<EAstNodeType::Assign_Expr>::Accept(Visitor *visitor) {
        visitor->Visit(*this);
    }
    void Node<EAstNodeType::Primary_Expr>::Accept(Visitor *visitor) {
        visitor->Visit(*this);
    }
    void Node<EAstNodeType::Number>::Accept(Visitor *visitor) {
        visitor->Visit(*this);
    }
}