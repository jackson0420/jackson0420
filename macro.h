#ifndef __MACRO_H__
#define __MACRO_H__

#include <string.h>
#include <assert.h>
#include "util.h"
#include "log.h"

#define ASSERT(x) \
    if(!(x)){ \
        LOG__ERROR(LOG_ROOT()) << "ASSERTION: " #x \
            <<"\nbacktrace:\n"\
            <<BacktraceToString(100,2,"     ");\
        assert(x);\
    }

#define ASSERT2(x, w) \
    if(!(x)){ \
        LOG__ERROR(LOG_ROOT()) << "ASSERTION: " #x \
            <<"\n" << w\
            <<BacktraceToString(100,2,"         ");\
        assert(x);\
    }


#endif