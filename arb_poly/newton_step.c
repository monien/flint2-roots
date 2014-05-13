/*=============================================================================

    This file is part of ARB.

    ARB is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    ARB is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with ARB; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA

=============================================================================*/
/******************************************************************************

    Copyright (C) 2013 Fredrik Johansson

******************************************************************************/

#include "arb_poly.h"

int
_arb_poly_newton_step(arb_t xnew, arb_srcptr poly, long len,
    const arb_t x,
    const arb_t convergence_interval,
    const arf_t convergence_factor, long prec)
{
    arf_t err;
    arb_t t, u, v;
    int result;

    arf_init(err);
    arb_init(t);
    arb_init(u);
    arb_init(v);

    arf_set_mag(err, arb_radref(x));
    arf_mul(err, err, err, MAG_BITS, ARF_RND_UP);
    arf_mul(err, err, convergence_factor, MAG_BITS, ARF_RND_UP);

    arf_set(arb_midref(t), arb_midref(x));
    mag_zero(arb_radref(t));

    _arb_poly_evaluate2(u, v, poly, len, t, prec);

    arb_div(u, u, v, prec);
    arb_sub(u, t, u, prec);

    arb_add_error_arf(u, err);

    if (arb_contains(convergence_interval, u) &&
        (arf_cmpabs_mag(arb_radref(u), arb_radref(x)) < 0))
    {
        arb_swap(xnew, u);
        result = 1;
    }
    else
    {
        arb_set(xnew, x);
        result = 0;
    }

    arb_clear(t);
    arb_clear(u);
    arb_clear(v);
    arf_clear(err);

    return result;
}

