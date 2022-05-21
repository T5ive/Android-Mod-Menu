#ifndef SUBSTRATE_LOG_HPP
#define SUBSTRATE_LOG_HPP

#if 0
#include <android/log.h>

#define MSLog(level, format, ...) ((void)__android_log_print(level, "NNNN", format, __VA_ARGS__))

#define MSLogLevelNotice ANDROID_LOG_INFO
#define MSLogLevelWarning ANDROID_LOG_WARN
#define MSLogLevelError ANDROID_LOG_ERROR

#else

#define MSLog(level, format, ...) printf(format, __VA_ARGS__)

#endif

#endif//SUBSTRATE_LOG_HPP
