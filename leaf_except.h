/**
 * @file:           leaf_except.h
 * @author:         qylWheels
 * @date:           27-Aug-2022
 * @brief:          APIs about exceptions
*/

#ifndef __LEAF_EXCEPT_H__
#define __LEAF_EXCEPT_H__

#include <setjmp.h>

#ifdef __cplusplus
# define _LEAF_BEGIN_DECLS extern "C" {
# define _LEAF_END_DECLS }
#else  /* cplusplus */
# define _LEAF_BEGIN_DECLS  /* empty */
# define _LEAF_END_DECLS  /* empty */
#endif  /* !cplusplus */

_LEAF_BEGIN_DECLS

/* Data structures */
typedef const char              *LeafExceptInfo;
typedef struct _LeafExceptFrame *LeafExceptFrame;
struct _LeafExceptFrame {
    LeafExceptFrame *prev;
    jmp_buf          env;
    const char      *file;
    int              line;
    LeafExceptInfo  *e;
};

/* Internal function */
static inline void _raise_exception(LeafExceptInfo *e, const char *file,
                                    int line) {

}

/* Exported macros */
#define TRY

#define EXCEPT(e)

#define ELSE

#define FINALLY

#define END_TRY

#define RAISE(e)

_LEAF_END_DECLS

#endif  /* __LEAF_EXCEPT_H__ */

