#ifndef SIMLAN_LEX_SOURCE_HPP
#define SIMLAN_LEX_SOURCE_HPP
#include <Simlan/Simlan.hpp>

namespace Simlan :: Lex {
    enum class ESourceType {
        Inline,
        File,
    };

    struct SourceInfo {
        // 行号 列号
        size_t line, column;
        // 内容
        StringView content;
    };

    class Source {
    public:
        // 释放资源 (虚函数: 希望使用子类的析构)
        virtual ~Source() = default;
        // 是否会结束
        // true: 会结束
        // false: 不会结束
        virtual bool WillEnd() const noexcept = 0;
        // 开启获取代码 (enable==false 表示停止获取代码)
        virtual void EnableGet(bool enable=true) = 0;
    public:
        // 开始获取代码(启动)
        virtual void Start() = 0;
        // 重新获取代码(更新)
        virtual void Renew() = 0;
    public:
        // 获取下一字符, 返回:上一个字符
        // e.g. "ab" 返回 'a' 字符指针指向 'b'
        // 若结尾返回 '\0'
        virtual char_t NextChar() = 0;
        // 获取下一字符, 且不移动指针, 返回:获取到的字符
        // e.g. "ab" 字符指针指向 'a'
        // PeekChar(0) => 'a' 字符指针仍指向 'a'
        // PeekChar(1) => 'b' 字符指针仍指向 'a'
        virtual char_t PeekChar(size_t offset=0) = 0;
        // 是否可以获取下一字符
        virtual bool AbleGet() = 0;
        // 获取当前位置的源代码信息
        virtual SourceInfo GetSourceInfo() const = 0;
        
    };

    template<ESourceType Type>
    class SourceImpl : public Source {};

    // @TODO: inline source

    template<>        // file source
    class SourceImpl<ESourceType::File> 
        : public Source  {
    public:
        // 使用源代码文件的路径
        SourceImpl(const Path& src);
    public: 
        // override
        bool WillEnd() const noexcept override;
        void EnableGet(bool enable=true) override;
        void Start() override;
        void Renew() override;
        char_t NextChar() override;
        char_t PeekChar(size_t offset=0) override;
        bool AbleGet() override;
        SourceInfo GetSourceInfo() const override;
    private:
        // 保存源代码文件路径
        Path SrcPath;
        // 保存源代码文件内容
        List<String> Codes;
        // 保存当前行
        List<String>::iterator m_CurLine;
        // 当前字符
        String::iterator m_CurChar;
        // 是否开启获取
        // bool m_AbleGet;
    };
}

#endif // ! SIMLAN_LEX_SOURCE_HPP