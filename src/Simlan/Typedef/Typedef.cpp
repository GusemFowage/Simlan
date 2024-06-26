#include "Typedef.hpp"

namespace Simlan {
    UnorderedMap<string, TypePtr> TableOfType::StandardType {
        {"uint64_t", make_shared<Type<EType::Number>>()},
    };

    void TableOfType::add(const string &name, const Type<EType::Auto> &type){
        if (table.count(name) || StandardType.count(name)) {
            // TODO: 报错：类型符号已被占用
            throw std::logic_error("Type already defined");
        }
        else table[name] = make_shared<Type<EType::Auto>>(type);
    }
    TypePtr TableOfType::operator[](const string &name){
        if (table.count(name)) {
            return table.at(name);
        } else if (StandardType.count(name)) {
            return StandardType.at(name);
        } else return nullptr;  // TODO: 报错：类型符号未定义
    };
} // namespace Simlan
