/*  Boolector: Satisfiability Modulo Theories (SMT) solver.
 *
 *  Copyright (C) 2018 Mathias Preiner.
 *
 *  This file is part of Boolector.
 *  See COPYING for more information on using this software.
 */

#ifndef BZLABVPROP_H_INCLUDED
#define BZLABVPROP_H_INCLUDED

#include "bzlabv.h"

struct BzlaBvDomain
{
  BzlaBitVector *lo;
  BzlaBitVector *hi;
};

typedef struct BzlaBvDomain BzlaBvDomain;

/* Create new bit-vector domain of width 'width' with low 0 and high ~0. */
BzlaBvDomain *bzla_bvprop_new_init(BzlaMemMgr *mm, uint32_t width);

/* Create new bit-vector domain with low 'lo' and high 'hi'. Makes copies of
 * lo/hi. */
BzlaBvDomain *bzla_bvprop_new(BzlaMemMgr *mm,
                              const BzlaBitVector *lo,
                              const BzlaBitVector *hi);

/* Delete bit-vector domain. */
void bzla_bvprop_free(BzlaMemMgr *mm, BzlaBvDomain *d);

/* Check whether bit-vector domain is valid, i.e., ~lo | hi == ones. */
bool bzla_bvprop_is_valid(BzlaMemMgr *mm, const BzlaBvDomain *d);

/* Check whether bit-vector domain is fixed, i.e., lo == hi */
bool bzla_bvprop_is_fixed(BzlaMemMgr *mm, const BzlaBvDomain *d);

/* Propagate domains 'd_x' and 'd_y' of z = (x = y). The domains for 'x' and
 * 'y' are either the same or the resulting domain 'res_xy' is invalid.
 * Domain 'res_d_z' is either fixed (if res_d_xy is fixed or invalid) or valid
 * (all values possible).
 */
void bzla_bvprop_eq(BzlaMemMgr *mm,
                    BzlaBvDomain *d_x,
                    BzlaBvDomain *d_y,
                    BzlaBvDomain **res_d_xy,
                    BzlaBvDomain **res_d_z);

// TODO:
// propagators:
//
// y = x
// y = ~x
// z = x & y
// y = x << n
// y = x >> n
// z = x o y
// y = x[n:m]
// x < y
//
//
// decomposed propagators:
// z = ite(b,x,y)
// z = x + y
// z = x * y
// z = x udiv y
// z = x urem y
#endif
