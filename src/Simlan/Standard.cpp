#include "Standard.hpp"

#include <iostream>
using namespace std;

#include <Lex/Lexer.hpp>
#include <Parse/Parser.hpp>
#include <Check/Checker.hpp>

namespace Simlan {
    Map<String, ESimlanKeyword> Standard::KeywordsTable {
        {"if", ESimlanKeyword::If},
        {"else", ESimlanKeyword::Else},
        {"while", ESimlanKeyword::While},
        {"for", ESimlanKeyword::For},
        {"break", ESimlanKeyword::Break},
        {"continue", ESimlanKeyword::Continue},
        {"return", ESimlanKeyword::Return},
        {"true", ESimlanKeyword::True},
        {"false", ESimlanKeyword::False},
        {"null", ESimlanKeyword::Null},
        // {"and", ESimlanKeyword::AND},
        // {"or", ESimlanKeyword::OR},
        // {"not", ESimlanKeyword::NOT},
        // {"xor", ESimlanKeyword::XOR},
    };

namespace Test {
    static void testLexer () {
        using namespace Lex;
        auto source {
            make_shared<SourceImpl<ESourceType::File>>
            ("/home/maker/project/Simlan/test/test.sn")
        };  
        Lexer lex(source);
        while (!lex.IsEnd()) {
            auto token = lex.NextToken();
            switch (token.type) {
            // case ETokenType::S_End:
            //     return;
            case ETokenType::L_Keyword:
                cout << "Keyword: ";
                break;
            case ETokenType::U_Number:
                cout << "Number: ";
                break;
            case ETokenType::U_Identifer:
                cout << "Identifer: ";
                break;
            case ETokenType::U_String:
                cout << "String: ";
                break;
            default:   
                cout << static_cast<char_t>(token.type) << '\n';
                continue;
            }
            cout << token.ident << '\n';
        }
    }
    static void testSimlan() {
        testLexer();
    }
}

    int SimlanMain(UnorderedSet<StringView> argv) {
        if (argv.count("--test")) {
            Test::testSimlan();
            return 0;
        }
        if (argv.count("--version") || argv.count("-v")) {
            cout << "Simlan 0.0.0000.0000.24aa01" << endl;
            return 0;
        }
        // TODO：实现标准分流器
        // TODO: 实现语法分析器
        // TODO：实现语义分析器
        // TODO：实现代码生成器
        try {
            auto source {
                make_shared<Lex::SourceImpl<Lex::ESourceType::File>>
                ("../demo/parse1.sn")
            };  // 获取源代码
            Lex::Lexer lexer(source);  // 词法分析器
            Parse::Parser parser(lexer);  // 语法分析器
            Check::Checker checker;  // 语义分析器
            while (!lexer.IsEnd()) {
            // TODO: parse 
            // TODO: code generation
                parser()->Accept(&checker);
            }
        } catch (const exception& e) {
            cerr << "Error: " << e.what() << endl;
        }
        return 0;
    }
}