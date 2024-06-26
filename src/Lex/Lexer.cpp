/**
 * @file Lexer.cpp
 * 本文件定义了Lexer类，用于将源代码分割成词法单元。
 */
#include "Lexer.hpp"
#include <cassert>
#include <Simlan/Standard.hpp>
// #include <Alert/Alertor.hpp>

namespace Simlan :: Lex {
    bool Lexer::iswhite(int c) {
        return (c == ' ' || c == '\t' || c == '\r' || c == '\n');
    }
    bool Lexer::isdigit(int c) {
        return (c >= '0' && c <= '9');
    }
    bool Lexer::islower(int c) {
        return (c >= 'a' && c <= 'z');
    }
    bool Lexer::isupper(int c) {
        return (c >= 'A' && c <= 'Z');
    }
    bool Lexer::isalpha(int c) {
        return (islower(c) || isupper(c));
    }
    bool Lexer::isident(int c) {
        return (isalpha(c) || isdigit(c) || c == '_' || c == '$');
    }

    Lexer::Lexer(shared<Source> source) 
        : Src(source) {
        // TODO: 初始化Lexer
        if (!Src->AbleGet()) Src->EnableGet();
    }
    Token Lexer::NextToken() {
        if (Tokens.empty()) return GainToken();
        auto ret = Tokens.front();
        Tokens.pop_front();
        return ret;
    }
    Token Lexer::PeekToken(size_t offset) {
        if (offset == numeric_limits<decltype(offset)>::max())
            throw std::out_of_range("offset out of range");
        // 注：offset + 1 可以防止 size == 0 所导致的错误
        if (offset+1 <= Tokens.size()) {
            auto ret = Tokens.begin();
            std::advance(ret, offset);
            return *ret;
        }
        offset = offset - Tokens.size() + 1;
        for (;offset>0;offset--) {
            Tokens.push_back(GainToken());   
        }
        return Tokens.back();
    }
    bool Lexer::IsEnd() {
        if (Src->WillEnd()) {
            return (!Src->AbleGet() && Tokens.empty());
        } else return false;
    }
    Token Lexer::GainToken() {
        SkipWhitespace();
        auto c = Src->PeekChar();
        if (isdigit(c)) {
            // 注：数字放在前面处理，以避免数字作为标识符开头
            return GetNumber();
        } else if (isident(c)) {
            return GetIdentifier();
        } else if (c == '\"') {
            return GetString();
        } else 
            c = Src->NextChar();
        switch (c) {
        case '\0':
            return Token{Src->GetSourceInfo(), ETokenType::S_End};
        default:
            return Token{Src->GetSourceInfo(), ETokenType(c)};
        }
    }
    void Lexer::SkipWhitespace(){
        if (!Src->WillEnd()) {
            if (!Src->AbleGet()) Src->Renew();
            else return;
        } 
        // 注：运用短路 || 
        while (iswhite(Src->PeekChar()) || SkipComment()) {
            Src->NextChar();
        }
    }

    bool Lexer::SkipComment() {
        if (Src->PeekChar() == '/' && Src->PeekChar(1) == '/') {
            while (Src->AbleGet() && (Src->PeekChar() != '\n')) Src->NextChar();
            // 此时指针指向 '\n' 下一个字符
            return true;
        } else if (Src->PeekChar() == '/' && Src->PeekChar(1) == '*') {
            // 避免 /*/ 导致的错误
            Src->NextChar();    // 跳过 '/' 此时指针指向 '*'
            while (Src->AbleGet() && (Src->NextChar() != '*' || Src->PeekChar() != '/'));
            if (!Src->AbleGet()) {
                // TODO: 报错
                throw std::runtime_error("Unexpected end of file");
                // Message({Level::ERROR,Src->GetSourceInfo(), "缺失 \'*/\' 结束注释" });
                exit(-1);
            }
            // 此时指针指向 '/' 
            // 则：程序会在 SkipWhitespace() 中跳过 '/'
            return true;
        }
        return false;
    }
    Token Lexer::GetString() {
        Token ret{Src->GetSourceInfo(), ETokenType::U_String};
        List<char_t> str;
        Src->NextChar();    // 跳过 '\"'
        while (Src->PeekChar() != '\"') {
            str.push_back(Src->NextChar());
        }
        Src->NextChar();    // 跳过 '\"'
        ret.ident.reserve(str.size());
        for (auto c : str) {
            ret.ident.push_back(c);
        }
        return ret;
    }
    Token Lexer::GetIdentifier() {
        Token ret{Src->GetSourceInfo(), ETokenType::U_Identifer};
        List<char_t> ident;
        while (isident(Src->PeekChar())) {
            ident.push_back(Src->NextChar());
        }
        ret.ident.reserve(ident.size());
        for (auto c : ident) {
            ret.ident.push_back(c);
        }
        if (Standard::KeywordsTable.count(ret.ident)) {
            ret.type = ETokenType::L_Keyword;
        }
        return ret;
    }
    Token Lexer::GetNumber() {
        bool isFloat = false;
        Token ret{Src->GetSourceInfo(), ETokenType::U_Number};
        List<char_t> num;
        auto c = Src->PeekChar();
        while (isdigit(c)) {
            num.push_back(Src->NextChar());
            c = Src->PeekChar();
            if (c == '_') {
                Src->NextChar();
                c = Src->PeekChar();
            } else if (c == '.') {
                num.push_back(Src->NextChar());
                isFloat = true;
                c = Src->PeekChar();
            }
        }
        ret.ident.reserve(num.size());
        for (auto c : num) {
            ret.ident.push_back(c);
        }
        return ret;
    }
}
