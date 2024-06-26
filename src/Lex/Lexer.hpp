#ifndef SIMLAN_LEXER_HPP
#define SIMLAN_LEXER_HPP

#include <Simlan/Simlan.hpp>
#include <Lex/Source.hpp>

namespace Simlan :: Lex {
    // 定义Token类型
    enum class ETokenType : uint16_t {
        S_End='\0', S_Line = '\n',
        S_LBrace = '{', S_RBrace = '}',
        S_LParenthesis = '(', S_RParenthesis = ')',
        // 运算符
        S_Plus='+', S_Minus='-', S_Star='*', S_Slash='/', S_Percent='%',
        S_Equal='=', S_Greater='>', S_Less='<',S_Question='?',
        S_Exclamation='!',S_Ampersand='&',S_Pipe='|',
        // 标点符号
        S_Comma=',', S_Colon=':', S_Semicolon=';', S_Dot='.', 
        // 语言相关
        L_Keyword=0x101,
        U_Identifer, U_Number, U_String,
        Unkown=numeric_limits<uint16_t>::max(),
    };
    // Token结构
    struct Token {
        // 源文件信息
        SourceInfo info;
        // token类型
        ETokenType type;
        // 标识符
        string ident;
        // Token() : type(ETokenType::Unkown) {}
        Token(const SourceInfo& Iinfo, ETokenType Itype = ETokenType::Unkown)
            : info(Iinfo), type(Itype) {}
    };
    // 词法分析器
    class Lexer {
    public:
        static bool iswhite(int c);
        static bool isdigit(int c);
        static bool islower(int c);
        static bool isupper(int c);
        static bool isalpha(int c);
        // static bool isalnum(int c);
        static bool isident(int c);
    public:
        // 构造函数 传入源代码
        Lexer(shared<Source> source);
        // 获取下一个 token
        // e.g. [var] [a] [=] [123], 当前指针指向 [var]
        // 则返回 Token{type=L_Keyword, ident="var", info=SourceInfo{line=1, col=1}}
        // 且指针指向 [a]
        Token NextToken();
        // 获取下 offset 的 token
        // e.g. [var] [a] [=] [123], 当前指针指向 [var]
        // offset=0, 则返回 Token{type=L_Keyword, ident="var", info=SourceInfo{line=1, col=1}}
        // offset=1, 则返回 Token{type=U_Identifer, ident="a", info=SourceInfo{line=1, col=5}}
        // 且仍然指针指向 [var]
        Token PeekToken(size_t offset=0);
        // 判断是否到达词法分析结尾
        bool IsEnd();
    private:
        // 获取token，核心函数
        Token GainToken();
        // 跳过空白字符
        void SkipWhitespace();
        // 跳过注释
        bool SkipComment(); 
        // 获取字符串
        Token GetString();
        // 获取标识符
        Token GetIdentifier();
        // 获取数字
        Token GetNumber();
    protected:
        // 源文件
        shared<Source> Src;
    private:
        // 获取到的token
        List<Token> Tokens;
        // 是否会结束 true 则会结束, false 则不会结束
        bool m_WillEnd;
    };
}

#endif // ! SIMLAN_LEXER_HPP