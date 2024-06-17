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

    class TableOfType {
        using TypePtr = shared<Type<EType::Auto>>;
    private:
        static UnorderedMap<String, TypePtr> StandardType;
        UnorderedMap<String, TypePtr> table;
    public:
        void add(const String& name, const Type<EType::Auto>& type);
        TypePtr operator[](const String& name) ;
    };

    struct Area {
        UnorderedMap<String, shared<Type<EType::Auto>>> Symbols;
        TableOfType TypeTable;
    };
} // namespace Simlan

#endif // ! SIMLAN_TYPEDEF_HPP