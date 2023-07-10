/*
    Copyright (C) 2023 Jean Kieffer

    This file is part of Arb.

    Arb is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include "acb_theta.h"

slong
acb_theta_uql_roots(acb_ptr r, acb_ptr w, acb_srcptr z, slong nb_z,
    const acb_mat_t tau, slong nb_steps, slong prec)
{
    slong g = acb_mat_nrows(tau);
    slong n = 1 << g;
    flint_rand_t state;
    acb_ptr x;
    acb_ptr res;
    slong k, j;
    slong hprec = -1;

    x = _acb_vec_init((nb_z + 1) * g);
    res = _acb_vec_init((nb_z + 1) * n * nb_steps);
    flint_randinit(state);

    for (j = 0; j < ACB_THETA_UQL_TRY; j++)
    {
        /* Get z', 2z' picked at random in [0,2] */
        for (k = 0; k < g; k++)
        {
            arb_urandom(acb_realref(&x[k]), state, prec);            
        }
        _acb_vec_scalar_mul_2exp_si(x, x, g, 1);
        _acb_vec_scalar_mul_2exp_si(x + g, x, g, 1);

        /* Get roots */
        for (k = 0; k < nb_z; k++)
        {
            _acb_vec_add(x + (k + 1) * 2 * g, x, z + k * g, g, prec);
            _acb_vec_add(x + (k + 1) * 2 * g + g, x + g, z + k * g, g, prec);
        }
        hprec = acb_theta_ql_roots(res, x, nb_z + 1, tau, nb_steps, prec);
        if (hprec >= 0)
        {
            break;
        }
    }
    _acb_vec_set(w, x, g);
    
    _acb_vec_clear(x, (nb_z + 1) * g);
    _acb_vec_clear(res, (nb_z + 1) * n * nb_steps);
    flint_randclear(state);
    return hprec;
}
