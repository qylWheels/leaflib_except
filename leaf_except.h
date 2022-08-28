/**
 * @file:     leaf_except.h
 * @author:   qylWheels
 * @date:     27-Aug-2022
 * @brief:    APIs about exceptions
*/

#ifndef __LEAF_EXCEPT_H__
#define __LEAF_EXCEPT_H__

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
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
typedef struct _LeafExceptFrame _LeafExceptFrame;
struct _LeafExceptFrame {
    _LeafExceptFrame *prev;
    jmp_buf           env;
    const char       *file;
    int               line;
    LeafExceptInfo   *expection;
};
_LeafExceptFrame *_leaf_except_stack = NULL;


/* Internal Status */
enum {
    _kLeafExceptEntered = 0,
    _kLeafExceptRaised,
    _kLeafExceptHandled,
    _kLeafExceptFinalized
};

/* Internal function */
static inline void _raise_exception(LeafExceptInfo *e, const char *file,
                                    int line) {
    assert(e);
    _LeafExceptFrame *p = _leaf_except_stack;
    if (p == NULL) {
        fprintf(stderr, "Uncaught exception");
        if (*e)
            fprintf(stderr, " %s", *e);
        else
            fprintf(stderr, " 0x%p", e);
        if (file && line > 0)
            fprintf(stderr, " raised at [%s: %d]\n", file, line);
        fprintf(stderr, "aborting...\n");
        fflush(stderr);
        abort();
    }
    p->expection = e;
    p->file = file;
    p->line = line;
    _leaf_except_stack = _leaf_except_stack->prev;
    longjmp(p->env, _kLeafExceptRaised);
    return;
}

/* Exported macros */
#undef TRY
#define TRY                                                 \
do {                                                        \
    volatile int _leaf_except_flag;                         \
    _LeafExceptFrame _leaf_except_frame;                    \
    _leaf_except_frame.prev = _leaf_except_stack;           \
    _leaf_except_stack = &_leaf_except_frame;               \
    _leaf_except_flag = setjmp(_leaf_except_frame.env);     \
    if (_leaf_except_flag == _kLeafExceptEntered) {

#define EXCEPT(e)                                           \
        if (_leaf_except_flag == _kLeafExceptEntered)       \
            _leaf_except_stack = _leaf_except_stack->prev;  \
    } else if (_leaf_except_frame.expection == &(e)) {      \
        _leaf_except_flag = _kLeafExceptHandled;

#define ELSE                                                \
        if (_leaf_except_flag == _kLeafExceptEntered)       \
            _leaf_except_stack = _leaf_except_stack->prev;  \
    } else {                                                \
        _leaf_except_flag = _kLeafExceptHandled;

#define FINALLY                                             \
        if (_leaf_except_flag == _kLeafExceptEntered)       \
            _leaf_except_stack = _leaf_except_stack->prev;  \
    } {                                                     \
        if (_leaf_except_flag == _kLeafExceptEntered)       \
            _leaf_except_flag = _kLeafExceptFinalized;

#define END_TRY                                             \
        if (_leaf_except_flag == _kLeafExceptEntered)       \
            _leaf_except_stack = _leaf_except_stack->prev;  \
    }                                                       \
    if (_leaf_except_flag == _kLeafExceptRaised)            \
        _raise_exception(_leaf_except_frame.expection,      \
                         __FILE__,                          \
                         __LINE__);                         \
} while (0);

#define RAISE(e) _raise_exception(&(e), __FILE__, __LINE__);

#define RETURN                                              \
    _leaf_except_stack = _leaf_except_stack->prev;          \
    return

_LEAF_END_DECLS

#endif  /* __LEAF_EXCEPT_H__ */
