#include "sleep.h"

#ifdef WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

void sleep_ms(int ms)
{
#ifdef WIN32
    Sleep(ms);
#else
    usleep(ms * 1000);
#endif
}
