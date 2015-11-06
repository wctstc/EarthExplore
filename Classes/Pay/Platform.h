#ifndef __PLATFORM_H__
#define __PLATFORM_H__

#ifdef WIN32
#include "Win32Platform.h"
#else
#include "AndroidPlatform.h"
#endif

#endif
