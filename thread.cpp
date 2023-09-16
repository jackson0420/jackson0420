#include "thread.h"
#include "log.h"
#include "util.h"

static thread_local Thread* t_thread = nullptr;
static thread_local std::string t_thread_name = "UNKNOW";
static Logger::ptr g_logger =  LOG_NAME("system");

Thread* Thread::GetThis(){
    return t_thread;
}

const std::string& Thread::GetName(){
    return t_thread_name;
}

void Thread::SetName(const std::string& name){
    if(t_thread){
        t_thread->m_name = name;
    }
    t_thread_name = name;
}

Thread::Thread(std::function<void()> cb,const std::string& name)
    :m_cb(cb)
    ,m_name(name){
    if(name.empty()){
        m_name="UNKNOW";
    }
    int rt = pthread_create(&m_thread,nullptr,&Thread::run,this);
    if(rt){
        LOG__ERROR(g_logger) << "pthread_create thread fail,rt=" << rt << "name=" << name;
        throw std::logic_error("pthread_create error");
    }
    m_semaphore.wait();

}

Thread::~Thread(){
    if(m_thread){
        pthread_detach(m_thread);//将一个线程设置为"分离状态"。分离状态的线程在其执行结束后会自动释放资源，无需其他线程等待它的结束或调用 
    }
}

void* Thread::run(void* arg){
    Thread* thread = (Thread*)arg;
    t_thread = thread;
    t_thread_name = thread->m_name;
    thread -> m_id = GetThreadId();
    pthread_setname_np(pthread_self(),thread->m_name.substr(0,15).c_str());

    std::function<void()> cb;
    cb.swap(thread->m_cb);

    thread->m_semaphore.notify();

    cb();
    return 0; 
}

void Thread::join(){
    if(m_thread){
        int rt = pthread_join(m_thread,nullptr);
        if(rt){
            LOG__ERROR(g_logger) << "pthread_join  thread fail,rt=" << rt << " name=" << m_name;
            throw std::logic_error("pthread_join error");
        }
        m_thread = 0;
    }
}

Semaphore::Semaphore(uint32_t count){
    if(sem_init(&m_semaphore,0,count)){
        throw std::logic_error("sem_init error");
    }
}

Semaphore::~Semaphore(){
    sem_destroy(&m_semaphore);
}

void Semaphore::wait(){
    if(sem_wait(&m_semaphore)){
        throw std::logic_error("sem_wait error");
    }
}

void Semaphore::notify(){
    if(sem_post(&m_semaphore)){
        throw std::logic_error("sem_post error");
    }
}
