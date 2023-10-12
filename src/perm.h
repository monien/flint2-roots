/*
    Copyright (C) 2011 Sebastian Pancratz

    This file is part of FLINT.

    FLINT is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <https://www.gnu.org/licenses/>.
*/

#ifndef PERM_H
#define PERM_H

#ifdef PERM_INLINES_C
#define PERM_INLINE
#else
#define PERM_INLINE static __inline__
#endif

#include <stdlib.h>
#include <stdio.h>

#include "flint.h"
#include "fmpz.h"
#include "fmpz_mat.h"

#ifdef __cplusplus
 extern "C" {
#endif

/* Memory management *********************************************************/

PERM_INLINE slong * _perm_init(slong n)
{
    slong i, *vec;

    vec = (slong *) flint_malloc(n * sizeof(slong));

    if (!vec)
    {
        flint_printf("ERROR (_perm_init).\n\n");
        flint_abort();
    }

    for (i = 0; i < n; i++)
        vec[i] = i;

    return vec;
}

PERM_INLINE void _perm_clear(slong * vec)
{
    flint_free(vec);
}

/* Assignment ****************************************************************/

PERM_INLINE slong _perm_equal(const slong *vec1, const slong *vec2, slong n)
{
    slong i;

    for (i = 0; i < n; i++)
        if (vec1[i] != vec2[i])
            return 0;

    return 1;
}

PERM_INLINE void _perm_set(slong *res, const slong *vec, slong n)
{
    slong i;

    for (i = 0; i < n; i++)
        res[i] = vec[i];
}

PERM_INLINE void _perm_set_one(slong *vec, slong n)
{
    slong i;

    for (i = 0; i < n; i++)
        vec[i] = i;
}

PERM_INLINE void
 _perm_inv(slong *res, const slong *vec, slong n)
{
    slong i;

    if (res == vec)
    {
        slong *t = (slong *) flint_malloc(n * sizeof(slong));

        if (!t)
        {
            flint_printf("ERROR (_perm_inv).\n\n");
            flint_abort();
        }

        for (i = 0; i < n; i++)
            t[i] = vec[i];
        for (i = 0; i < n; i++)
            res[t[i]] = i;

        flint_free(t);
    }
    else
    {
        for (i = 0; i < n; i++)
            res[vec[i]] = i;
    }
}

/* Composition ***************************************************************/

PERM_INLINE void
_perm_compose(slong *res, const slong *vec1, const slong *vec2, slong n)
{
    slong i;

    if (res == vec1)
    {
        slong *t = (slong *) flint_malloc(n * sizeof(slong));

        for (i = 0; i < n; i++)
            t[i] = vec1[i];
        for (i = 0; i < n; i++)
            res[i] = t[vec2[i]];

        flint_free(t);
    }
    else
    {
        for (i = 0; i < n; i++)
            res[i] = vec1[vec2[i]];
    }
}

PERM_INLINE void
_perm_power (slong *x_p, slong *x, slong p, slong n)
{

  ulong tmp = labs (p);

  slong *w = _perm_init (n);

  _perm_set (x_p, w, n);
  _perm_set (w, x, n);

  for (slong j = 0; j < 8 * sizeof (ulong); j++)
    {
      if (tmp & 0x1)
	{
	  _perm_compose (x_p, x_p, w, n);
	}
      _perm_compose (w, w, w, n);
      tmp >>= 1;
    }

  if (p < 0)
    _perm_inv (x_p, x_p, n);

  _perm_clear (w);
}

/* Matrix representation ******************************************************/

PERM_INLINE void _perm_mat(fmpz_mat_t a, slong *x, slong n)
{
    fmpz_mat_zero(a);
    for (slong j = 0; j < n; j++)
    {
        fmpz_one(fmpz_mat_entry(a, j, x[j]));
    }
}

/* Randomisation *************************************************************/

int _perm_randtest(slong * vec, slong n, flint_rand_t state);

/* Parity ********************************************************************/

int _perm_parity(const slong * vec, slong n);

/* Input and output **********************************************************/

PERM_INLINE  int _long_vec_print(const slong * vec, slong len)
{
    slong i;

    flint_printf("%wd", len);
    if (len > 0)
    {
        flint_printf(" ");
        for (i = 0; i < len; i++)
            flint_printf(" %wd", vec[i]);
    }

    return 1;
}

PERM_INLINE int _perm_print(const slong * vec, slong n)
{
    slong i;

    flint_printf("%wd", n);
    if (n > 0)
    {
        flint_printf(" ");
        for (i = 0; i < n; i++)
            flint_printf(" %wd", vec[i]);
    }

    return 1;
}

void _perm_fprint_pretty(FILE *file, slong *x, slong n);

void _perm_print_pretty(slong *x, slong n);

char * _perm_get_str_pretty(slong *x, slong n);

#ifdef __cplusplus
}
#endif

#endif
