/*
 * “ут объ€влены общие используемые типы
 */

#pragma once

/**
 * Union of basic types.
 */
typedef union anytype_s {
    void* ptr;
    long i;
    unsigned long ui;
    double fl;
} anytype_t;

typedef signed char		int8;
typedef short			int16;
typedef int				int32;
typedef unsigned char	uint8;
typedef unsigned int	uint32;
typedef float			float32;
typedef double			float64;

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

#define MAX_INT32		((int32) 0x7fffffff)