#ifndef __FIBER_H__
#define __FIBER_H__

#include <ucontext.h>
#include <functional>
#include <memory>
#include "thread.h"

// std::enable_shared_from_this 是 C++ 标准库中的一个模板类，
// 通常用于帮助类在其成员函数中安全地生成 std::shared_ptr 对象
// 当最后一个持有对象引用的智能指针销毁时，对象才会被销毁。这种方式有助于避免悬垂指针和内存泄漏问题。
class Fiber : public std::enable_shared_from_this<Fiber>{
public:
    typedef std::shared_ptr<Fiber> ptr;
    
    //协程状态
    enum State{
        //初始化状态
        INIT,
        //暂停状态
        HOLD,
        //执行中状态
        EXEC,
        //结束状态
        TERM,
        //可执行状态
        READY
    };

public:
    Fiber(std::function<void()> cb, size_t stacksize = 0);
    ~Fiber();

    //重置协程函数，并重置状态
    void reset(std::function<void()> cd);
    //切换到当前协程执行
    void swapIn();
    //切换到后台执行
    void swapOut();

public:
    static void SetThis(Fiber* f);
    //返回当前协程
    static Fiber::ptr GetThis();
    //协程切换到后台，并设置为Ready状态
    static void YieldToReady();
    //协程切换到后台，并设置为Hold状态
    static void YieldToHold();
    //总协程数
    static uint64_t TotalFibers();

    static void MainFunc();
private:
    Fiber();

private:
    uint64_t m_id;
    uint32_t m_stacksize;
    State m_state = INIT; 

    ucontext_t m_ctx;
    void* m_stack = nullptr;

    std::function<void()> m_cb;
};




#endif