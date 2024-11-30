#ifndef DEBUG_INFO_H
#define DEBUG_INFO_H

//#define DEBUG

    #ifdef DEBUG
        #define ON_DEBUG(arg) arg
    #else
        #define ON_DEBUG(arg)
    #endif

#endif
