/**
 * @purpose: 获取与处理源代码
**/
#include "Source.hpp"
#include <fstream>

namespace Simlan :: Lex {
/**
 * inline 版本: 不断输入源代码
**/ 

/**
 * file 版本: 从文件读取源代码
**/
    SourceImpl<ESourceType::File>::SourceImpl(const Path& src) : SrcPath(src) {
        EnableGet(true);
    }

    bool SourceImpl<ESourceType::File>::WillEnd() const noexcept {
        return true;
    }

    void SourceImpl<ESourceType::File>::EnableGet(bool enable) {
        if (enable) {
            std::ifstream fin(SrcPath);
            if (!fin.is_open()) {
                throw std::runtime_error("Failed to open file: " + SrcPath.string());
            }
            string buff;
            while (getline(fin, buff)) {
                Codes.push_back(buff);
            }
            // 加入空行 (方便处理无代码文件)
            Codes.push_back("");
            fin.close();
            Start();
        } 
        return;
    }

    void SourceImpl<ESourceType::File>::Start() {
        m_CurLine = Codes.begin();
        m_CurChar = m_CurLine->begin();
    }
    void SourceImpl<ESourceType::File>::Renew() {
        Start();
    }

    char_t SourceImpl<ESourceType::File>::NextChar() {
        if (!AbleGet()) return '\0';
        if (m_CurChar == m_CurLine->end()) {
            m_CurLine++;
            if (m_CurLine == Codes.end()) {
                return '\0';
            }
            m_CurChar = m_CurLine->begin();
            return '\n';
        }
        return (*m_CurChar++);
    }
    char_t SourceImpl<ESourceType::File>::PeekChar(size_t offset) {
        if (!AbleGet()) return '\0';
        if (m_CurChar != m_CurLine->end() && offset == 0) return *m_CurChar;
        auto tmpLn = m_CurLine;
        auto tmpCh = m_CurChar;
        size_t tmp;
        while ((tmp = (tmpLn->end() - tmpCh)) <= offset) {
            offset -= tmp;
            if (offset==0) {
                return '\n';
            }
            tmpLn++;
            if (tmpLn == Codes.end()) {
                return '\0';
            }
            tmpCh = tmpLn->begin();
            offset--;
        }
        tmpCh += offset;
        return *tmpCh;
    }
    bool SourceImpl<ESourceType::File>::AbleGet() {
        return (m_CurLine != Codes.end());
    }
    SourceInfo SourceImpl<ESourceType::File>::GetSourceInfo() const {
        size_t l {1};
        for (auto m{Codes.begin()};m!=m_CurLine;l++) {
            m++;
        }
        return {
            .line = l,
            .column = static_cast<size_t>(std::distance(m_CurLine->begin(), m_CurChar) + 1),
            .content = *m_CurLine,
        };
    }
}