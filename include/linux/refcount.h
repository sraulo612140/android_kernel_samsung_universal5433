/*
 * include/linux/refcount.h
 *
 * Minimal refcount_t compatibility layer for 3.10 based on atomic_t.
 *
 * The upstream refcount_t guards against overflow/underflow saturation and
 * incorporates memory-barrier semantics.  For the limited set of users
 * backported into this kernel (kernel/bpf/btf.c) the plain atomic
 * implementation is sufficient and matches the pre-refcount_t (atomic based)
 * handling used across the rest of the 3.10 tree.
 */
#ifndef _LINUX_REFCOUNT_H
#define _LINUX_REFCOUNT_H

#include <linux/atomic.h>
#include <linux/bug.h>
#include <linux/compiler.h>
#include <linux/mutex.h>

typedef struct refcount_struct {
	atomic_t refs;
} refcount_t;

#define REFCOUNT_INIT(n)	{ .refs = ATOMIC_INIT(n) }

static inline void refcount_set(refcount_t *r, int n)
{
	atomic_set(&r->refs, n);
}

static inline unsigned int refcount_read(const refcount_t *r)
{
	return (unsigned int)atomic_read(&r->refs);
}

static inline void refcount_inc(refcount_t *r)
{
	atomic_inc(&r->refs);
}

static inline unsigned int refcount_inc_not_zero(refcount_t *r)
{
	return atomic_add_unless(&r->refs, 1, 0);
}

static inline bool refcount_dec_and_test(refcount_t *r)
{
	return atomic_dec_and_test(&r->refs);
}

#endif /* _LINUX_REFCOUNT_H */