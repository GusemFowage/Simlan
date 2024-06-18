#ifndef SIMLAN_TYPEDEF_HPP
#define SIMLAN_TYPEDEF_HPP

#include <Simlan/Simlan.hpp>

namespace Simlan {
    enum class EType { 
        Auto,
        Void,
        Number,
        Struct,
    };

    template<EType Itd = EType::Auto> 
    struct Type {
        // TODO: type definition
    };

    template<>
    struct Type<EType::Number>
        : public Type<EType::Auto> {
    };

    using TypePtr = shared<Type<EType::Auto>>;
    class TableOfType {
    private:
        static UnorderedMap<string, TypePtr> StandardType;
        UnorderedMap<string, TypePtr> table;
    public:
        void add(const string& name, const Type<EType::Auto>& type);
        TypePtr operator[](const string& name) ;
    };

    struct Defarea {
        using SymbolTable = UnorderedMap<string, shared<Type<EType::Auto>>>;
        SymbolTable Symbols;
        TableOfType TypeTable;
    };
} // namespace Simlan

#endif // ! SIMLAN_TYPEDEF_HPP