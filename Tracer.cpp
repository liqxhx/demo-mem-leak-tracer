//
// Created by liqh on 2018/1/29.
//

#include <iostream>
#include "Tracer.h"

// 默认已准备好
bool Tracer::ready = true;

// 构造函数与析构函数
Tracer::Tracer()
{
    ready = true;
}
Tracer::~Tracer()
{
    ready = false;
    Dump();
}

// 添加跟踪
void Tracer::Add(void* p, const char* file, long line)
{
    mapEntry_[p] = Entry(file, line);
}
// 移除跟踪
void Tracer::Remove(void* p)
{
    std::map<void*, Entry>::iterator it = mapEntry_.find(p);
    if(it != mapEntry_.end())
    {
        mapEntry_.erase(it); // 会调用void operator delete(void *p)
    }
}
// 打印内存泄漏情况
void Tracer::Dump()
{
    if(mapEntry_.size() > 0)
    {
        // 打印发生内存泄漏
        std::cout<<"***Memory leak(s)***"<<std::endl;
        std::map<void*, Entry>::iterator it;
        for(it = mapEntry_.begin(); it != mapEntry_.end(); ++it)
        {
            const char* file = it->second.File();
            long line = it->second.Line();
//            int addr = reinterpret_cast<int>(it->first);
            void *p = it->first;
//            std::cout<<"0x"<<std::hex<<addr<<": "<<file<<",Line: "<<std::dec<<line<<std::endl;
            std::cout<<p<<" : "<<file<<",Line: "<<std::dec<<line<<std::endl;
        }
        std::cout<<std::endl;
    }
}


Tracer NewTracer;

// 覆盖全局的operator new及operator delete
void* operator new(size_t size, const char* file, long line)
{
    std::cout<<"void* operator new(size_t size, const char* file, long line)"<<std::endl;
    void* p = malloc(size);
    if(Tracer::ready)
    {
        NewTracer.Add(p, file, line);
    }
    return p;
}
void operator delete(void* p, const char* file, long line)
{
    std::cout<<"void operator delete(void* p, const char* file, long line)"<<std::endl;
    if(Tracer::ready)
    {
        NewTracer.Remove(p);
    }
    free(p);
}

//
void operator delete(void* p)
{
    std::cout<<"void operator delete(void* p)"<<p<<std::endl;
    if(Tracer::ready)
    {
        NewTracer.Remove(p);
    }
    free(p);
}