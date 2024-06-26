namespace Simlan :: Parse {
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
    node_ptr<Type::For_Stmt> Parser::ParseNode<Type::For_Stmt>();
    template<>
    node_ptr<Type::While_Stmt> Parser::ParseNode<Type::While_Stmt>();
    // template<>
    // node_ptr<Type::Return_Stmt> Parser::ParseNode<Type::Return_Stmt>();
    // template<>
    // node_ptr<Type::Break_Stmt> Parser::ParseNode<Type::Break_Stmt>();
    // template<>
    // node_ptr<Type::Continue_Stmt> Parser::ParseNode<Type::Continue_Stmt>();
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
    template<>
    node_ptr<Type::Function> Parser::ParseNode<Type::Function>();
    template<>
    node_ptr<Type::Function_Def> Parser::ParseNode<Type::Function_Def>();
} // namespace Simlan :: Parse