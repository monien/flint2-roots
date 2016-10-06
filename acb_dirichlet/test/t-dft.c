/*
    Copyright (C) 2016 Pascal Molin

    This file is part of Arb.

    Arb is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include "acb_dirichlet.h"

typedef void (*do_f) (acb_ptr w, acb_srcptr v, slong len, slong prec);

void
check_vec_eq_prec(acb_srcptr w1, acb_srcptr w2, slong len, slong prec, slong digits, ulong q, char f1[], char f2[])
{
    slong i;

    for (i = 0; i < len; i++)
    {
        if (!acb_overlaps(w1 + i, w2 + i))
        {
            flint_printf("FAIL\n\n");
            flint_printf("q = %wu, size = %wu\n", q, len);
            flint_printf("\nDFT differ from index %ld / %ld \n", i, len);
            flint_printf("\n%s =\n", f1);
            acb_vec_printd(w1, len, digits);
            flint_printf("\n%s =\n", f2);
            acb_vec_printd(w2, len, digits);
            flint_printf("\n\n");
            abort();
        }
        else if (acb_rel_accuracy_bits(w1 + i) < 30
                || acb_rel_accuracy_bits(w2 + i) < 30)
        {
            flint_printf("FAIL\n\n");
            flint_printf("q = %wu\n", q);
            flint_printf("\nDFT inaccurate from index %ld / %ld \n", i, len);
            flint_printf("\nnaive =\n");
            acb_printd(w1 + i, digits);
            flint_printf("\nfast =\n");
            acb_printd(w2 + i, digits);
            flint_printf("\nerrors %ld & %ld [prec = %wu]\n",
                    acb_rel_accuracy_bits(w1 + i),
                    acb_rel_accuracy_bits(w2 + i), prec);
            abort();
        }
    }
}


int main()
{

    slong k;
    slong prec = 100, digits = 30;
    slong nq = 13;
    ulong q[13] = { 2, 3, 4, 5, 6, 23, 10, 15, 30, 59, 308, 335, 961};
    flint_rand_t state;

    flint_printf("dft....");
    fflush(stdout);

    flint_randinit(state);

    /* Dirichlet group DFT */
    for (k = 0; k < nq; k++)
    {
        slong i, j, len;
        acb_dirichlet_group_t G;
        acb_dirichlet_conrey_t x, y;
        acb_t chiy;
        acb_ptr v, w1, w2;

        acb_dirichlet_group_init(G, q[k]);

        len = G->phi_q;
        v = _acb_vec_init(len);
        w1 = _acb_vec_init(len);
        w2 = _acb_vec_init(len);

        acb_dirichlet_conrey_init(x, G);
        acb_dirichlet_conrey_one(x, G);
        for (i = 0; i < len; i++)
            acb_randtest_precise(v + i, state, prec, 0);

        /* naive */
        acb_init(chiy);
        acb_dirichlet_conrey_init(y, G);
        acb_dirichlet_conrey_one(x, G);
        for (i = 0;  i < len; i++)
        {
            acb_zero(w1 + i);
            acb_dirichlet_conrey_one(y, G);
            for (j = 0; j < len; j++)
            {
                acb_dirichlet_pairing_conrey(chiy, G, x, y, prec);
                acb_addmul(w1 + i, chiy, v + j, prec);
                acb_dirichlet_conrey_next(y, G);
            }
            acb_dirichlet_conrey_next(x, G);
        }
        acb_clear(chiy);
        acb_dirichlet_conrey_clear(y);
        acb_dirichlet_conrey_clear(x);

        /* dft */
        acb_dirichlet_dft_conrey(w2, v, G, prec);

        check_vec_eq_prec(w1, w2, len, prec, digits, q[k], "naive", "group");

        _acb_vec_clear(v, len);
        _acb_vec_clear(w1, len);
        _acb_vec_clear(w2, len);

        acb_dirichlet_group_clear(G);
    }

    flint_randclear(state);
    flint_cleanup();
    flint_printf("PASS\n");
    return EXIT_SUCCESS;
}
