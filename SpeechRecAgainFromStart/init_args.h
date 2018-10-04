#pragma once

/**
 * @struct arg_t
 * Argument definition structure.
 */
typedef struct arg_s {
	char const *name;   /**< Name of the command line switch */
	int type;           /**< Type of the argument in question */
	char const *deflt;  /**< Default value (as a character string), or NULL if none */
	char const *doc;    /**< Documentation/description string */
} arg_t;

/**
 * @name Values for arg_t::type
 * Bit indicating a required argument.
 */
#define ARG_REQUIRED (1<<0)
/**
 * Integer argument (optional).
 */
#define ARG_INTEGER  (1<<1)
/**
 * Floating point argument (optional).
 */
#define ARG_FLOAT (1<<2)
/**
 * String argument (optional).
 */
#define ARG_STRING   (1<<3)
/**
 * Boolean (true/false) argument (optional).
 */
#define ARG_BOOLEAN  (1<<4)
/**
 * Boolean (true/false) argument (optional).
 */
#define ARG_STRING_LIST  (1<<5)

/**
 * Берет значение переменной s и оборачивает в кавычки - превращает в строку.
 **/
#define ARG_STRINGIFY(s) ARG_STRINGIFY1(s)
#define ARG_STRINGIFY1(s) #s