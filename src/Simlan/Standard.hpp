#ifndef SIMLAN_STANDARD_HPP
#define SIMLAN_STANDARD_HPP

#include "Simlan.hpp"

namespace Simlan {
    enum class ESimlanKeyword : uint8_t {
        // 关键字枚举
        Var, Const, Fnc,
        If, Else,
        While, For, Keep, Until,
        Break, Continue,
        Return, Throw, Catch,
        Class, Structure,
        True, False, Null,
    };
    // 定义标准
    class Standard {
    public:
        static Map<string, ESimlanKeyword> KeywordsTable;
    };
    
    // 标准运行程序
    int SimlanMain(UnorderedSet<string_view> argv);
}


#endif // ! SIMLAN_STANDARD_HPP