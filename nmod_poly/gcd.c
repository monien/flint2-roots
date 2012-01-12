/*=============================================================================

    This file is part of FLINT.

    FLINT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    FLINT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with FLINT; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA

=============================================================================*/
/******************************************************************************

    Copyright (C) 2012 Sebastian Pancratz

******************************************************************************/

#include "nmod_poly.h"

long _nmod_poly_gcd(mp_ptr G, mp_srcptr A, long lenA, 
                              mp_srcptr B, long lenB, nmod_t mod)
{
    const long cutoff = FLINT_BIT_COUNT(mod.n) <= 8 ? 
                        NMOD_POLY_SMALL_GCD_CUTOFF : NMOD_POLY_GCD_CUTOFF;

    if (lenA < cutoff)
        return _nmod_poly_gcd_euclidean(G, A, lenA, B, lenB, mod);
    else
        return _nmod_poly_gcd_hgcd(G, A, lenA, B, lenB, mod);
}

void nmod_poly_gcd(nmod_poly_t G, const nmod_poly_t A, const nmod_poly_t B)
{
    long lenA = A->length, lenB = B->length, lenG;
    nmod_poly_t tG;
    mp_ptr g;

    if (lenA == 0)
    {
        if (lenB == 0) nmod_poly_zero(G);
        else nmod_poly_make_monic(G, B);
    } 
    else if (lenB == 0)
    {
        nmod_poly_make_monic(G, A);
    }
    else if (lenA == 1 || lenB == 1)
    {
        nmod_poly_one(G);
    }
    else
    {
        if (G == A || G == B)
        {
            nmod_poly_init2(tG, A->mod.n, FLINT_MIN(lenA, lenB));
            g = tG->coeffs;
        }
        else
        {
            nmod_poly_fit_length(G, FLINT_MIN(lenA, lenB));
            g = G->coeffs;
        }

        if (lenA >= lenB)
            lenG = _nmod_poly_gcd(g, A->coeffs, lenA, B->coeffs, lenB, A->mod);
        else
            lenG = _nmod_poly_gcd(g, B->coeffs, lenB, A->coeffs, lenA, A->mod);

        if (G == A || G == B)
        {
            nmod_poly_swap(tG, G);
            nmod_poly_clear(tG);
        }
        G->length = lenG;

        if (G->length == 1)
            G->coeffs[0] = 1;
        else
            nmod_poly_make_monic(G, G);
    }
}
