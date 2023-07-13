/**
 * @file oprate.h
 * @author your name (you@domain.com)
 * @brief 一些基本操作，这里提前定义好，方便后续使用
 * @version 0.1
 * @date 2023-07-13
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef INI_OPRATE_H
#define INI_OPRATE_H

#define ARRAYSIZE(x) (sizeof((x)) / sizeof((x)[0]))

inline static void *malloc_zero(uint32 size)
{
    void *p = malloc(size);
    if (p == NULL) return NULL;
    memset(p, 0, size);
    return p;
}

#define MALLOC_ZERO(size) malloc_zero((size))

#endif