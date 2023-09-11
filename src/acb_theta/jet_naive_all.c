/*
    Copyright (C) 2023 Jean Kieffer

    This file is part of Arb.

    Arb is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include "acb_theta.h"

static void
worker_dim0(acb_ptr dth, const acb_t term, slong* coords, slong g,
    slong ord, slong prec, slong fullprec)
{
    slong n = 1 << g;
    slong nb_max = acb_theta_jet_nb(ord, g);
    slong nb_tot = acb_theta_jet_nb(ord, g + 1);
    acb_t x;
    fmpz_t m;
    acb_ptr f;
    ulong a, b;
    slong k, j, i, nb, ind;
    slong* orders;

    acb_init(x);
    fmpz_init(m);
    orders = flint_malloc(g * nb_max * sizeof(slong));
    f = _acb_vec_init(nb_max);

    a = acb_theta_char_get_a(coords, g);
    ind = 0;
    for (k = 0; k <= ord; k++)
    {
        /* Get list of orders */
        nb = acb_theta_jet_nb(k, g);
        acb_theta_jet_orders(orders, k, g);

        /* Compute factor for each tuple */
        for (j = 0; j < nb; j++)
        {
            acb_one(&f[j]);
            for (i = 0; i < g; i++)
            {
                fmpz_set_si(m, coords[i]);
                fmpz_pow_ui(m, m, orders[j * g + i]);
                acb_mul_fmpz(&f[j], &f[j], m, prec);
                fmpz_fac_ui(m, orders[j * g + i]);
                acb_div_fmpz(&f[j], &f[j], m, prec);
            }
        }
        acb_const_pi(x, prec);
        acb_mul_onei(x, x);
        acb_pow_ui(x, x, k, prec);
        _acb_vec_scalar_mul(f, f, nb, x, prec);

        /* Loop over b */
        for (b = 0; b < n; b++)
        {
            acb_mul_powi(x, term, acb_theta_char_dot_slong(b, coords, g) % 4);
            for (j = 0; j < nb; j++)
            {
                acb_addmul(&dth[(n * a + b) * nb_tot + ind + j], x, &f[j], fullprec);
            }
        }

        ind += nb;
    }

    acb_clear(x);
    fmpz_clear(m);
    flint_free(orders);
    _acb_vec_clear(f, nb_max);
}

/* Use a big ellipsoid to avoid complicated formulas for derivatives */

static void
acb_theta_jet_naive_all_gen(acb_ptr dth, acb_srcptr z, const acb_mat_t tau,
    slong ord, slong prec)
{
    slong g = acb_mat_nrows(tau);
    slong n = 1 << g;
    acb_theta_eld_t E;
    acb_theta_precomp_t D;
    acb_t c;
    arb_t u;
    acb_mat_t new_tau;
    acb_ptr new_z;
    slong nb = n * n * acb_theta_jet_nb(ord, g + 1);

    acb_theta_eld_init(E, g, g);
    acb_theta_precomp_init(D, 1, g);
    acb_init(c);
    arb_init(u);
    acb_mat_init(new_tau, g, g);
    new_z = _acb_vec_init(g);

    _acb_vec_scalar_mul_2exp_si(new_z, z, g, -1);
    acb_mat_scalar_mul_2exp_si(new_tau, tau, -2);

    acb_theta_jet_ellipsoid(E, u, new_z, new_tau, ord, prec);
    prec = acb_theta_naive_fullprec(E, prec);
    acb_theta_precomp_set(D, new_z, new_tau, E, prec);
    acb_one(c);

    acb_theta_naive_worker(dth, nb, c, u, E, D, 0, ord, prec, worker_dim0);

    acb_theta_eld_clear(E);
    acb_theta_precomp_clear(D);
    acb_clear(c);
    arb_clear(u);
    acb_mat_clear(new_tau);
    _acb_vec_clear(new_z, g);
}

void
acb_theta_jet_naive_all(acb_ptr dth, acb_srcptr z, const acb_mat_t tau,
    slong ord, slong prec)
{
    slong g = acb_mat_nrows(tau);
    slong nb = acb_theta_jet_nb(ord, g + 1);
    acb_ptr res;

    if (g == 1)
    {
        res = _acb_vec_init(4 * nb);

        acb_modular_theta_jet(res, res + nb, res + 2 * nb, res + 3 * nb,
            z, acb_mat_entry(tau, 0, 0), nb, prec);
        _acb_vec_set(dth, res + 2 * nb, nb);
        _acb_vec_set(dth + nb, res + 3 * nb, nb);
        _acb_vec_set(dth + 2 * nb, res + nb, nb);
        _acb_vec_neg(dth + 3 * nb, res, nb);

        _acb_vec_clear(res, 4 * nb);
    }
    else
    {
        acb_theta_jet_naive_all_gen(dth, z, tau, ord, prec);
    }
}
