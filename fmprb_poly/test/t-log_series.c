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

    Copyright (C) 2012 Fredrik Johansson

******************************************************************************/

#include "fmprb_poly.h"


int main()
{
    long iter;
    flint_rand_t state;

    printf("log_series....");
    fflush(stdout);

    flint_randinit(state);

    /* compare with fmpq_poly */
    for (iter = 0; iter < 10000; iter++)
    {
        long m, n, qbits, rbits1, rbits2;
        fmpq_poly_t A, B;
        fmprb_poly_t a, b;

        qbits = 2 + n_randint(state, 200);
        rbits1 = 2 + n_randint(state, 200);
        rbits2 = 2 + n_randint(state, 200);

        m = 1 + n_randint(state, 20);
        n = 1 + n_randint(state, 20);

        fmpq_poly_init(A);
        fmpq_poly_init(B);

        fmprb_poly_init(a);
        fmprb_poly_init(b);

        fmpq_poly_randtest_not_zero(A, state, m, qbits);
        fmpq_poly_set_coeff_ui(A, 0, 1UL);

        fmpq_poly_log_series(B, A, n);
        fmprb_poly_set_fmpq_poly(a, A, rbits1);
        fmprb_poly_randtest(b, state, 1 + n_randint(state, 20), rbits1, 5);
        fmprb_poly_log_series(b, a, n, rbits2);

        if (!fmprb_poly_contains_fmpq_poly(b, B))
        {
            printf("FAIL\n\n");
            printf("bits2 = %ld\n", rbits2);

            printf("A = "); fmpq_poly_print(A); printf("\n\n");
            printf("B = "); fmpq_poly_print(B); printf("\n\n");

            printf("a = "); fmprb_poly_printd(a, 15); printf("\n\n");
            printf("b = "); fmprb_poly_printd(b, 15); printf("\n\n");

            abort();
        }

        fmpq_poly_clear(A);
        fmpq_poly_clear(B);

        fmprb_poly_clear(a);
        fmprb_poly_clear(b);
    }

    /* test aliasing */
    for (iter = 0; iter < 10000; iter++)
    {
        long m, n, qbits, rbits1, rbits2;
        fmpq_poly_t A;
        fmprb_poly_t a, b;

        qbits = 2 + n_randint(state, 200);
        rbits1 = 2 + n_randint(state, 200);
        rbits2 = 2 + n_randint(state, 200);

        m = 1 + n_randint(state, 20);
        n = 1 + n_randint(state, 20);

        fmpq_poly_init(A);
        fmprb_poly_init(a);
        fmprb_poly_init(b);

        do {
            fmpq_poly_randtest_not_zero(A, state, m, qbits);
        } while (fmpz_sgn(A->coeffs + 0) <= 0);

        fmprb_poly_set_fmpq_poly(a, A, rbits1);

        fmprb_poly_log_series(b, a, n, rbits2);
        fmprb_poly_log_series(a, a, n, rbits2);

        if (!fmprb_poly_equal(a, b))
        {
            printf("FAIL\n\n");
            printf("bits2 = %ld\n", rbits2);

            printf("A = "); fmpq_poly_print(A); printf("\n\n");

            printf("a = "); fmprb_poly_printd(a, 15); printf("\n\n");
            printf("b = "); fmprb_poly_printd(b, 15); printf("\n\n");

            abort();
        }

        fmpq_poly_clear(A);
        fmprb_poly_clear(a);
        fmprb_poly_clear(b);
    }

    /* test that exp(log(f)) contains f */
    for (iter = 0; iter < 10000; iter++)
    {
        long m, n, qbits, rbits1, rbits2, rbits3;
        fmpq_poly_t A;
        fmprb_poly_t a, b, c;

        qbits = 2 + n_randint(state, 200);
        rbits1 = 2 + n_randint(state, 200);
        rbits2 = 2 + n_randint(state, 200);
        rbits3 = 2 + n_randint(state, 200);

        m = 1 + n_randint(state, 20);
        n = 1 + n_randint(state, 20);

        fmpq_poly_init(A);
        fmprb_poly_init(a);
        fmprb_poly_init(b);
        fmprb_poly_init(c);

        do {
            fmpq_poly_randtest_not_zero(A, state, m, qbits);
        } while (fmpz_sgn(A->coeffs + 0) <= 0);

        fmprb_poly_set_fmpq_poly(a, A, rbits1);

        fmprb_poly_randtest(b, state, 1 + n_randint(state, 20), rbits1, 5);
        fmprb_poly_log_series(b, a, n, rbits2);
        fmprb_poly_exp_series_basecase(c, b, n, rbits3);

        fmpq_poly_truncate(A, n);

        if (!fmprb_poly_contains_fmpq_poly(c, A))
        {
            printf("FAIL\n\n");
            printf("bits2 = %ld\n", rbits2);
            printf("bits3 = %ld\n", rbits3);

            printf("A = "); fmpq_poly_print(A); printf("\n\n");

            printf("a = "); fmprb_poly_printd(a, 15); printf("\n\n");
            printf("b = "); fmprb_poly_printd(b, 15); printf("\n\n");
            printf("c = "); fmprb_poly_printd(c, 15); printf("\n\n");

            abort();
        }

        fmpq_poly_clear(A);
        fmprb_poly_clear(a);
        fmprb_poly_clear(b);
        fmprb_poly_clear(c);
    }

    flint_randclear(state);
    flint_cleanup();
    printf("PASS\n");
    return EXIT_SUCCESS;
}
