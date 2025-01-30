#pragma once

#include <cstdio>

#define RENDERER_LOG(lvl, fmt, ...)                                                             \
    do {                                                                                \
        fprintf(stderr, "[" lvl "-%s:%d]" fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__); \
    } while (0);

#define RENDERER_CRITICAL_ERROR(fmt, ...) RENDERER_LOG("CRITIAL ERROR", fmt, ##__VA_ARGS__);

#define RENDERER_ERROR(fmt, ...) RENDERER_LOG("ERROR", fmt, ##__VA_ARGS__);

#if VERBOSE >= 1
#define RENDERER_WARNING(fmt, ...) RENDERER_LOG("WARNING", fmt, ##__VA_ARGS__);
#else
#define RENDERER_WARNING(fmt, ...)
#endif

#if VERBOSE >= 2
#define RENDERER_INFO(fmt, ...) RENDERER_LOG("INFO", fmt, ##__VA_ARGS__);
#else
#define RENDERER_INFO(fmt, ...)
#endif

#if VERBOSE >= 3
#define RENDERER_DEBUG(fmt, ...) RENDERER_LOG("DEBUG", fmt, ##__VA_ARGS__);
#else
#define RENDERER_DEBUG(fmt, ...)
#endif

