#include "log.h"
#include "util.h"
#include "config.h"
#include "thread.h"

Logger::ptr g_logger = LOG_ROOT();

int count = 0; 

void fun1(){
    LOG__INFO(g_logger) << "name: " << Thread::GetName()
                        << " this.name: " << Thread::GetThis()->getName()
                        << " id: " << GetThreadId()
                        <<" this.id:" << Thread::GetThis()->getId();
    for(int i = 0;i < 10000;++i){
        ++count;
    }
}

int main(){
    LOG__INFO(g_logger) << "thread test begin";
    std::vector<Thread::ptr> thrs;
    for(int i=0;i<5;++i){
        Thread::ptr thr(new Thread(&fun1,"name_" + std::to_string(i)));
        thrs.push_back(thr);
    }

    for(int i=0;i<5;++i){
        thrs[i]->join();
    }
    LOG__INFO(g_logger) << "thread test end";
    LOG__INFO(g_logger) << "count:" << count;






    // Logger::ptr logger(new Logger);
    // logger->addAppender(LogAppender::ptr(new StdoutLogAppender));

    // FileLogAppender::ptr file_appender(new FileLogAppender("./log.txt"));
    // LogFormatter::ptr fmt(new LogFormatter("%d%T%p%T%m%n"));
    // file_appender->setFormatter(fmt);
    // file_appender->setLevel(LogLevel::ERROR);

    // logger->addAppender(file_appender);

    // // LogEvent::ptr event(new LogEvent(__FILE__,__LINE__,0,GetThreadId(),GetFiberId(),time(0)));

    // // logger->log(LogLevel::DEBUG,event); 

    // LOG__INFO(logger) << "test";
    // LOG__ERROR(logger) << "test";

    // LOG_FMT_ERROR(logger,"test %s","aa");
    
    // auto l=LoggerMgr::GetInstance()->getLogger("xx");
    // LOG__INFO(l) << "xxx"; 


    return 0;
}