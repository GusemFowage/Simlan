#include "Typedef.hpp"

namespace Simlan {
    UnorderedMap<String, TableOfType::TypePtr> TableOfType::StandardType {
        
    };

    void TableOfType::add(const String &name, const Type<EType::Auto> &type){
        if (table.count(name) || StandardType.count(name)) {
            // TODO: 报错：类型符号已被占用
            throw std::logic_error("Type already defined");
        }
        else table[name] = make_shared<Type<EType::Auto>>(type);
    }
    TableOfType::TypePtr TableOfType::operator[](const String &name){
        if (table.count(name) == 0) {
            // TODO：报错：类型符号未定义
            return nullptr;
        } else return table.at(name);
    };
} // namespace Simlan
