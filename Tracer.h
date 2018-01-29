//
// Created by liqh on 2018/1/29.
//

#ifndef MEM_TRACER_TRACER_H_
#define MEM_TRACER_TRACER_H_

#include <map>

// 覆盖全局的operator new及operator delete
//
void* operator new(size_t size, const char* file, long line);
void operator delete(void* p, const char* file, long line);
//

void operator delete(void* p);
//

class Tracer
{
private:
    // 嵌套的Entry类用来保存文件名及new所出现在文件中的行号
    class Entry{
    private :
        const char* file_;
        long line_;
    public:
        // 构造函数，默认值file为空，line为0
        Entry(const char* file=0, long line=0):file_(file),line_(line){}
        // 获取file_
        const char* File() const {return this->file_;}
        // 获取line_
        long Line() {return this->line_ ;}
    };
private:
    std::map<void*, Entry> mapEntry_;

public:
    // 声明静态变量，是否准备好，只有当ready为true时，才进行跟踪
    static bool ready;
public:
    // 构造函数与析构函数
    Tracer();
    ~Tracer();

    // 添加跟踪
    void Add(void*, const char*, long);
    // 移除跟踪
    void Remove(void*);
    // 打印内存泄漏情况
    void Dump();

};

extern Tracer NewTracer;
#endif // MEM_TRACER_TRACER_H_
